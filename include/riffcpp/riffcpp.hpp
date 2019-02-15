#ifndef RIFFCPP_H
#define RIFFCPP_H

#include <array>
#include <cstdint>
#include <istream>
#include <iterator>
#include <vector>

namespace riffcpp {
  /**
    Represents a FourCC

    This is a sequence of four bytes used to identify the various types of RIFF
    chunks
  */
  using FourCC = std::array<char, 4>;

  /// The `RIFF` FourCC, used to identify toplevel chunks
  constexpr FourCC riff_id = {'R', 'I', 'F', 'F'};

  /// The `LIST` FourCC, used to identify chunks that contain other chunks
  constexpr FourCC list_id = {'L', 'I', 'S', 'T'};

  class ChunkIt;

  /**
    Represents a RIFF chunk

    Every chunk has a four byte identifier (FourCC) and some contents.
    Depending on the value of the identifier, the chunk may contain other chunks
    as its contents, and in those cases a second FourCC is used to distinguish
    the chunk type.
  */
  class Chunk {
    std::istream &m_stream;
    std::streampos m_pos;

  public:
    /**
      Reads a chunk from the specified stream position

      The chunk's data is not read initially, it is only loaded when requested
      via the various methods provided.

      The stream needs to be able to seek to arbitrary positions.
    */
    Chunk(std::istream &stream, std::streampos pos);

    /// The chunk's identifier
    FourCC id();

    /// If the chunk contains other chunks, this is its type FourCC
    FourCC type();

    /// Returns the size of the chunk's contents in bytes
    std::uint32_t size();

    /**
      If this chunk contains other chunks, returns an iterator to the first
      chunk contained

      `no_chunk_id` is used for chunks which have no chunk id but still contain
      subchunks, like `seqt` from DirectMusic
    */
    ChunkIt begin(bool no_chunk_id = false);

    /**
      If this chunk contains other chunks, returns an iterator pointing past the
      last chunk contained
    */
    ChunkIt end();

    /**
      Returns the raw contents of the chunk
    */
    std::vector<char> data();
  };

  /**
    Provides a way to iterate over subchunks
  */
  class ChunkIt {
    std::streampos m_pos;
    std::istream &m_stream;

  public:
    /// Creates an iterator starting from the specified stream position
    ChunkIt(std::istream &stream, std::streampos pos);

    /// Returns whether two iterators point to the same chunk
    bool operator==(const ChunkIt &a) const;
    /// Returns whether two iterators do not point to the same chunk
    bool operator!=(const ChunkIt &a) const;

    /// Returns the chunk pointed by the iterator
    Chunk operator*() const;

    /// Moves the iterator ahead, to point to the following iterator
    ChunkIt &operator++();

    /**
      Moves the iterator ahead, to point to the following iterator and returns
      an iterator to the current position
    */
    ChunkIt operator++(int);
  };
} // namespace riffcpp

namespace std {
  template <> struct iterator_traits<riffcpp::ChunkIt> {
    using value_type = riffcpp::Chunk;
    using pointer = riffcpp::Chunk *;
    using iterator_category = std::input_iterator_tag;
  };
} // namespace std

#ifdef RIFFCPP_IMPLEMENTATION
riffcpp::Chunk::Chunk(std::istream &stream, std::streampos pos)
  : m_stream(stream), m_pos(pos) {}

riffcpp::FourCC riffcpp::Chunk::id() {
  m_stream.seekg(m_pos);
  riffcpp::FourCC read_id;
  m_stream.read(read_id.data(), read_id.size());
  return read_id;
}

std::uint32_t riffcpp::Chunk::size() {
  std::streamoff offs{4};
  m_stream.seekg(m_pos + offs);
  uint32_t read_size;
  m_stream.read(reinterpret_cast<char *>(&read_size), 4);
  return read_size;
}

riffcpp::FourCC riffcpp::Chunk::type() {
  std::streamoff offs{8};
  m_stream.seekg(m_pos + offs);
  riffcpp::FourCC read_type;
  m_stream.read(read_type.data(), read_type.size());
  return read_type;
}

std::vector<char> riffcpp::Chunk::data() {
  std::streamoff offs{8};
  m_stream.seekg(m_pos + offs);

  std::uint32_t data_size = size();

  std::vector<char> read_data;
  read_data.resize(data_size);

  m_stream.read(read_data.data(), data_size);
  return read_data;
}

riffcpp::ChunkIt riffcpp::Chunk::begin(bool no_chunk_id) {
  std::streamoff offs{no_chunk_id ? 8 : 12};
  return riffcpp::ChunkIt(m_stream, m_pos + offs);
}

riffcpp::ChunkIt riffcpp::Chunk::end() {
  std::uint32_t sz = size();
  std::streamoff offs{sz + sz % 2 + 8};
  return riffcpp::ChunkIt(m_stream, m_pos + offs);
}

riffcpp::ChunkIt::ChunkIt(std::istream &stream, std::streampos pos)
  : m_stream(stream), m_pos(pos) {}

bool riffcpp::ChunkIt::operator==(const ChunkIt &a) const {
  return m_pos == a.m_pos;
}
bool riffcpp::ChunkIt::operator!=(const ChunkIt &a) const {
  return !(*this == a);
}

riffcpp::Chunk riffcpp::ChunkIt::operator*() const {
  return riffcpp::Chunk(m_stream, m_pos);
}

riffcpp::ChunkIt &riffcpp::ChunkIt::operator++() {
  riffcpp::Chunk chunk(m_stream, m_pos);
  std::uint32_t sz = chunk.size();
  std::streamoff offs{sz + sz % 2 + 8};

  m_pos += offs;
  return *this;
}

riffcpp::ChunkIt riffcpp::ChunkIt::operator++(int) {
  riffcpp::ChunkIt it(m_stream, m_pos);

  riffcpp::Chunk chunk(m_stream, m_pos);
  std::uint32_t sz = chunk.size();
  std::streamoff offs{sz + sz % 2 + 8};

  m_pos += offs;

  return it;
}
#endif

#endif // RIFFCPP_H
#ifndef RIFFCPP_H
#define RIFFCPP_H

#include <array>
#include <cstdint>
#include <istream>
#include <iterator>
#include <vector>

namespace riffcpp {
  /// Represents a FourCC
  /**
    This is a sequence of four bytes used to identify the various types of RIFF
    chunks
  */
  using FourCC = std::array<char, 4>;

  /// The `RIFF` FourCC, used to identify toplevel chunks
  constexpr FourCC riff_id = {'R', 'I', 'F', 'F'};

  /// The `LIST` FourCC, used to identify chunks that contain other chunks
  constexpr FourCC list_id = {'L', 'I', 'S', 'T'};

  class ChunkIt;

  /// Represents a RIFF chunk
  /**
    Every chunk has a four byte identifier (FourCC) and some contents.
    Depending on the value of the identifier, the chunk may contain other chunks
    as its contents, and in those cases a second FourCC is used to distinguish
    the chunk type.
  */
  class Chunk {
    std::istream &m_stream;
    std::streampos m_pos;

  public:
    /// Reads a chunk from the specified stream position
    /**
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

    /// Provides a way to iterate over subchunks
    class iterator {
      std::streampos m_pos;   ///< Position of the chunk in the stream
      std::istream &m_stream; ///< Stream of the chunk

    public:
      using value_type = riffcpp::Chunk;
      using reference = value_type &;
      using pointer = value_type *;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::input_iterator_tag;

      /// Creates an iterator starting from the specified stream position
      iterator(std::istream &stream, std::streampos pos);

      /// Returns whether two iterators point to the same chunk
      bool operator==(const iterator &a) const;
      /// Returns whether two iterators do not point to the same chunk
      bool operator!=(const iterator &a) const;

      /// Returns the chunk pointed by the iterator
      Chunk operator*() const;

      /// Moves the iterator ahead, to point to the following iterator
      iterator &operator++();

      /// Moves the iterator ahead, to point to the following iterator and
      /// returns an iterator to the current position
      iterator operator++(int);
    };

    /// If this chunk contains other chunks, returns an iterator to the first
    /// chunk contained
    /**
      `no_chunk_id` is used for chunks which have no chunk id but still contain
      subchunks, like `seqt` from DirectMusic
    */
    iterator begin(bool no_chunk_id = false);

    ///  If this chunk contains other chunks, returns an iterator pointing past
    ///  the
    /// last chunk contained
    iterator end();

    /// Returns the raw contents of the chunk
    std::vector<char> data();

    using value_type = Chunk;
    using reference = value_type &;
    using const_reference = const Chunk &;
    using pointer = value_type *;
    using const_pointer = const pointer;
    using const_iterator = const iterator;
  };
} // namespace riffcpp
#endif // RIFFCPP_H
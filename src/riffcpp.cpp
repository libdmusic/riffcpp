#include <riffcpp/riffcpp.hpp>

using namespace riffcpp;

Chunk::Chunk(std::istream &stream, std::streampos pos)
  : m_stream(stream), m_pos(pos) {}

FourCC Chunk::id() {
  m_stream.seekg(m_pos);
  FourCC read_id;
  m_stream.read(read_id.data(), read_id.size());
  return read_id;
}

std::uint32_t Chunk::size() {
  std::streamoff offs{4};
  m_stream.seekg(m_pos + offs);
  uint32_t read_size;
  m_stream.read(reinterpret_cast<char *>(&read_size), 4);
  return read_size;
}

FourCC Chunk::type() {
  std::streamoff offs{8};
  m_stream.seekg(m_pos + offs);
  FourCC read_type;
  m_stream.read(read_type.data(), read_type.size());
  return read_type;
}

std::vector<char> Chunk::data() {
  std::streamoff offs{8};
  m_stream.seekg(m_pos + offs);

  std::uint32_t data_size = size();

  std::vector<char> read_data;
  read_data.resize(data_size);

  m_stream.read(read_data.data(), data_size);
  return read_data;
}

ChunkIt Chunk::begin() {
  std::streamoff offs{12};
  return ChunkIt(m_stream, m_pos + offs);
}

ChunkIt Chunk::end() {
  std::uint32_t sz = size();
  std::streamoff offs{sz + sz % 2 + 8};
  return ChunkIt(m_stream, m_pos + offs);
}

ChunkIt::ChunkIt(std::istream &stream, std::streampos pos)
  : m_stream(stream), m_pos(pos) {}

bool ChunkIt::operator==(const ChunkIt &a) const { return m_pos == a.m_pos; }
bool ChunkIt::operator!=(const ChunkIt &a) const { return !(*this == a); }

Chunk ChunkIt::operator*() const { return Chunk(m_stream, m_pos); }

ChunkIt &ChunkIt::operator++() {
  Chunk chunk(m_stream, m_pos);
  std::uint32_t sz = chunk.size();
  std::streamoff offs{sz + sz % 2 + 8};

  m_pos += offs;
  return *this;
}

ChunkIt ChunkIt::operator++(int) {
  ChunkIt it(m_stream, m_pos);

  Chunk chunk(m_stream, m_pos);
  std::uint32_t sz = chunk.size();
  std::streamoff offs{sz + sz % 2 + 8};

  m_pos += offs;

  return it;
}
#include <riffcpp.hpp>

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
#include "membuffer.hpp"
#include <fstream>
#include <riffcpp.hpp>

using iter = riffcpp::Chunk::iterator;

class riffcpp::Chunk::iterator::impl {
public:
  std::shared_ptr<std::istream> m_stream = nullptr;
  std::streampos m_pos;
};

class riffcpp::Chunk::impl {
public:
  std::shared_ptr<std::istream> m_stream = nullptr;
  std::shared_ptr<std::streambuf> m_buf = nullptr;
  std::streampos m_pos;
  std::streampos m_limit;
};

riffcpp::Chunk::Chunk(const char *filename) {
  pimpl = std::make_unique<riffcpp::Chunk::impl>();
  auto stream = std::make_shared<std::ifstream>(filename, std::ios::binary);

  if (!stream->is_open()) {
    throw std::runtime_error("Couldn't open the specified file");
  }

  pimpl->m_stream = stream;
  pimpl->m_pos = pimpl->m_stream->tellg();
}

riffcpp::Chunk::Chunk(const Chunk &ch) {
  pimpl = std::make_unique<riffcpp::Chunk::impl>();
  pimpl->m_buf = ch.pimpl->m_buf;
  pimpl->m_stream = ch.pimpl->m_stream;
  pimpl->m_pos = ch.pimpl->m_pos;
}

riffcpp::Chunk &riffcpp::Chunk::operator=(const riffcpp::Chunk &lhs) {
  pimpl->m_buf = lhs.pimpl->m_buf;
  pimpl->m_stream = lhs.pimpl->m_stream;
  pimpl->m_pos = lhs.pimpl->m_pos;
  return *this;
}

riffcpp::Chunk::Chunk(const void *buffer, std::size_t len) {
  if (buffer == nullptr) {
    throw std::runtime_error("Null buffer");
  }

  pimpl = std::make_unique<riffcpp::Chunk::impl>();

  const char *charbuf = static_cast<const char *>(buffer);

  pimpl->m_buf = std::make_shared<membuffer>(const_cast<char *>(charbuf), len);
  pimpl->m_stream = std::make_shared<std::istream>(pimpl->m_buf.get());
  pimpl->m_pos = pimpl->m_stream->tellg();
}

riffcpp::Chunk::Chunk(std::unique_ptr<riffcpp::Chunk::impl> &&impl) {
  pimpl = std::move(impl);
}

riffcpp::Chunk::~Chunk() = default;

riffcpp::FourCC riffcpp::Chunk::id() {
  pimpl->m_stream->seekg(pimpl->m_pos);
  riffcpp::FourCC read_id;
  pimpl->m_stream->read(read_id.data(), read_id.size());
  return read_id;
}

std::uint32_t riffcpp::Chunk::size() {
  std::streamoff offs{4};
  pimpl->m_stream->seekg(pimpl->m_pos + offs);
  uint32_t read_size;
  pimpl->m_stream->read(reinterpret_cast<char *>(&read_size), 4);
  return read_size;
}

riffcpp::FourCC riffcpp::Chunk::type() {
  std::streamoff offs{8};
  pimpl->m_stream->seekg(pimpl->m_pos + offs);
  riffcpp::FourCC read_type;
  pimpl->m_stream->read(read_type.data(), read_type.size());
  return read_type;
}

std::vector<char> riffcpp::Chunk::data() {
  std::streamoff offs{8};
  pimpl->m_stream->seekg(pimpl->m_pos + offs);

  std::uint32_t data_size = size();

  std::vector<char> read_data;
  read_data.resize(data_size);

  pimpl->m_stream->read(read_data.data(), data_size);
  return read_data;
}

iter riffcpp::Chunk::begin(bool no_chunk_id) {
  std::streamoff offs{no_chunk_id ? 8 : 12};
  auto it = std::make_unique<riffcpp::Chunk::iterator::impl>();
  it->m_pos = pimpl->m_pos + offs;
  it->m_stream = pimpl->m_stream;

  return iter{std::move(it)};
}

iter riffcpp::Chunk::end() {
  std::uint32_t sz = size();
  std::streamoff offs{sz + sz % 2 + 8};

  auto it = std::make_unique<riffcpp::Chunk::iterator::impl>();
  it->m_pos = pimpl->m_pos + offs;
  it->m_stream = pimpl->m_stream;

  return iter(std::move(it));
}

iter::iterator(std::unique_ptr<riffcpp::Chunk::iterator::impl> &&impl) {
  pimpl = std::move(impl);
}

bool iter::operator==(const iter &a) const {
  return pimpl->m_pos == a.pimpl->m_pos;
}
bool iter::operator!=(const iter &a) const { return !(*this == a); }

riffcpp::Chunk iter::operator*() const {
  auto im = std::make_unique<riffcpp::Chunk::impl>();
  im->m_stream = pimpl->m_stream;
  im->m_pos = pimpl->m_pos;

  return riffcpp::Chunk(std::move(im));
}

iter &iter::operator++() {
  auto im = std::make_unique<riffcpp::Chunk::impl>();
  im->m_stream = pimpl->m_stream;
  im->m_pos = pimpl->m_pos;

  riffcpp::Chunk chunk(std::move(im));
  std::uint32_t sz = chunk.size();
  std::streamoff offs{sz + sz % 2 + 8};

  pimpl->m_pos += offs;
  return *this;
}

iter iter::operator++(int) {
  auto im = std::make_unique<riffcpp::Chunk::iterator::impl>();
  im->m_stream = pimpl->m_stream;
  im->m_pos = pimpl->m_pos;

  this->operator++();
  return iter(std::move(im));
}

riffcpp::Chunk::iterator::~iterator() = default;

riffcpp::Chunk::iterator::iterator(const iter &it) {
  auto im = std::make_unique<riffcpp::Chunk::iterator::impl>();
  im->m_stream = it.pimpl->m_stream;
  im->m_pos = it.pimpl->m_pos;

  pimpl = std::move(im);
}
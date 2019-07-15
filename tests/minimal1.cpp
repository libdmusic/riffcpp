#include "common.hpp"

int main(int argc, char *argv[]) {
  riffcpp::Chunk ch("minimal.riff");
  TEST_ASSERT(ch.id() == riffcpp::riff_id, "Toplevel 'RIFF' id expected");
  TEST_ASSERT(ch.type() == smpl_id, "Expected 'smpl' chunk");
  std::vector<riffcpp::Chunk> chunks(ch.begin(), ch.end());
  TEST_ASSERT(chunks.size() == 1, "Toplevel chunk must contain one subchunk");
  TEST_ASSERT(chunks[0].id() == test_id, "Subchunk must have 'test' id");
  std::vector<char> buffer;
  buffer.resize(chunks[0].size());
  chunks[0].read_data(buffer.data(), buffer.size());
  TEST_ASSERT(buffer.size() == 1, "Subchunk must have data size 1");
  TEST_ASSERT((unsigned char)buffer[0] == 0xFF, "Subchunk must contain '0xFF'");
}
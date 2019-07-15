#include "common.hpp"

int main(int argc, char *argv[]) {
  std::ifstream stream("minimal_2.riff", std::ios::binary);
  riffcpp::Chunk ch("minimal_2.riff");
  std::vector<char> buffer;
  TEST_ASSERT(ch.id() == riffcpp::riff_id, "Toplevel 'RIFF' id expected");
  TEST_ASSERT(ch.type() == smpl_id, "Expected 'smpl' chunk");
  std::vector<riffcpp::Chunk> chunks(ch.begin(), ch.end());
  TEST_ASSERT(chunks.size() == 2, "Toplevel chunk must contain two subchunks");

  TEST_ASSERT(chunks[0].id() == tst1_id, "Subchunk must have 'tst1' id");
  buffer.resize(chunks[0].size());
  chunks[0].read_data(buffer.data(), buffer.size());
  TEST_ASSERT(buffer.size() == 1, "Subchunk must have data size 1");
  TEST_ASSERT((unsigned char)buffer[0] == 0xFF, "Subchunk must contain '0xFF'");

  TEST_ASSERT(chunks[1].id() == tst2_id, "Subchunk must have 'tst2' id");
  buffer.resize(chunks[1].size());
  chunks[1].read_data(buffer.data(), buffer.size());
  TEST_ASSERT(buffer.size() == 1, "Subchunk must have data size 1");
  TEST_ASSERT((unsigned char)buffer[0] == 0xEE, "Subchunk must contain '0xFF'");
}
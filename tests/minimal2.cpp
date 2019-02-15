#include "common.h"

int main(int argc, char *argv[]) {
  std::ifstream stream("minimal_2.riff", std::ios::binary);
  riffcpp::Chunk ch(stream, stream.tellg());
  TEST_ASSERT(ch.id() == riffcpp::riff_id, "Toplevel 'RIFF' id expected");
  TEST_ASSERT(ch.type() == smpl_id, "Expected 'smpl' chunk");
  std::vector<riffcpp::Chunk> chunks(ch.begin(), ch.end());
  TEST_ASSERT(chunks.size() == 2, "Toplevel chunk must contain two subchunks");

  TEST_ASSERT(chunks[0].id() == tst1_id, "Subchunk must have 'tst1' id");
  auto data = chunks[0].data();
  TEST_ASSERT(data.size() == 1, "Subchunk must have data size 1");
  TEST_ASSERT((unsigned char)data[0] == 0xFF, "Subchunk must contain '0xFF'");

  TEST_ASSERT(chunks[1].id() == tst2_id, "Subchunk must have 'tst2' id");
  data = chunks[1].data();
  TEST_ASSERT(data.size() == 1, "Subchunk must have data size 1");
  TEST_ASSERT((unsigned char)data[0] == 0xEE, "Subchunk must contain '0xFF'");
}
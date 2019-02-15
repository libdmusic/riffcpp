#include "common.h"

int main(int argc, char *argv[]) {
  std::ifstream stream("test_2.riff", std::ios::binary);
  std::string str1("hey this is a test"), str2("hey this is another test!"),
   str3("final test");

  riffcpp::Chunk ch(stream, stream.tellg());
  TEST_ASSERT(ch.id() == riffcpp::riff_id, "Toplevel 'RIFF' id expected");
  TEST_ASSERT(ch.type() == smpl_id, "Expected 'smpl' chunk");
  std::vector<riffcpp::Chunk> chunks{ch.begin(), ch.end()};
  TEST_ASSERT(chunks.size() == 2, "Toplevel chunk must contain two subchunks");

  TEST_ASSERT(chunks[0].id() == riffcpp::list_id,
              "Subchunk must have 'LIST' id");
  TEST_ASSERT(chunks[0].type() == tst1_id, "Subchunk must have 'tst1' type");
  std::vector<riffcpp::Chunk> subchunks_1{chunks[0].begin(), chunks[0].end()};
  TEST_ASSERT(subchunks_1.size() == 2, "Subchunk must have two subchunks");
  TEST_ASSERT(subchunks_1[0].id() == test_id, "Subchunk must have 'test' id");
  auto data = subchunks_1[0].data();
  TEST_ASSERT(std::string(data.data(), data.size()) == str1,
              "Strings must be equal");
  TEST_ASSERT(subchunks_1[1].id() == test_id, "Subchunk must have 'test' id");
  data = subchunks_1[1].data();
  TEST_ASSERT(std::string(data.data(), data.size()) == str2,
              "Strings must be equal");

  TEST_ASSERT(chunks[1].id() == seqt_id, "Subchunk must have 'seqt' id");
  std::vector<riffcpp::Chunk> subchunks_2{chunks[1].begin(true),
                                          chunks[1].end()};
  TEST_ASSERT(subchunks_2.size() == 1, "Subchunk must have one subchunk");
  TEST_ASSERT(subchunks_2[0].id() == test_id, "Subchunk must have 'test' id");
  data = subchunks_2[0].data();
  TEST_ASSERT(std::string(data.data(), data.size()) == str3,
              "Strings must be equal");
}
#include "common.hpp"

void test_chunk(riffcpp::Chunk &ch) {
  std::string str1("hey this is a test"), str2("hey this is another test!"),
   str3("final test");

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

int main(int argc, char *argv[]) {
  std::uint8_t test2_riff[] = {
   0x52, 0x49, 0x46, 0x46, 0x66, 0x00, 0x00, 0x00, 0x73, 0x6d, 0x70, 0x6c, 0x4c,
   0x49, 0x53, 0x54, 0x40, 0x00, 0x00, 0x00, 0x74, 0x73, 0x74, 0x31, 0x74, 0x65,
   0x73, 0x74, 0x12, 0x00, 0x00, 0x00, 0x68, 0x65, 0x79, 0x20, 0x74, 0x68, 0x69,
   0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x74, 0x65,
   0x73, 0x74, 0x19, 0x00, 0x00, 0x00, 0x68, 0x65, 0x79, 0x20, 0x74, 0x68, 0x69,
   0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x6e, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x20,
   0x74, 0x65, 0x73, 0x74, 0x21, 0x00, 0x73, 0x65, 0x71, 0x74, 0x12, 0x00, 0x00,
   0x00, 0x74, 0x65, 0x73, 0x74, 0x0a, 0x00, 0x00, 0x00, 0x66, 0x69, 0x6e, 0x61,
   0x6c, 0x20, 0x74, 0x65, 0x73, 0x74};
  std::size_t test2_riff_len = 110;

  riffcpp::Chunk ch1("test_2.riff");
  test_chunk(ch1);
  riffcpp::Chunk ch2(test2_riff, test2_riff_len);
  test_chunk(ch2);
}
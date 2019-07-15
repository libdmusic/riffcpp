#include "common.hpp"
#include <fstream>

void test_chunk(riffcpp::Chunk &ch) {
  std::string str1("hey this is a test"), str2("hey this is another test"),
   str3("final test");

  std::vector<char> buffer;

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
  buffer.resize(subchunks_1[0].size());
  subchunks_1[0].read_data(buffer.data(), buffer.size());
  TEST_ASSERT(std::string(buffer.data(), buffer.size()) == str1,
              "Strings must be equal");
  TEST_ASSERT(subchunks_1[1].id() == test_id, "Subchunk must have 'test' id");
  buffer.resize(subchunks_1[1].size());
  subchunks_1[1].read_data(buffer.data(), buffer.size());
  TEST_ASSERT(std::string(buffer.data(), buffer.size()) == str2,
              "Strings must be equal");

  TEST_ASSERT(chunks[1].id() == seqt_id, "Subchunk must have 'seqt' id");
  std::vector<riffcpp::Chunk> subchunks_2{chunks[1].begin(true),
                                          chunks[1].end()};
  TEST_ASSERT(subchunks_2.size() == 1, "Subchunk must have one subchunk");
  TEST_ASSERT(subchunks_2[0].id() == test_id, "Subchunk must have 'test' id");
  buffer.resize(subchunks_2[0].size());
  subchunks_2[0].read_data(buffer.data(), buffer.size());
  TEST_ASSERT(std::string(buffer.data(), buffer.size()) == str3,
              "Strings must be equal");

  // Test prefix and postfix increment operators
  auto first_it = ch.begin();
  auto first = *(first_it++);
  auto second = *first_it;

  TEST_ASSERT(first.id() == riffcpp::list_id, "Subchunk must have 'LIST' id");
  TEST_ASSERT(first.type() == tst1_id, "Subchunk must have 'LIST' id");
  TEST_ASSERT(second.id() == seqt_id, "Subchunk must have 'seqt' id");

  auto first_it2 = ch.begin();
  auto first2 = *first_it2;
  ++first_it2;
  auto second2 = *first_it2;

  TEST_ASSERT(first2.id() == riffcpp::list_id, "Subchunk must have 'LIST' id");
  TEST_ASSERT(first2.type() == tst1_id, "Subchunk must have 'LIST' id");
  TEST_ASSERT(second2.id() == seqt_id, "Subchunk must have 'seqt' id");
}

int main(int argc, char *argv[]) {
  std::uint8_t test_riff[] = {
   0x52, 0x49, 0x46, 0x46, 0x64, 0x00, 0x00, 0x00, 0x73, 0x6d, 0x70, 0x6c,
   0x4c, 0x49, 0x53, 0x54, 0x3e, 0x00, 0x00, 0x00, 0x74, 0x73, 0x74, 0x31,
   0x74, 0x65, 0x73, 0x74, 0x12, 0x00, 0x00, 0x00, 0x68, 0x65, 0x79, 0x20,
   0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65,
   0x73, 0x74, 0x74, 0x65, 0x73, 0x74, 0x18, 0x00, 0x00, 0x00, 0x68, 0x65,
   0x79, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x6e,
   0x6f, 0x74, 0x68, 0x65, 0x72, 0x20, 0x74, 0x65, 0x73, 0x74, 0x73, 0x65,
   0x71, 0x74, 0x12, 0x00, 0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x0a, 0x00,
   0x00, 0x00, 0x66, 0x69, 0x6e, 0x61, 0x6c, 0x20, 0x74, 0x65, 0x73, 0x74};
  std::size_t test_riff_len = 108;

  riffcpp::Chunk ch1("test.riff");
  test_chunk(ch1);
  riffcpp::Chunk ch2(test_riff, test_riff_len);
  test_chunk(ch2);
}
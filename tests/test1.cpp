#include <riffcpp/riffcpp.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>

#define TEST_ASSERT(expr, msg)                                                 \
  {                                                                            \
    if (!(expr)) {                                                             \
      std::cerr << "Assertion failed on line " << __LINE__ << ": " #expr "\n"  \
                << "Reason: " << (msg) << '\n';                                \
      std::exit(EXIT_FAILURE);                                                 \
    }                                                                          \
  }

constexpr riffcpp::FourCC seqt_id{'s', 'e', 'q', 't'};
constexpr riffcpp::FourCC smpl_id{'s', 'm', 'p', 'l'};
constexpr riffcpp::FourCC test_id{'t', 'e', 's', 't'};
constexpr riffcpp::FourCC tst1_id{'t', 's', 't', '1'};
constexpr riffcpp::FourCC tst2_id{'t', 's', 't', '2'};

void minimal_1() {
  std::ifstream stream("minimal.riff", std::ios::binary);
  riffcpp::Chunk ch(stream, stream.tellg());
  TEST_ASSERT(ch.id() == riffcpp::riff_id, "Toplevel 'RIFF' id expected");
  TEST_ASSERT(ch.type() == smpl_id, "Expected 'smpl' chunk");
  std::vector<riffcpp::Chunk> chunks(ch.begin(), ch.end());
  TEST_ASSERT(chunks.size() == 1, "Toplevel chunk must contain one subchunk");
  TEST_ASSERT(chunks[0].id() == test_id, "Subchunk must have 'test' id");
  auto data = chunks[0].data();
  TEST_ASSERT(data.size() == 1, "Subchunk must have data size 1");
  TEST_ASSERT((unsigned char)data[0] == 0xFF, "Subchunk must contain '0xFF'");
}

void minimal_2() {
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

void test_1() {
  std::ifstream stream("test.riff", std::ios::binary);
  std::string str1("hey this is a test"), str2("hey this is another test"),
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
  TEST_ASSERT(chunks[1].type() == tst2_id, "Subchunk must have 'tst2' type");
  std::vector<riffcpp::Chunk> subchunks_2{chunks[1].begin(), chunks[1].end()};
  TEST_ASSERT(subchunks_2.size() == 1, "Subchunk must have one subchunk");
  TEST_ASSERT(subchunks_2[0].id() == test_id, "Subchunk must have 'test' id");
  data = subchunks_2[0].data();
  TEST_ASSERT(std::string(data.data(), data.size()) == str3,
              "Strings must be equal");
}

void test_2() {
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
  TEST_ASSERT(chunks[1].type() == tst2_id, "Subchunk must have 'tst2' type");
  std::vector<riffcpp::Chunk> subchunks_2{chunks[1].begin(), chunks[1].end()};
  TEST_ASSERT(subchunks_2.size() == 1, "Subchunk must have one subchunk");
  TEST_ASSERT(subchunks_2[0].id() == test_id, "Subchunk must have 'test' id");
  data = subchunks_2[0].data();
  TEST_ASSERT(std::string(data.data(), data.size()) == str3,
              "Strings must be equal");
}

int main(int argc, char *argv[]) {
  std::cout << "test1 started\n";
  minimal_1();
  minimal_2();

  test_1();
  test_2();
  std::cout << "test1 ended\n";
  return 0;
}
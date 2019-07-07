#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <riffcpp.hpp>

void print_hex_dump(std::vector<char> &data, int indent) {
  int i = 0;
  for (char c : data) {
    if (i % 16 == 0) {
      for (int j = 0; j < indent; j++) {
        std::cout << "  ";
      }
    }
    std::cout << std::setfill('0') << std::setw(2) << std::hex
              << (int)((unsigned char)c) << ' ';
    if (i % 16 == 15) {
      std::cout << "  ";
      for (int j = 0; j < 16; j++) {
        char x = data[i - 15 + j];
        if (x >= 0x20 && x <= 0x7E) {
          std::cout << x;
        } else {
          std::cout << '.';
        }
      }
      std::cout << '\n';
    }
    i++;
  }
  if (i % 16 != 0) {
    for (int k = i % 16; k < 16; k++) {
      std::cout << "-- ";
    }
    std::cout << "  ";
    for (int j = 0; j < i % 16; j++) {
      char x = data[i - (i % 16) + j];
      if (x >= 0x20 && x <= 0x7E) {
        std::cout << x;
      } else {
        std::cout << '.';
      }
    }
  }
  std::cout << std::dec << '\n';
}

void print_chunks(riffcpp::Chunk &ch, int offs = 0) {
  auto id = ch.id();
  auto size = ch.size();
  auto data = ch.data();
  if (id == riffcpp::riff_id || id == riffcpp::list_id) {
    for (int i = 0; i < offs; i++) {
      std::cout << "  ";
    }
    auto type = ch.type();
    std::cout << std::string(id.data(), 4) << " " << std::string(type.data(), 4)
              << " size: " << size << "\n";
    for (auto ck : ch) {
      print_chunks(ck, offs + 1);
    }
  } else {
    for (int i = 0; i < offs; i++) {
      std::cout << "  ";
    }
    std::cout << std::string(id.data(), 4) << " size: " << size << "\n";
    print_hex_dump(data, offs + 1);
  }
}

int main(int argc, char *argv[]) {
  riffcpp::Chunk ch(argv[1]);
  print_chunks(ch);
  return 0;
}
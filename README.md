# riffcpp

[![Appveyor status](https://ci.appveyor.com/api/projects/status/github/frabert/riffcpp)](https://ci.appveyor.com/project/frabert/riffcpp)

Simple library for reading RIFF files

The data is read lazily, so in cases where seeking is cheaper than reading and
large chunks of data can be skipped, this results in fast parsing, and also saves
memory usage.

## Usage

The following example program prints the structure of the RIFF file, and provides
an hex dump of unknown chunks:
```c++
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <riffcpp/riffcpp.hpp>

void print_indent(int indent) {
  for (int j = 0; j < indent; j++) {
    std::cout << "  ";
  }
}

void print_hex_dump(std::vector<char> &data, int indent) {
  int i = 0;
  for (char c : data) {
    if (i % 16 == 0) {
      print_indent(indent);
    }
    std::cout << std::setfill('0') << std::setw(2) << std::hex
              << (int)((unsigned char)c) << ' ';
    if (i % 16 == 15) {
      std::cout << '\n';
    }
    i++;
  }
  if (i % 16 != 0) {
    i = i % 16;
    for (; i < 16; i++) {
      std::cout << "-- ";
    }
  }
  std::cout << std::dec << '\n';
}

void print_chunks(riffcpp::Chunk &ch, int offs = 0) {
  auto id = ch.id();     // Reads the chunk's id
  auto size = ch.size(); // Reads the chunk's size
  if (id == riffcpp::riff_id || id == riffcpp::list_id) {
    // The chunk is either a 'RIFF' or a 'LIST', so it contains subchunks
    print_indent(offs);
    auto type = ch.type(); // Reads the chunk's type
    std::cout << std::string(id.data(), 4) << " " << std::string(type.data(), 4)
              << " size: " << size << "\n";

    // Iterate subchunks
    for (auto ck : ch) {
      print_chunks(ck, offs + 1);
    }
  } else {
    // The chunk is an unknown type, provide an hexdump
    auto data = ch.data();
    print_indent(offs);
    std::cout << std::string(id.data(), 4) << " size: " << size << "\n";
    print_hex_dump(data, offs + 1);
  }
}

int main(int argc, char *argv[]) {
  std::ifstream stream(argv[1], std::ios::binary);

  // Read the chunk from the current position
  riffcpp::Chunk ch(stream, stream.tellg());
  print_chunks(ch);
}
```

## Building and installing

After downloading the sources (either by `git clone` or archive), run the following commands:
```
mkdir build_riffcpp
cd build_riffcpp
cmake path/to/riffcpp
cmake --build . --target INSTALL
```
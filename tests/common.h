#ifndef COMMON_H
#define COMMON_H

#include <riffcpp.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>

constexpr riffcpp::FourCC seqt_id{'s', 'e', 'q', 't'};
constexpr riffcpp::FourCC smpl_id{'s', 'm', 'p', 'l'};
constexpr riffcpp::FourCC test_id{'t', 'e', 's', 't'};
constexpr riffcpp::FourCC tst1_id{'t', 's', 't', '1'};
constexpr riffcpp::FourCC tst2_id{'t', 's', 't', '2'};

#define TEST_ASSERT(expr, msg)                                                 \
  {                                                                            \
    if (!(expr)) {                                                             \
      std::cerr << "Assertion failed on line " << __LINE__ << ": " #expr "\n"  \
                << "Reason: " << (msg) << '\n';                                \
      std::exit(EXIT_FAILURE);                                                 \
    } else {                                                                   \
      std::cout << "" #expr " (" << __LINE__ << "): OK\n";                     \
    }                                                                          \
  }

#endif
#ifndef STRING_HANDLING_HPP
#define STRING_HANDLING_HPP

#include <string>

#include "basic-memory.hpp"

// Section 38

constexpr halfword str_ptr = max_halfword; // In TeX, this is the next entry in the string table

// Section 41
constexpr int length(int n) {
  std::string* s = reinterpret_cast<std::string*>(&n);
  return int(s->size());
}

constexpr int cur_length() { return 1'000; } // Arbitrary length of current string

#endif

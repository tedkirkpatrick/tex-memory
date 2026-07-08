#ifndef STRING_HANDLING_HPP
#define STRING_HANDLING_HPP

#include <vector>
#include <string>

#include "basic-memory.hpp"

class string_table {
public:
  string_table();

  std::vector<std::string*> strings;
};

extern string_table str_start;

// Following variables and routines are not from TeX

constexpr std::string get_str(int s) {
  return *str_start.strings[s];
}

// Section 38

constexpr halfword str_ptr = max_halfword; // In TeX, this is the next entry in the string table

// Section 41
constexpr int length(int n) {
  return int(str_start.strings[n]->size());
}

constexpr int cur_length() {
  return str_start.strings.back()->size();
}

#endif

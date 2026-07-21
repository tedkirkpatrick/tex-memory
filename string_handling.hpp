#ifndef STRING_HANDLING_HPP
#define STRING_HANDLING_HPP

#include <vector>
#include <string>

#include "basic_memory.hpp"


// Section 38

using str_number = int;


// C++ equivalent to TeX's string table

class string_table {
public:
  string_table();
  void reset_strings();
  str_number first_avail() { return str_number(init_array_size); }

  std::vector<std::string*> strings;

private:
  std::vector<std::string*>::size_type init_array_size;
};

extern string_table str_start;

// Following variables and routines are not from TeX

constexpr std::string get_str(int s) {
  return *str_start.strings[s];
}

// Not present in TeX (which computes this value via WEB's string-to-str_number conversion)
extern str_number mu_string;

// Section 39

constexpr halfword str_ptr = max_halfword; // In TeX, this is the next entry in the string table

// Section 41
constexpr int length(int n) {
  return int(str_start.strings[n]->size());
}

constexpr int cur_length() {
  return str_start.strings.back()->size();
}

constexpr void append_char(char c) {
  str_start.strings.back()->push_back(c);
}

constexpr void flush_char() { str_start.strings.back()->pop_back();}

// Section 42

[[nodiscard]] extern int make_string();

#endif

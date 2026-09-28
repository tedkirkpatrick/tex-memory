#ifndef STRING_HANDLING_HPP
#define STRING_HANDLING_HPP
/*
  A fresh implementation of string handling
  Part 38 of Tex: The Program

  This code handles strings differently from TeX. TeX does not use any Pascal string-handling
  features but instead uses a custom string pool.

  This code uses C++ std::strings and C-style null-delimited string constants. Any TeX
  code with an explicit string constant is written as a C-style double-quote-delimited string
  (or a single-quote-delimited character constant for single-character instances). Strings
  defined at runtime---typically csnames---are allocated as dynamic instances of std::string.
  TeX never deletes a string, so we use std::string* pointers freely, passing them by
  value and never deleting them. There is no need to refer to them via std::unique_ptr
  or std::shared_ptr.

  For compatibility with TeX routines, we refer to dynamically-allocated strings via
  a table of pointers.

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines are NOT transliterations of TeX code. They are purpose-written
  for this demonstration. They are written in modern C++.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

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

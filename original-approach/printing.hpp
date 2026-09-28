#ifndef PRINTING_HPP
#define PRINTING_HPP
/*
  Substitute routines for Part 5, "On-line and off-line printing"

  These routines have a different purpose, as this code is only going
  to be used for demonstration and testing purposes. So I don't mimic
  the full complexity of the routines in TeX Part 5.

  Furthermore, this code only runs on ASCII-based machines, so I do
  not include translation to alternative external character sets.

  The routines write to a stream, typically std::cout or a streambuf.

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines are NOT transliterations of TeX code. They are purpose-written
  for this demonstration. They are written in modern C++.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <sstream>
#include <string_view>

#include "string_handling.hpp"

// The following variables and routines and not from TeX

extern void set_cout();

extern void set_str(std::ostringstream* ostr);

// Section 54

extern int tally;

// Section 57

extern void print_ln();

// Section 58

extern void print_char(char c);

// Section 59

extern void print(int sn);
extern void print(const char* s);

// Section 62

extern void print_nl(int sn);
extern void print_nl(const char* s);

// Section 63

extern void print_esc(std::string_view s);

// Section 65

extern void print_int(int n);

// Section 67

extern void print_hex(int n);

// Section 68

extern void print_ASCII(int c);

// Section 70

constexpr void print_current_string() { print(str_number(str_start.strings.size() - 1)); }

#endif

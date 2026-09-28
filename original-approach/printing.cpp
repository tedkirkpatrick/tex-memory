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


#include "printing.hpp"

#include <format>
#include <iostream>

#include "eqtb.hpp"
#include "string_handling.hpp"

// The following variables and routines and not from TeX

static std::ostream* ostream;

void set_cout() {
  ostream = & std::cout;
}

void set_str(std::ostringstream* ostr) {
  ostream = ostr;
}

// Section 54

int tally = 0; // Initialized in Section 55

// Section 57

void print_ln() {
  *ostream << '\n';
}

// Section 58

void print_char(char c) {
  *ostream << c;
}

// Section 59

void print(int sn) {
  *ostream << get_str(sn);
}

// A simple overload to handle C-style strings
void print(const char* s) {
  *ostream << s;
}

// Section 62

// The actual TeX code only displays a newline if the current position is not at line start.
// This implementation always displays a newline, accepting the occasional extra blank line.
void print_nl(int sn) {
  *ostream << '\n';
  print(sn);
}

// A simple overload to handle C-style strings
void print_nl(const char* s) {
  *ostream << '\n';
  print(s);
}

// Section 63

void print_esc(std::string_view s) {
  *ostream << (unsigned char) (escape_char());
  *ostream << s;
}

// Section 65

void print_int(int n) {
  *ostream << n;
}

// Section 67

void print_hex(int n) {
  *ostream << std::hex << n << std::dec;
}
  
// Section 68

void print_ASCII(int c) {
  if (c >= 0 && c <= 127)
    print(c);
  else {
    print_char('[');
    if (c < 0)
      print_int(c);
    else
      print_hex(c);
    print_char(']');
  }

}

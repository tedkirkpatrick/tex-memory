#include "print.hpp"

#include <iostream>
#include <string>

#include "eqtb.hpp"

// Section 54

int tally;

// Section 59

// In TeX, s is an index to a string table. We instead use a pointer to a std::string
void print(int sn) {
  std::string* s = reinterpret_cast<std::string*>(&sn);
  std::cout << *s;
}

// A simple overload to handle C-style strings
void print(const char* s) {
  std::cout << s;
}

// Section 63

void print_esc(std::string_view s) {
  std::cout << (unsigned char) (escape_char());
  std::cout << s;
}

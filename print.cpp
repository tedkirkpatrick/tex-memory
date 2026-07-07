#include "print.hpp"

#include <iostream>

#include "eqtb.hpp"

// Section 54

int tally;

// Section 63

void print_esc(std::string_view s) {
  std::cout << (unsigned char) (escape_char());
  std::cout << s;
}

#include "eqtb.hpp"

#include "printing.hpp"

// Section 225

void print_skip_param(int n) {
  print_esc("skip of type ");
  print_int(n);
}

// Section 253

memory_word eqtb[eqtb_size + 1]; // Pascal uses inclusive final index but C++ uses exclusive

#include "eqtb.hpp"

#include "printing.hpp"

void init_eqtb() {
  // By default, entries are 0
  for (int k = 0; k <= eqtb_size; k++)
    eqtb[k].intv = 0;
  // In Section 240, these next two are initialized to 0.
  // They are reset before debugging displays, such as in Section 1139.
  // We pick big values for our purposes.
  show_box_depth() = max_halfword;
  show_box_breadth() = 1'000;
  // Following from Section 240
  escape_char() = '\\';
}

// Section 225

void print_skip_param(int n) {
  print_esc("skip of type ");
  print_int(n);
}

// Section 253

memory_word eqtb[eqtb_size + 1]; // Pascal uses inclusive final index but C++ uses exclusive

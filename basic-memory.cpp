/*
  Basic memory routines in the style of TeX.
  Parts 8--9 of TeX: The Program

  Initialization of static variables is done via routines from Section 164, rather
  than using C++-style static initializers.

  In the original Pascal, most of these variables and routines have global scope, so
  we declare them extern in the header.

  We do not define any static entries as this is just testing the dynamic memory.
*/

#include "basic-memory.hpp"

#include <iostream>
#include <stdexcept>
#include <string_view>


// Section 115

pointer temp_ptr;

// Section 116

memory_word mem [mem_max+1]; // Pascal uses inclusive upper bound but C/C++ use exclusive upper bound
pointer lo_mem_max;
pointer hi_mem_min;

// Section 118

pointer avail;
pointer mem_end;

void overflow(std::string_view s, int n) {
  std::cout << "TeX capacity exceeded, Sorry [" << s;
  std::cout << '=' << n << "]\n";
  std::cout << std::flush;
  throw std::runtime_error("Exiting");
}

// Section 120

pointer get_avail() {
  pointer p;
  p = avail;
  if (p != null)
    avail = link(avail);
  else if (mem_end < mem_max) {
    incr(mem_end);
    p = mem_end;
  }
  else {
    decr(hi_mem_min);
    p = hi_mem_min;
    if (hi_mem_min <= lo_mem_max) {
      overflow("main memory size", mem_max + 1 - mem_min);
    }
  }
  link(p) = null;
  return p;
}

// Section 123

void flush_list(pointer& p) {
  pointer q, r;

  if (p != null) {
    r = p;
    do {
      q = r;
      r = link(r);
    }
    while (r != null);
    link(q) = avail;
    avail = p;
  }
}

// Section 124

pointer rover;

// Section 162

// For now, no statically-allocated values
pointer low_mem_stat_max = -1;
pointer hi_mem_stat_min = mem_top;


// Section 164

void initialize_the_special_list_heads_and_constant_nodes_790();

void init_table_entries() {
  pointer k;
  #if 0
  for (k = mem_bot + 1; k <= low_mem_stat_max; k++)
    mem[k].sc = 0.0;
  k = mem_bot;
  while (k < lo_mem_stat_max) {
    glue_ref_count(k) = null + 1;
    stretch_order(k) = normal;
    shrink_order(k) normal;
    k = k + glue_spec_size;
  }
  // ... stretch stuff ignored for now ...
  #endif
  rover = low_mem_stat_max + 1;
  link(rover) = empty_flag;
  node_size(rover) = 1000;
  llink(rover) = rover;
  rlink(rover) = rover;
  for (k = hi_mem_stat_min; k <= mem_top; k++) {
    mem[k] = mem[lo_mem_max];
  }
  initialize_the_special_list_heads_and_constant_nodes_790();
  avail = null;
  mem_end = mem_top;
  hi_mem_min = hi_mem_stat_min;
  
}

// Section 790

// None of these exist in our basic memory system, so this routine is empty
void initialize_the_special_list_heads_and_constant_nodes_790() {
  return;
}

// -------

void test_avail() {
  pointer p;
  fast_get_avail(p);
  std::cout << p << ' ' << link(p) << ' ' << mem_end << '\n';
  pointer p2;
  fast_get_avail(p2);
  link(p) = p2;
  std::cout << p2 << ' ' << link(p2) << ' ' << avail << ' ' << mem_end << '\n';
  
  flush_list(p);
  std::cout << p << ' ' << avail << '\n';

  fast_get_avail(p);
  std::cout << "After pull from avail " << p << " avail " << avail << '\n';
  free_avail(p);
  std::cout << "After free avail " << p << " avail " << avail << '\n';

  if (mem_max < 50) {
    std::cout << "Testing to exhaustion\n";
    pointer r = null;
    for (int i = 0; i <= mem_max; i++) {
      fast_get_avail(p);
      link(p) = r;
      r = p;
    }
  }
}

int main(int argc, char* argv[]) {
  std::cout << sizeof(memory_word) << '\n';

  init_table_entries();
  test_avail();
}

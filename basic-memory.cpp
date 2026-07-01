/*
  Basic memory routines in the style of TeX.
  Parts 8--9 of TeX: The Program

  Initialization of static variables is done via routines from Section 164, rather
  than using C++-style static initializers.

  In the original Pascal, most of these variables and routines have global scope, so
  we declare them extern in the header.

  We do not define any static nodes or single-word values as this is just testing the dynamic memory.

  We do not include any TeX init, debug, or stats code. We DO add some routines not defined in TeX
  that assist in testing and debugging.

  Although TeX places all the variables used in these routines in global-variables-13 scope,
  we instead make variables for the internal state of dynamic memory local to this file via 'static'
  and only declare variables 'extern' if they are used by macros in the .hpp file.
*/

#include "basic-memory.hpp"

#include <print>

#include "overflow.hpp"

using std::print, std::println;


// Section 115

static pointer temp_ptr;

// Section 116

memory_word mem [mem_max+1]; // Pascal uses inclusive upper bound but C/C++ use exclusive upper bound
static pointer lo_mem_max;
static pointer hi_mem_min;

// Section 118

pointer avail;
static pointer mem_end;

// Section 120

[[nodiscard]] pointer get_avail() {
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

void flush_list(pointer p) {
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

static pointer rover; // Declared global-13 in TeX but just local to dynamic memory routines

// Section 125

// The TeX source code just repeats this constant but we want to be able to export it for testing
static const halfword node_increment = 1000;

// Kunuth breaks this complex routine into multiple separate WEB sections.
// This implementation inserts those paragraphs directly into the routine.
[[nodiscard]] pointer get_node(int s) {
  println("\n\nEntering get_node({})", s);
  pointer p;
  pointer q;
  int r;
  int t;

 restart:
  p = rover;
  do {
    // Begin Section 127 "Try to allocate within node p and its physical successors and go to found if allocation was possible"
    q = p + node_size(p);
    println("Section 127 q {} p {}", q, p);
    while (is_empty(q)) {
      t = rlink(q);
      if (q == rover)
        rover = t;
      llink(t) = llink(q);
      rlink(llink(q)) = t;
      q = q + node_size(q);
    }
    r = q - s;
    println("Start of free node p {}, size {}, possible start of allocation within it {}", p, node_size(p), r);
    if (r > p + 1) {
      // Begin Section 128 "Allocate from the top of node p and goto found"
      node_size(p) = r - p;
      rover = p;
      goto found;
      // End Section 128
    }
    if (r == p) {
      println("r ({}) == p, rover {}, llink(p) {}, rlink(p) {}", r, rover, llink(p), rlink(p));
      if ((rlink(p) != rover) || (llink(p) != rover)) {
        // Begin Section 129 "Here we delete node p from the ring and let rover rove around"
        println("Begin Section 129 r {} p {}", r, p);
        rover = rlink(p);
        t = llink(p);
        llink(rover) = t;
        rlink(t) = rover;
        goto found;
        // End Section 129
      }
    }
    node_size(p) = q - p;
    // End Section 127
    p = rlink(p);
  }
  while (p != rover);
  if (s == merge_only)
    return max_halfword;
  println("Could not find space in free, extend memory: lo_mem_max+2 {} hi_mem_min {} mem_bot {} max_halfword {}", lo_mem_max+2, hi_mem_min, mem_bot, max_halfword);
  if (lo_mem_max + 2 < hi_mem_min)
    if (lo_mem_max + 2 <= mem_bot + max_halfword) {
      // Begin Section 126 "Grow more variable-size memory and goto restart"
      println("Beginning Section 126");
      if (lo_mem_max + node_increment < hi_mem_min) {
        t = lo_mem_max + node_increment;
        println("lo_mem_max {} + node_increment {} < hi_mem_min {}, t == {}", lo_mem_max, node_increment, hi_mem_min, t);
      }
      else {
        t = (lo_mem_max + hi_mem_min + 2) / 2;
        println("(lo_mem_max {} +  hi_mem_min {}) / 2 == {}", lo_mem_max, hi_mem_min, t);
      }
      println("t {}", t);
      p = llink(rover);
      q = lo_mem_max;
      rlink(p) = q;
      llink(rover) = q;
      if (t > mem_bot + max_halfword)
        t = mem_bot + max_halfword;
      rlink(q) = rover;
      llink(q) = p;
      link(q) = empty_flag;
      node_size(q) = t - lo_mem_max;
      lo_mem_max = t;
      link(lo_mem_max) = null;
      info(lo_mem_max) = null;
      rover = q;
      println("Returning to restart rover {} lo_mem_max {}", rover, lo_mem_max);
      goto restart;
      // End Section 126
    }
  overflow("main memory size", mem_max + 1 - mem_min);
 found:
  link(r) = null;
  return r;
    
}

// Section 130

void free_node(pointer p, halfword s) {
  pointer q;

  node_size(p) = s;
  link(p) = empty_flag;
  q = llink(rover);
  llink(p) = q;
  rlink(p) = rover;
  llink(rover) = p;
  rlink(q) = p;
}

// Sections 131--132 not required

// These sections are specific to INITEX, which we are not testing

// Section 162

// For now, no statically-allocated values
static pointer low_mem_stat_max = -1;
static pointer hi_mem_stat_min = mem_top;


// Section 164

static void initialize_the_special_list_heads_and_constant_nodes_790();

// Not a separate routine in TeX but makes sense to make it one for this implementation
void init_table_entries() {
  pointer k;
  // ... Glue code not included ...
  rover = low_mem_stat_max + 1;
  link(rover) = empty_flag;
  node_size(rover) = node_increment;
  llink(rover) = rover;
  rlink(rover) = rover;
  lo_mem_max = rover + node_increment;
  link(lo_mem_max) = null;
  info(lo_mem_max) = null;
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
static void initialize_the_special_list_heads_and_constant_nodes_790() {
  return;
}

// Not part of original TeX

namespace dynmemdbg {

  // A more modern approach would return a std::tuple but updating the parameters is closer
  // to the old-school approach of the original TeX code.
  void expose_avail_vars(memory_word*& mem_parm, pointer& avail_parm, pointer& mem_end_parm, pointer& hi_mem_min_parm) {
    mem_parm = mem;
    avail_parm = avail;
    mem_end_parm = mem_end;
    hi_mem_min_parm = hi_mem_min;
  }

  void expose_node_vars(pointer& rover_parm, pointer& lo_mem_max_parm, pointer& hi_mem_min_parm, halfword& node_increment_parm) {
    rover_parm = rover;
    lo_mem_max_parm = lo_mem_max;
    hi_mem_min_parm = hi_mem_min;
    node_increment_parm = node_increment;
  }

  void dump_free_list() {
    pointer p = rover;
    println("-- Free list --");
    do {
      if (p == rover)
        println("Rover node at {} (size {}, llink {}, rlink {}, free {})", p, node_size(p), llink(p), rlink(p), is_empty(p));
      else
        println("Free node at {} (size {}, llink {}, rlink {}, free {})", p, node_size(p), llink(p), rlink(p), is_empty(p));
      p  = rlink(p);
    }
    while (p != rover);
  }

}

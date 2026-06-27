// Basic definitions for Knuth-style memory
/*
  Basic memory routines in the style of TeX.
  Parts 8--9 of TeX: The Program

  See the corresponding .cpp file for details.

  This header has three purposes:
  1. Initialize a few Pascal constants from Section 11.
  2. Implement the WEB constants and macros in corresponding C++ idioms. By declaring
     these constexpr and incorporating the routines in this header they are declared inline.
  3. Give the constants and routines from the .cpp file global scope.
*/

#include <cstddef>

// Section 11
constexpr int mem_max = 30'000;
//constexpr int mem_max = 5; // For testing memory exhaustion
constexpr int mem_min = 0;
constexpr int buf_size = 500;
constexpr int stack_size = 200;

// Section 12
constexpr int mem_bot = 0;
constexpr int mem_top = mem_max; // For our purposes we make these equivalent

// Section 16
constexpr void incr(short int& v) { v++; }
constexpr void decr(short int& v) { v--; }

// Section 109
using glue_ratio = float;

// Section 110
constexpr std::byte min_quarterword {0};
constexpr std::byte max_quarterword {255};

constexpr short int min_halfword = 0;
constexpr short int max_halfword = 65'535;

// Section 112
consteval std::byte qi(std::byte v) { return v; } // Because min_quarterword == 0
consteval std::byte qo(std::byte v) { return v; } // Because min_quarterword == 0

consteval short int hi(short int v) { return v; } // Because min_halfword == 0
consteval short int ho(short int v) { return v; } // Because min_halfword == 0

// Section 113
using sc = int;
using quarterword = std::byte;
using halfword = short int;

struct two_halves {
  halfword rh;
  union {
    halfword lh;
    struct {
      quarterword b0;
      quarterword b1;
    };
  };
};

struct four_quarters {
  quarterword b0;
  quarterword b1;
  quarterword b2;
  quarterword b3;
};

union memory_word {
  int intv;
  glue_ratio gr;
  two_halves hh;
  four_quarters qqqq;
};

// Section 115

using pointer = halfword;
constexpr halfword null = min_halfword;

// Section 116 (type only)

extern memory_word mem[];

// Section 118

extern pointer avail;

constexpr pointer& link(pointer p) {
  return mem[p].hh.rh;
}

constexpr halfword& info(pointer p) {
  return mem[p].hh.lh;
}

// Section 120

pointer get_avail();

// Section 121

constexpr void free_avail(pointer p) {
  link(p) = avail;
  avail = p;
}

// Section 122

constexpr void fast_get_avail(pointer& p) {
  p = avail;
  if (p == null)
    p = get_avail();
  else {
    avail = link(p);
    link(p) = null;
  }
}

// Section 124

constexpr halfword empty_flag = max_halfword;
constexpr bool is_empty(pointer p) {
  return link(p) == empty_flag;
}

// Repeats info (Knuth just does direct macro equivalence)
constexpr halfword& node_size(pointer p) {
  return mem[p].hh.lh;
}

constexpr pointer& llink(pointer p) {
  return info(p + 1);
}

constexpr pointer& rlink(pointer p) {
  return link(p + 1);
}

extern pointer rover;

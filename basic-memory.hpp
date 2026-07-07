// Basic definitions for Knuth-style memory
/*
  Basic memory routines in the style of TeX.
  Parts 8--9 of TeX: The Program

  See the corresponding .cpp file for details.

  This header has three purposes:
  1. Initialize a few global Pascal constants from Section 11.
  2. Implement the WEB constants and macros in corresponding C++ idioms. By declaring
     these constexpr and incorporating the routines in this header they are declared inline.
  3. Set global scope for the constants and routines exported from the .cpp file.

  STYLE: See basic-memory.cpp comments for rationale for code style.
*/

#ifndef basic_memory_hpp
#define basic_memory_hpp

#include <cstddef>

// Section 11
constexpr int mem_max = 30'000;
constexpr int mem_min = 0;
constexpr int buf_size = 500;
constexpr int max_print_line = 79;
constexpr int stack_size = 200;
constexpr int font_max = 75;

// Section 12
constexpr int mem_bot = 0;
constexpr int mem_top = mem_max; // For our purposes we make these equivalent
constexpr int font_base = 0;
constexpr int hash_size = 2100;

// Section 109
using glue_ratio = float; // Moved this first so we can use the type in the next expressions
constexpr void set_glue_ratio_zero(glue_ratio& gr) { gr = glue_ratio(0.0f); }
constexpr void set_glue_ratio_one(glue_ratio& gr) { gr = glue_ratio(1.0f); }
// float() is part of the C++ type system so does not need to be defined
constexpr glue_ratio& unfloat(float& r) { return r; }
constexpr float float_constant(int v) { return float(v); }

// Section 110
constexpr std::byte min_quarterword {0};
constexpr std::byte max_quarterword {255};

// Section 112
consteval std::byte qi(std::byte v) { return v; } // Because min_quarterword == 0
consteval std::byte qo(std::byte v) { return v; } // Because min_quarterword == 0

consteval short int hi(short int v) { return v; } // Because min_halfword == 0
consteval short int ho(short int v) { return v; } // Because min_halfword == 0

// Section 113
using sc = int;
using quarterword = std::byte;
using halfword = short unsigned int;

// Next two defined in Section 16 in TeX but I want to use the type alias 'halfword'
constexpr void incr(halfword& v) { v++; }
constexpr void decr(halfword& v) { v--; }

// Next two defined in Section 110 in TeX but I want to use the type alias 'halfword'
constexpr halfword min_halfword = 0;
constexpr halfword max_halfword = 65'535;

struct quarterwords {
  quarterword b0;
  quarterword b1;
};

struct two_halves {
  halfword rh;
  union {
    halfword lh;
    struct quarterwords qw;
  };
};

struct four_quarters {
  quarterword b0;
  quarterword b1;
  quarterword b2;
  quarterword b3;
};

union memory_word {
  // TeX uses a WEB macro to make the string 'scv' equivalent to 'intv'. The equivalent
  // in C++ without using the preprocessor is to define a union.
  union {
    int intv;
    sc scv;
  };
  glue_ratio gr;
  two_halves hh;
  four_quarters qqqq;
};

// Section 115

using pointer = halfword;
constexpr halfword null = min_halfword;

extern pointer temp_ptr;

// Section 116 (type only)

extern memory_word mem[];
extern pointer hi_mem_min;

// Section 118

constexpr pointer& link(pointer p) {
  return mem[p].hh.rh;
}

constexpr halfword& info(pointer p) {
  return mem[p].hh.lh;
}

extern pointer avail;
extern pointer mem_end;

// Section 120

[[nodiscard]] extern pointer get_avail();

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

// Section 123

extern void flush_list(pointer p);

// Section 124

constexpr halfword empty_flag = max_halfword;
constexpr bool is_empty(pointer p) {
  return link(p) == empty_flag;
}

constexpr halfword& node_size(pointer p) {
  return info(p);
}

constexpr pointer& llink(pointer p) {
  return info(p + 1);
}

constexpr pointer& rlink(pointer p) {
  return link(p + 1);
}

// Section 125

// Knuth does not define this constant but I find it makes the code more readable
constexpr int merge_only = 0x40'00'00'00; // Size argument to get_node() requesting only merge frees

[[nodiscard]] extern pointer get_node(int s);

// Section 130

extern void free_node(pointer p, halfword s);

// Section 162

constexpr halfword zero_glue {mem_bot};

// Section 164

extern void init_table_entries();

// Non-TeX debugging

namespace dynmemdbg {
/*
  Functions in this namespace should only be called by testing and debugging code.
 */

  // Expose the key variables of the dynamic memory routines for debugging
  extern void expose_avail_vars(memory_word*& mem_parm, pointer& avail_parm, pointer& mem_end_parm, pointer& hi_mem_min_parm);
  extern void expose_node_vars(pointer& rover_parm, pointer& lo_mem_max_parm, pointer& hi_mem_min_parm, halfword& node_increment_parm);
  extern void dump_free_list();
}
#endif

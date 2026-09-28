/*
  Selected entries for the equivalence table
  Part 17 of TeX: The Program

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines adopt TeX's Pascal style as closely as possible in C++. The primary differnce
  from TeX is that WEB macros are all implemented via inline constexpr constants or functions,
  which retain type safety, rather than C-style preprocessor macros.

  As a result, this code does not conform to typical C++ best practices. For example,
  namespaces are only used for testing and debugging functions, variables are declared
  without initialization, and other good practices are not followed.

  Function subsections that in WEB are separated out are here embedded in the containing function.

  COPYRIGHT
  The code in this file is a transliteration of the original Pascal TeX routines into a C-style C++.
  The file tex.web containing the original code features the following notice:

      This program is copyright (C) 1982 by D. E. Knuth; all rights are reserved.
      Unlimited copying and redistribution of this file are permitted as long
      as this file is not modified. Modifications are permitted, but only if
      the resulting file is not named tex.web

   This modification of the original file is public domain.
   See See https://creativecommons.org/publicdomain/zero/1.0/

   The copyright of the original versions remains.
 */

#ifndef EQTB_HPP
#define EQTB_HPP

#include <cstddef>

#include "basic_memory.hpp"

// This routine peforms multiple functions from Part 17

extern void init_eqtb();

// Section 221

extern memory_word eqtb[]; // Declared in Section 253 but required for earlier definitions

constexpr halfword& equiv_field(memory_word& m) { return m.hh.rh; }
constexpr halfword& equiv(halfword h)  { return equiv_field(eqtb[h]); }

// Section 222

constexpr halfword active_base = 1;
constexpr halfword single_base = active_base + 128;
constexpr halfword null_cs = single_base + 128;
constexpr halfword hash_base = null_cs + 1;
constexpr halfword frozen_control_sequence = hash_base + hash_size;
constexpr halfword frozen_null_font = frozen_control_sequence + 10;
constexpr halfword undefined_control_sequence = frozen_null_font + 257;
constexpr halfword glue_base = undefined_control_sequence + 1;

// Section 224

constexpr std::byte line_skip_code {0};
constexpr std::byte baseline_skip_code {1};
constexpr std::byte par_skip_code {2};
constexpr std::byte above_display_skip_code {3};
constexpr std::byte below_display_skip_code {4};
constexpr std::byte above_display_short_skip_code {5};
constexpr std::byte below_display_short_skip_code {6};
// Skip codes 7--17 not yet defined
constexpr halfword& glue_par(halfword h) { return equiv(glue_base + h); }

// Section 225

extern void print_skip_param(int n);

// Section 230

constexpr halfword cat_code_base = 3'727; // In Section 230, this is in fact computed from a long sequence of bases
constexpr halfword int_base = 4'367; // In Section 230, this is in fact computed from a long sequence of bases

constexpr halfword& cat_code(halfword h) { return equiv(cat_code_base + h); }

// Section 232

constexpr std::byte null_font {font_base}; // null_font is implicitly type int in TeX but C++ has stricter requirements

// Section 236

constexpr halfword show_box_breadth_code = 24;
constexpr halfword show_box_depth_code = 25;
constexpr halfword escape_char_code = 45;
constexpr halfword int_pars = 50;
constexpr halfword count_base = int_base + int_pars;
constexpr halfword del_code_base = count_base + 256;
constexpr halfword dimen_base = del_code_base + 128;

constexpr int& int_par(halfword code) { return eqtb[int_base + code].intv; }

constexpr int& show_box_breadth() { return int_par(show_box_breadth_code); }
constexpr int& show_box_depth() { return int_par(show_box_depth_code); }
constexpr int& escape_char() { return int_par(escape_char_code); }

// Section 247

constexpr halfword dimen_pars = 20;
constexpr halfword scaled_base = dimen_base + dimen_pars;
constexpr halfword eqtb_size = scaled_base + 255;

#endif

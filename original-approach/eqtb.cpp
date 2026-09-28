/*
  Excerpts of the eqtb.
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

#include "eqtb.hpp"

#include "printing.hpp"

// Not in TeX; handled as part of other routines
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

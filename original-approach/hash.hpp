#ifndef HASH_HPP
#define HASH_HPP
/*
  Excerpts from the hash table
  Part 18 of TeX: The Program

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

#include <cstddef>
#include <string_view>

#include "basic_memory.hpp"

// Section 256

extern two_halves hash[];

constexpr halfword& next(int i) { return hash[i].lh; }
constexpr halfword& text(int i) { return hash[i].rh; }

// This implementation will be elaborated when the hash table is implemented
constexpr std::string_view font_id_text(std::byte font_id) {
  return std::string_view("Default font");
}

// Section 262

extern void print_cs(int p);

#endif

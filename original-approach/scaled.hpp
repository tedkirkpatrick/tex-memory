/*
  A subset of the definitions required for TeX's arithmetic with scaled dimensions
  Part 7 of TeX: The Program

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

#ifndef SCALED_HPP
#define SCALED_HPP

#include <cstddef>

#include "basic_memory.hpp"

// Section 101

constexpr int half = 0x0'80'00; // Not in TeX but useful for testing
constexpr int unity = 0x1'00'00;
constexpr int two = 0x2'00'00;

using scaled = int;
using non_negative_integer = unsigned int; // TeX defines this with a max. of 2^31 - 1, not 2^32 - 1
using small_number = std::byte;

// Section 103

extern void print_scaled(scaled s);

// Section 104

extern bool arith_error;

// Section 105

extern scaled nx_plus_y(int n, scaled x, scaled y);

// Section 108

constexpr halfword inf_bad = 10'000;

#endif

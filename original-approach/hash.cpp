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

#include "hash.hpp"

#include <format>
#include <iostream>
#include <print>

#include "command_codes.hpp"
#include "eqtb.hpp"
#include "hash.hpp"
#include "printing.hpp"
#include "string_handling.hpp"

// Section 256

two_halves hash[undefined_control_sequence];

// Section 262

void print_cs(int p) {
  if (p < hash_base) {
    if (p >= single_base)
      if (p == null_cs) {
        print_esc("csname");
        print_esc("endcsname");
      }
      else {
        print_esc(std::format("Whatever control sequence is located at string {}", p - single_base));
        if (cat_code(p - single_base) == letter)
          std::cout << ' ';
      }
    else if (p < active_base)
      print_esc("IMPOSSIBLE.");
    else
      std::cout << p - active_base;
  }
  else if (p >= undefined_control_sequence)
    print_esc("IMPOSSIBLE.");
  else if (text(p) < 0 || text(p) >= str_ptr)
    print_esc("NONEXISTENT.");
  else {
    print_esc("");
    std::print("String referenced by hash entry {}", text(p));
    std::cout << ' ';
  }
}

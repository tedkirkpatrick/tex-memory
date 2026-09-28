#ifndef TOKEN_LIST_HPP
#define TOKEN_LIST_HPP
/*
  Token lists.
  Part 20 of TeX: The Program

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

#include "basic_memory.hpp"
#include "command_codes.hpp"

// Not in TeX but useful for testing and debugging

[[nodiscard]] constexpr halfword make_letter_token(char c) { return halfword(letter * 0x1'00 + c); }

[[nodiscard]] extern pointer new_token_list(int refc);

extern pointer add_token_to_list(pointer p, halfword token);

// Section 289

constexpr int cs_token_flag = 0x10'00;

// Section 292

extern void show_token_list(int p, int q, int l);

// Section 295

extern void token_show(pointer p);

// Section 296

extern void print_meaning();

#endif

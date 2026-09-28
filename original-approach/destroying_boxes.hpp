#ifndef DESTROYING_BOXES_HPP
#define DESTROYING_BOXES_HPP
/*
  Destroying boxes.
  Part 13 of TeX: The Program

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
#include "boxes.hpp"

// For debugging
#include <print>

// Section 200

constexpr halfword& token_ref_count(pointer p) { return info(p); }
constexpr void delete_token_ref(pointer p) {
  if (token_ref_count(p) == null) {
    flush_list(p);
  }
  else
    decr(token_ref_count(p));
}

// Section 201

constexpr void fast_delete_glue_ref(pointer p) {
  if (glue_ref_count(p) == null)
    free_node(p, glue_spec_size);
  else
    decr(glue_ref_count(p));
}

extern void delete_glue_ref(pointer p);

// Section 202

extern void flush_node_list(pointer p);

#endif

#ifndef EXTENSIONS_HPP
#define EXTENSIONS_HPP
/*
  Selected constants for extensions.
  Part 53 of TeX: The Program

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

#include "basic_memory.hpp"

// Section 1341

constexpr halfword write_node_size = 2;
constexpr halfword open_node_size = 3;

constexpr std::byte open_node {0};
constexpr std::byte write_node {1};
constexpr std::byte close_node {2};
constexpr std::byte special_node {3};

constexpr halfword& write_tokens(pointer p) { return link(p + 1); }

#endif

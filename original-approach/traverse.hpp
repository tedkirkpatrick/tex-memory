#ifndef TRAVERSE_HPP
#define TRAVERSE_HPP
/*
  Testing code.
  Does not correspond to any code in TeX.

  A function to touch an element of every node linked from pointer p.
  Logic based on node_display() except we don't display, we simply
  grab an integer value and recurse down into any linked nodes.

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines are NOT transliterations of TeX code. They are purpose-written
  for this demonstration. They are written in modern C++.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "basic_memory.hpp"

int traverse_node(pointer p);

#endif

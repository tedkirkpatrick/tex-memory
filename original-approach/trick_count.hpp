#ifndef TRICK_COUNT_HPP
#define TRICK_COUNT_HPP

/*
  Substitute for trick count function.
  Part 22 of TeX: The Program

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines are NOT transliterations of TeX code. They are purpose-written
  for this demonstration. They are written in modern C++.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

// Section 316

// We're not worrying about table size the way TeX must, so we don't do the pseudoprinting trick
constexpr void set_trick_count() { return; }

#endif

#ifndef OVERFLOW_HPP
#define OVERFLOW_HPP
/*
  Substitute routines for Part 6, "Reporting Errors"

  These routines have a different purpose, as this code is only going
  to be used for demonstration and testing purposes. So I don't mimic
  the full complexity of the routines in TeX Part 6.

  Furthermore, this code only runs on ASCII-based machines, so I do
  not include translation to alternative external character sets.

  The routines write to a stream, typically std::cout or a streambuf.

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines are NOT transliterations of TeX code. They are purpose-written
  for this demonstration. They are written in modern C++.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <string_view>

// Section 94

extern void overflow(std::string_view s, int n);

// Section 95

extern void confusion(std::string_view s);

#endif

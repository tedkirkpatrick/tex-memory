/*
  Scaled arithmentic
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

#include "scaled.hpp"

#include "printing.hpp"

// Section 103

void print_scaled(scaled s) {
  scaled delta;
  if (s < 0) {
    print_char('-');
    s = - s;
  }
  print_int(s / unity);
  print_char('.');
  s = 10 * (s % unity) + 5;
  delta = 10;
  do {
    if (delta > unity)
      s += 0x80'00 - (delta / 2);
    print_char('0' + (s / unity));
    s = 10 * (s % unity);
    delta *= 10;
  }
  while (s > delta);
}

// Section 104

bool arith_error {false};

// Section 105

scaled nx_plus_y(int n, scaled x, scaled y) {
  if (n < 0) {
    x = -x;
    n = -n;
  }
  if (n == 0)
    return y;
  else if (x <= (0x3F'FF'FF'FF - y) / n && -x <= (0x3F'FF'FF'FF + y) / n)
    return n * x + y;
  else {
    arith_error = true;
    return 0;
  }
}

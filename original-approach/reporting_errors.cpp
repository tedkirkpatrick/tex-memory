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

#include "reporting_errors.hpp"

#include <format>
#include <sstream>
#include <stdexcept>
#include <string_view>


// Section 94

// This implementation uses C++ exceptions rather than the internal TeX error handlers

void overflow(std::string_view s, int n) {
  throw std::runtime_error(std::format("TeX capacity exceeded, Sorry [{}={}].  Exiting", s, n));
}

// Section 95

void confusion(std::string_view s) {
  throw std::runtime_error(std::format("Internal error: {}", s));
}

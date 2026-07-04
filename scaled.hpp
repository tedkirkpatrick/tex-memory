/*
  A subset of the definitions required for TeX's arithmetic with scaled dimensions
 */

#ifndef SCALED_HPP
#define SCALED_HPP

#include <cstddef>

// Section 101

using scaled = int;
using non_negative_integer = unsigned int; // TeX defines this with a max. of 2^31 - 1, not 2^32 - 1
using small_number = std::byte;

#endif

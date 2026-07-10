/*
  A subset of the definitions required for TeX's arithmetic with scaled dimensions
 */

#ifndef SCALED_HPP
#define SCALED_HPP

#include <cstddef>

#include "basic-memory.hpp"

// Section 101

constexpr int unity = 0x1'00'00;
constexpr int two = 0x2'00'00;

using scaled = int;
using non_negative_integer = unsigned int; // TeX defines this with a max. of 2^31 - 1, not 2^32 - 1
using small_number = std::byte;

// Section 103

extern void print_scaled(scaled s);

// Section 108

constexpr halfword inf_bad = 10'000;

#endif

#ifndef HASH_HPP
#define HASH_HPP

#include <cstddef>
#include <string_view>

#include "basic-memory.hpp"

// Section 256

extern two_halves hash[];

constexpr halfword& next(int i) { return hash[i].lh; }
constexpr halfword& text(int i) { return hash[i].rh; }

// This implementation will be elaborated when the hash table is implemented
constexpr std::string_view font_id_text(std::byte font_id) {
  return std::string_view("Font name will go here");
}

// Section 262

extern void print_cs(int p);

#endif

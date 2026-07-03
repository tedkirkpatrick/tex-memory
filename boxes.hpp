/*
  Data structures for vlists, hlists, and their friends
 */

#ifndef BOXES_HPP
#define BOXES_HPP

#include <cstddef>

#include "basic-memory.hpp"

// Section 133

constexpr std::byte type(pointer p) { return mem[p].hh.qw.b0; }
constexpr std::byte subtype(pointer p) { return mem[p].hh.qw.b1; }

// Section 134

constexpr bool is_char_node(pointer p) { return p >= hi_mem_min; }
// TeX defines the next two as WEB macros but C++ requires repeating the bodies
constexpr std::byte font(pointer p) { return mem[p].hh.qw.b0; }
constexpr std::byte character(pointer p) { return mem[p].hh.qw.b1; }

// Section 135

constexpr std::byte hlist_node {0};
constexpr halfword box_node_size = 7;
constexpr halfword width_offset = 1;
constexpr halfword depth_offset = 2;
constexpr halfword height_offset = 3;
constexpr sc width(pointer p) { return mem[p + width_offset].scv; }
constexpr sc depth(pointer p) { return mem[p + depth_offset].scv; }
constexpr sc height(pointer p) { return mem[p + height_offset].scv; }
constexpr sc shift_amount(pointer p) { return mem[p + 4].scv; } // No idea why TeX uses explicit constant "4"
constexpr halfword list_offset = 5;
constexpr pointer list_ptr(pointer p) { return link(p + list_offset); }
constexpr std::byte glue_order(pointer p) { return subtype(p + list_offset); }
constexpr std::byte glue_sign(pointer p) { return type(p + list_offset); }
constexpr halfword normal = 0;
constexpr halfword stretching = 1;
constexpr halfword shrinking = 2;
constexpr halfword glue_offset = 6;
constexpr glue_ratio glue_set(pointer p) { return mem[p + glue_offset].gr; }

#endif

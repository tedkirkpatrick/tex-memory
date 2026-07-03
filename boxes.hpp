/*
  Data structures for vlists, hlists, and their friends
 */

#ifndef BOXES_HPP
#define BOXES_HPP

#include <cstddef>

#include "basic-memory.hpp"

// Section 133

constexpr std::byte& type(pointer p) { return mem[p].hh.qw.b0; }
constexpr std::byte& subtype(pointer p) { return mem[p].hh.qw.b1; }

// Section 134

constexpr bool is_char_node(pointer p) { return p >= hi_mem_min; }
// TeX defines the next two as WEB macros but C++ requires repeating the bodies
constexpr std::byte& font(pointer p) { return mem[p].hh.qw.b0; }
constexpr std::byte& character(pointer p) { return mem[p].hh.qw.b1; }

// Section 135

constexpr std::byte hlist_node {0};
constexpr halfword box_node_size = 7;
constexpr halfword width_offset = 1;
constexpr halfword depth_offset = 2;
constexpr halfword height_offset = 3;
constexpr sc& width(pointer p) { return mem[p + width_offset].scv; }
constexpr sc& depth(pointer p) { return mem[p + depth_offset].scv; }
constexpr sc& height(pointer p) { return mem[p + height_offset].scv; }
constexpr sc& shift_amount(pointer p) { return mem[p + 4].scv; } // No idea why TeX uses explicit constant "4"
constexpr halfword list_offset = 5;
constexpr pointer& list_ptr(pointer p) { return link(p + list_offset); }
constexpr std::byte& glue_order(pointer p) { return subtype(p + list_offset); }
constexpr std::byte& glue_sign(pointer p) { return type(p + list_offset); }
constexpr std::byte normal {0};
constexpr std::byte stretching {1};
constexpr std::byte shrinking {2};
constexpr halfword glue_offset = 6;
constexpr glue_ratio& glue_set(pointer p) { return mem[p + glue_offset].gr; }

// Section 136

extern pointer new_null_box();

// Section 137

constexpr std::byte vlist_node {1};

// Section 138

constexpr std::byte rule_node {2};
constexpr halfword rule_node_size = 4;
constexpr int null_flag = - 0x40'00'00'00;

// Section 139

extern pointer new_rule();

// Section 140

constexpr std::byte ins_node {3};
constexpr halfword ins_node_size = 5;
constexpr int& float_cost(pointer p) { return mem[p + 1].intv; }
constexpr pointer& ins_ptr(pointer p) { return info(p + 4); }
constexpr pointer& split_top_ptr(pointer p) { return link(p + 4); }

// Section 141

constexpr std::byte mark_node {4};
constexpr halfword small_node_size = 2;
constexpr int& mark_ptr(pointer p) { return mem[p + 1].intv; }

// Section 142

constexpr std::byte adjust_node {5};
// TeX uses WEB equivalence to mark_ptr but i C++ we have to repeat the body for following function
constexpr int& adjust_ptr(pointer p) { return mem[p + 1].intv; }

// Section 143

constexpr std::byte ligature_node {6};
constexpr pointer lig_char(pointer p) { return p + 1; }
constexpr pointer& lig_ptr(pointer p) { return link(lig_char(p)); }

// Section 144

extern pointer new_ligature(quarterword f, quarterword c, pointer q);

#endif

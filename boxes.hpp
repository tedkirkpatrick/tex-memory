/*
  Data structures for vlists, hlists, and their friends
 */

#ifndef BOXES_HPP
#define BOXES_HPP

#include <cstddef>

#include "basic-memory.hpp"
#include "eqtb.hpp"
#include "scaled.hpp"

// Section 133

constexpr std::byte& type(pointer p) { return mem[p].hh.qw.b0; }
constexpr std::byte& subtype(pointer p) { return mem[p].hh.qw.b1; }

// Section 134

constexpr bool is_char_node(pointer p) { return p >= hi_mem_min; }
constexpr std::byte& font(pointer p) { return type(p); }
constexpr std::byte& character(pointer p) { return subtype(p); }

// Next two not present in TeX but useful for testing
[[nodiscard]] extern pointer new_char_node(std::byte font_v, char c_v);
extern pointer add_char_node_to_list(pointer p, std::byte font_v, char c_v);

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

[[nodiscard]] extern pointer new_null_box();

// Section 137

constexpr std::byte vlist_node {1};

// Section 138

constexpr std::byte rule_node {2};
constexpr halfword rule_node_size = 4;
constexpr int null_flag = - 0x40'00'00'00;
constexpr bool is_running(scaled d) { return d == null_flag; }

// Section 139

[[nodiscard]] extern pointer new_rule();

// Section 140

constexpr std::byte ins_node {3};
constexpr halfword ins_node_size = 5;
constexpr int& float_cost(pointer p) { return mem[p + 1].intv; }
constexpr pointer& ins_ptr(pointer p) { return info(p + 4); }
constexpr pointer& split_top_ptr(pointer p) { return link(p + 4); }

// Not in TeX, added for testing convenience
[[nodiscard]] extern pointer new_ins(std::byte subtype, scaled height, scaled depth,
                                     pointer split_top_ptr, int float_penalty, pointer ins_ptr);

// Section 141

constexpr std::byte mark_node {4};
constexpr halfword small_node_size = 2;
constexpr int& mark_ptr(pointer p) { return mem[p + 1].intv; }

// Not in TeX, added for testing convenience
[[nodiscard]] extern pointer new_mark(pointer token_list);

// Section 142

constexpr std::byte adjust_node {5};
constexpr int& adjust_ptr(pointer p) { return mark_ptr(p); }

// Not in TeX, added for testing convenience
[[nodiscard]] extern pointer new_adjust(pointer vlist);

// Section 143

constexpr std::byte ligature_node {6};
constexpr pointer lig_char(pointer p) { return p + 1; }
constexpr pointer& lig_ptr(pointer p) { return link(lig_char(p)); }

// Section 144

[[nodiscard]] extern pointer new_ligature(quarterword f, quarterword c, pointer q);

// Section 145

constexpr std::byte disc_node {7};
constexpr std::byte& replace_count(pointer p) { return subtype(p); }
constexpr pointer& pre_break(pointer p) { return llink(p); }
constexpr pointer& post_break(pointer p) { return llink(p); }

[[nodiscard]] extern pointer new_disc();

// Section 146

constexpr std::byte whatsit_node {8};

// Section 147

constexpr std::byte math_node {9};
constexpr std::byte before {0};
constexpr std::byte after {1};

[[nodiscard]] extern pointer new_math(scaled w, small_number s);

// Section 148

constexpr bool precedes_break(pointer p) { return type(p) < math_node; }
constexpr bool non_discardable(pointer p) { return type(p) < math_node; }

// Section 149

constexpr std::byte glue_node {10};
constexpr std::byte cond_math_glue {98};
constexpr std::byte mu_glue {99};
constexpr std::byte a_leaders {100};
constexpr std::byte c_leaders {101};
constexpr std::byte x_leaders {102};
constexpr pointer& glue_ptr(pointer p) { return llink(p); }
constexpr pointer& leader_ptr(pointer p) { return rlink(p); }

// Section 150

constexpr halfword glue_spec_size = 4;
constexpr pointer& glue_ref_count(pointer p) { return link(p); }
constexpr sc& stretch(pointer p) { return mem[p + 2].scv; }
constexpr sc& shrink(pointer p) { return mem[p + 3].scv; }
constexpr std::byte& stretch_order(pointer p) { return type(p); }
constexpr std::byte& shrink_order(pointer p) { return subtype(p); }
constexpr std::byte fil {1};
constexpr std::byte fill {2};
constexpr std::byte filll {3};

using glue_ord = halfword; // Could fit into a std::byte (range is only 0 .. 3) but halfword is more manageable

// Section 151

[[nodiscard]] extern pointer new_spec(pointer p);
[[nodiscard]] pointer new_glue_spec(scaled width, scaled stretch_v, std::byte stretch_o, scaled shrink_v, std::byte shrink_o);

// Section 152

[[nodiscard]] extern pointer new_param_glue(small_number n);

// Section 153

[[nodiscard]] extern pointer new_glue(pointer q);

// Section 154

[[nodiscard]] pointer new_skip_param(small_number n);

// Section 155

constexpr std::byte kern_node {11};
constexpr std::byte explicit_kern {1};
constexpr std::byte acc_kern {2};

// Section 156

[[nodiscard]] extern pointer new_kern(scaled w);

// Section 157

constexpr std::byte penalty_node {12};
constexpr int inf_penalty = inf_bad;
constexpr int eject_penalty = - inf_penalty;
constexpr int& penalty(pointer p) { return mem[p + 1].intv; }

// Section 158

[[nodiscard]] extern pointer new_penalty(int m);

// Section 159

constexpr std::byte unset_node {13};
constexpr sc& glue_stretch(pointer p) { return mem[p + glue_offset].scv; }
constexpr sc& glue_shrink(pointer p) { return shift_amount(p); }
constexpr std::byte& span_count(pointer p) { return subtype(p); }

#endif

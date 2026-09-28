/*
  Data structures for boxes (vlists, hlists, and their friends).
  Part 10 of TeX: The Program

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

#include "boxes.hpp"

// Section 134

// Next two not present in TeX but useful for testing
[[nodiscard]] pointer new_char_node(std::byte font_v, char c_v) {
  pointer c = get_avail();
  font(c) = font_v;
  character(c) = std::byte(c_v);
  return c;
}

pointer add_char_node_to_list(pointer p, std::byte font_v, char c_v) {
  while(link(p) != null)
    p = link(p);
  pointer c_n = new_char_node(font_v, c_v);
  link(p) = c_n;
  return c_n;
}

// Section 136

pointer new_null_box() {
  pointer p;
  p = get_node(box_node_size);
  type(p) = hlist_node;
  subtype(p) = min_quarterword;
  width(p) = 0;
  depth(p) = 0;
  height(p) = 0;
  shift_amount(p) = 0;
  list_ptr(p) = null;
  glue_sign(p) = normal;
  glue_order(p) = normal;
  set_glue_ratio_zero(glue_set(p));
  return p;
}

// Section 139

pointer new_rule() {
  pointer p;
  p = get_node(rule_node_size);
  type(p) = rule_node;
  subtype(p) = std::byte(0);
  width(p) = null_flag;
  depth(p) = null_flag;
  return p;
}

// Section 140

// Not in TeX, added for testing convenience
pointer new_ins(std::byte st, scaled h, scaled d,
                pointer st_ptr, int float_penalty, pointer i_ptr) {
  pointer p = get_node(ins_node_size);
  type(p) = ins_node;
  subtype(p) = st;
  height(p) = h;
  depth(p) = d;
  split_top_ptr(p) = st_ptr;
  float_cost(p) = float_penalty;
  ins_ptr(p) = i_ptr;
  return p;
}

// Section 141

// Not in TeX, added for testing convenience
[[nodiscard]] pointer new_mark(pointer token_list) {
  pointer p = get_node(small_node_size);
  type(p) = mark_node;
  mark_ptr(p) = token_list;
  return p;
}

// Section 142

// Not in TeX, added for testing convenience
[[nodiscard]] pointer new_adjust(pointer vlist) {
  pointer p = get_node(small_node_size);
  type(p) = adjust_node;
  adjust_ptr(p) = vlist;
  return p;
}
 
// Section 144

[[nodiscard]] pointer new_ligature(quarterword f, quarterword c, pointer q) {
  pointer p;
  p = get_node(small_node_size);
  type(p) = ligature_node;
  subtype(p) = std::byte(0);
  font(lig_char(p)) = f;
  character(lig_char(p)) = c;
  lig_ptr(p) = q;
  return p;
}

// Section 145

[[nodiscard]] pointer new_disc() {
  pointer p;
  p = get_node(small_node_size);
  type(p) = disc_node;
  replace_count(p) = std::byte(0);
  pre_break(p) = null;
  post_break(p) = null;
  return p;
}

// Section 147

[[nodiscard]] pointer new_math(scaled w, small_number s) {
  pointer p;
  p = get_node(small_node_size);
  type(p) = math_node;
  subtype(p) = s;
  width(p) = w;
  return p;
}

// Section 151

[[nodiscard]] pointer new_spec(pointer p) {
  pointer q;
  q = get_node(glue_spec_size);
  mem[q] = mem[p];
  glue_ref_count(q) = null;
  width(q) = width(p);
  stretch(q) = stretch(p);
  shrink(q) = shrink(p);
  return q;
}

// Not in TeX but useful for testing. Create a glue spec
[[nodiscard]] pointer new_glue_spec(scaled width_v, scaled stretch_v, std::byte stretch_o,
                                    scaled shrink_v, std::byte shrink_o, halfword ref_c) {
  pointer p = get_node(glue_spec_size);
  glue_ref_count(p) = ref_c;
  width(p) = width_v;
  stretch(p) = stretch_v;
  stretch_order(p) = stretch_o;
  shrink(p) = shrink_v;
  shrink_order(p) = shrink_o;
  return p;
}

// Section 152

[[nodiscard]] pointer new_param_glue(small_number n) {
  pointer p;
  pointer q;
  p = get_node(small_node_size);
  type(p) = glue_node;
  subtype(p) = small_number(int(n) + 1);
  leader_ptr(p) = null;
  q = glue_par(halfword(n));
  glue_ptr(p) = q;
  incr(glue_ref_count(q));
  return p;
}

//  Section 153

[[nodiscard]] pointer new_glue(pointer q) {
  pointer p;
  p = get_node(small_node_size);
  type(p) = glue_node;
  subtype(p) = normal;
  leader_ptr(p) = null;
  glue_ptr(p) = q;
  incr(glue_ref_count(q));
  return p;
}

// Section 154

[[nodiscard]] pointer new_skip_param(small_number n) {
  pointer p;
  temp_ptr = new_spec(glue_par(halfword(n)));
  p = new_glue(temp_ptr);
  glue_ref_count(temp_ptr) = null;
  subtype(p) = small_number(int(n) + 1);
  return p;
}

// Setion 156

[[nodiscard]] pointer new_kern(scaled w) {
  pointer p;
  p = get_node(small_node_size);
  type(p) = kern_node;
  subtype(p) = normal;
  width(p) = w;
  return p;
}

// Section 158

[[nodiscard]] pointer new_penalty(int m) {
  pointer p;
  p = get_node(small_node_size);
  type(p) = penalty_node;
  subtype(p) = std::byte(0);
  penalty(p) = m;
  return p;
}

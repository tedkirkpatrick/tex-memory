#include "boxes.hpp"

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

// Section 144

pointer new_ligature(quarterword f, quarterword c, pointer q) {
  pointer p;
  p = get_node(small_node_size);
  type(p) = ligature_node;
  subtype(p) = std::byte(0);
  font(lig_char(p)) = f;
  character(lig_char(p)) = c;
  return p;
}

// Section 145

pointer new_disc() {
  pointer p;
  p = get_node(small_node_size);
  type(p) = disc_node;
  replace_count(p) = std::byte(0);
  pre_break(p) = null;
  post_break(p) = null;
  return p;
}

// Section 147

pointer new_math(scaled w, small_number s) {
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

// Not in TeX but useful for testing. Create a glue spec with a ref count of 0.
[[nodiscard]] pointer new_glue_spec(scaled width, scaled stretch_v, std::byte stretch_o, scaled shrink_v, std::byte shrink_o) {
  pointer p = get_node(glue_spec_size);
  glue_ref_count(p) = 0;
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

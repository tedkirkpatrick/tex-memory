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

pointer new_spec(pointer p) {
  pointer q;
  q = get_node(glue_spec_size);
  mem[q] = mem[p];
  glue_ref_count(q) = null;
  width(q) = width(p);
  stretch(q) = stretch(p);
  shrink(q) = shrink(p);
  return q;
}

// Section 152

pointer new_param_glue(small_number n) {
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

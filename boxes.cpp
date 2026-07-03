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

#include "sample_lists.hpp"

#include "basic_memory.hpp"
#include "boxes.hpp"
#include "catch2.hpp"
#include "extensions.hpp"
#include "token_list.hpp"

// For debugging only

#include <print>

constexpr std::byte default_font {0};

[[nodiscard]] static pointer new_hlist(char ch='a') {
  pointer h = new_null_box();
  pointer c = get_avail();
  font(c) = default_font;
  character(c) = std::byte(ch);
  list_ptr(h) = c;
  return h;
}

[[nodiscard]] static pointer new_vlist(char ch='a') {
  pointer v = new_null_box();
  type(v) = vlist_node;
  pointer h = new_hlist(ch);
  list_ptr(v) = h;
  return v;
}

[[nodiscard]] static pointer new_char_node(char c, pointer prev=null) {
  pointer cn = get_avail();
  font(cn) = default_font;
  character(cn) = std::byte(c);
  if (prev != null) {
    link(prev) = cn;
  }
  return cn;
}

[[nodiscard]] static pointer new_ins_node() {
  std::byte boxn {11};
  pointer gs = new_glue_spec(unity, 0, normal, 0, normal);
  pointer p = new_ins(boxn, two, unity, gs, 0, new_null_box());
  return p;
}

[[nodiscard]] static pointer new_whatsit_write_node() {
  pointer w = get_node(small_node_size);
  type(w) = whatsit_node;
  subtype(w) = write_node;
  pointer token_list = new_token_list(0);
  halfword q = make_letter_token('q');
  halfword r = make_letter_token('r');
  add_token_to_list(add_token_to_list(token_list, q), r);
  //pointer nb = new_null_box();
  //std::println("new_null_box() for whatsit {}", nb);
  write_tokens(w) = token_list;
  return w;
}

[[nodiscard]] static pointer new_glue_node(pointer leader=null) {
  pointer gs = new_glue_spec(two, unity, normal, unity, normal);
  pointer g = new_glue(gs);
  leader_ptr(g) = leader;
  return g;
}

[[nodiscard]] static pointer new_ligature_node() {
  pointer cl = new_char_node(default_font, 'f');
  add_char_node_to_list(cl, default_font, 'i');
  return new_ligature(default_font, quarterword('F'), cl);
}

[[nodiscard]] static pointer new_mark_node() {
  pointer token_list = new_token_list(0);
  halfword a = make_letter_token('a');
  halfword z = make_letter_token('z');
  add_token_to_list(add_token_to_list(token_list, a), z);
  return new_mark(token_list);
}

[[nodiscard]] static pointer new_disc_node() {
  pointer d = new_disc();
  replace_count(d) = std::byte(5);
  pre_break(d) = new_vlist('x');
  post_break(d) = new_vlist('y');
  return d;
}

[[nodiscard]] pointer new_basic_list() {
  pointer vl = new_null_box();
  std::println("new_null() box for vlist at {}", vl);
  type(vl) = vlist_node;
  pointer hl = new_null_box();
  std::println("new_null() box for hlist at {}", hl);
  list_ptr(vl) = hl;
  pointer ac = new_char_node('a');
  list_ptr(hl) = ac;
  pointer ab = new_char_node('b', ac);
  pointer r = new_rule();
  link(ab) = r;
  pointer ins = new_ins_node();
  link(r) = ins;
  pointer wo = new_whatsit_write_node();
  link(ins) = wo;
  pointer gn = new_null_box();
  std::println("new_null() box for glue at {}", gn);
  pointer g = new_glue_node(gn);
  link(wo) = g;
  pointer k = new_kern(two);
  link(g) = k;
  pointer l = new_ligature_node();
  link(k) = l;
  pointer m = new_mark_node();
  link(l) = m;
  pointer d = new_disc_node();
  link(m) = d;
  return vl;
}

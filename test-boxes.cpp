/*
  Style:  Since this code is original and does not mimic TeX's Pascal code,
  it uses a more modern C++ style, such as initializing variables at declaration.
 */

#include <format>
#include <iostream>
#include <sstream>

#include "boxes.hpp"
#include "catch2.hpp"
#include "display.hpp"
#include "eqtb.hpp"
#include "extensions.hpp"
#include "printing.hpp"
#include "string_handling.hpp"
#include "token_list.hpp"

// Debugging
#include <print>

TEST_CASE("Printing char") {
  std::ostringstream ostr;
  set_str(&ostr);
  print_char('a');
  REQUIRE(ostr.view() == "a");
}


TEST_CASE("Printing ASCII 0") {
  std::ostringstream ostr;
  set_str(&ostr);
  print(0);
  REQUIRE(ostr.view() == "^^@");
}

TEST_CASE("Printing ASCII A") {
  std::ostringstream ostr;
  set_str(&ostr);
  print(65);
  REQUIRE(ostr.view() == "A");
}

TEST_CASE("Printing ASCII <del>") {
  std::ostringstream ostr;
  set_str(&ostr);
  print(127);
  REQUIRE(ostr.view() == "^^?");
}

TEST_CASE("Creating and printing a string") {
  std::ostringstream ostr;
  set_str(&ostr);
  str_start.reset_strings();
  str_number first_string = str_start.first_avail();
  append_char('a');
  append_char('b');
  int a = make_string();
  REQUIRE(a == first_string);
  print(a);
  REQUIRE(ostr.view() == "ab");
}

TEST_CASE("Create several strings") {
  std::ostringstream ostr;
  set_str(&ostr);
  str_start.reset_strings();
  str_number first_string = str_start.first_avail();
  append_char('a');
  append_char('b');
  int s1 = make_string();
  append_char('c');
  append_char('d');
  int s2 = make_string();
  REQUIRE(s1 == first_string);
  REQUIRE(s2 == first_string + 1);
  print(s1);
  print_nl(s2);
  REQUIRE(ostr.view() == "ab\ncd");
}

TEST_CASE("Test print_current_string()") {
  str_start.reset_strings();
  std::ostringstream ostr;
  set_str(&ostr);
  append_char('a');
  append_char('b');
  (void) make_string();
  append_char('c');
  append_char('d');
  (void) make_string();
  append_char('e');
  append_char('f');
  print_current_string();
  REQUIRE(ostr.view() == "ef");
}

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

TEST_CASE("Printing a vlist") {
  init_table_entries();
  init_eqtb();
  str_start.reset_strings();
  std::ostringstream ostr;
  set_str(&ostr);
  pointer v = new_vlist();
  show_box(v);
  REQUIRE(ostr.view() == "\n\\vbox(0.0+0.0)x0.0\n.\\hbox(0.0+0.0)x0.0\n..\\Default font a\n");
}

TEST_CASE("Create various node types") {
  init_table_entries();
  init_eqtb();
  str_start.reset_strings();
  std::ostringstream ostr;
  set_str(&ostr);
  SECTION("Create rule node") {
    pointer r = new_rule();
    height(r) = two;
    depth(r) = nx_plus_y(2, two, 0);
    width(r) = unity;
    show_box(r);
    REQUIRE(ostr.view() == "\n\\rule(2.0+4.0)x1.0\n");
  }
  SECTION("Create ins node with basic glue spec") {
    std::byte boxn {11};
    pointer gs = new_glue_spec(unity, 0, normal, 0, normal);
    pointer p = new_ins(boxn, two, unity, gs, 0, null);
    show_box(p);
    REQUIRE(ostr.view() ==
            std::format("\n\\insert{}, natural size 2.0; split(1.0,1.0) float cost 0\n", int(boxn)));
  }
  SECTION("Create more complete glue spec from an ins node containing a vlist") {
    std::byte boxn {43};
    pointer gs= new_glue_spec(nx_plus_y(1, unity, half), unity, fil, two, fill);
    pointer v = new_vlist();
    pointer p = new_ins(boxn, two, unity, gs, 3, v);
    show_box(p);
    REQUIRE(ostr.view() ==
            std::format("\n\\insert{}, natural size 2.0; split(1.5 plus 1.0fil minus 2.0fill,1.0) float cost 3\n.\\vbox(0.0+0.0)x0.0\n..\\hbox(0.0+0.0)x0.0\n...\\Default font a\n", int(boxn)));
  }
  SECTION("Create mark node") {
    pointer token_list = new_token_list(1);
    halfword a = make_letter_token('a');
    halfword z = make_letter_token('z');
    add_token_to_list(add_token_to_list(token_list, a), z);
    pointer m = new_mark(token_list);
    show_box(m);
    REQUIRE(ostr.view() == "\n\\mark{az}\n");
  }
  SECTION("Create adjust node") {
    pointer v = new_vlist();
    pointer a = new_adjust(v);
    show_box(a);
    REQUIRE(ostr.view() == "\n\\vadjust\n.\\vbox(0.0+0.0)x0.0\n..\\hbox(0.0+0.0)x0.0\n...\\Default font a\n");
  }
  SECTION("Create ligature node") {
    pointer cl = new_char_node(default_font, 'f');
    add_char_node_to_list(cl, default_font, 'f');
    pointer l = new_ligature(default_font, quarterword('f'), cl);
    show_box(l);
    REQUIRE(ostr.view() == "\n\\Default font f (ligature ff)\n");
  }
  SECTION("Create discretionary line break node") {
    pointer d = new_disc();
    replace_count(d) = std::byte(5);
    pre_break(d) = new_vlist('x');
    post_break(d) = new_vlist('y');
    show_box(d);
    REQUIRE(ostr.view() ==
            "\n\\discretionary replacing 5\n"
            ".\\vbox(0.0+0.0)x0.0\n"
            "..\\hbox(0.0+0.0)x0.0\n"
            "...\\Default font x\n"
            "|\\vbox(0.0+0.0)x0.0\n"
            "|.\\hbox(0.0+0.0)x0.0\n"
            "|..\\Default font y\n"
            );
  }
  SECTION("Whatsit node") {
    pointer w = get_node(small_node_size);
    type(w) = whatsit_node;
    subtype(w) = open_node;
    show_box(w);
    REQUIRE(ostr.view() == "\nopen_node subtype of whatsit_node\n");
  }
  SECTION("Math node") {
    pointer m = new_math(two, after);
    show_box(m);
    REQUIRE(ostr.view() == "\n\\mathoff, surrounded 2.0\n");
    
  }
  SECTION("Normal glue node") {
    pointer gs = new_glue_spec(two, unity, normal, unity, normal);
    pointer g = new_glue(gs);
    REQUIRE(glue_ref_count(glue_ptr(g)) == 1);
    show_box(g);
    REQUIRE(ostr.view() == "\n\\glue 2.0 plus 1.0 minus 1.0\n");
  }
  SECTION("Math glue node") {
    pointer gs = new_glue_spec(unity, 0, normal, 0, normal);
    pointer g = new_glue(gs);
    subtype(g) = mu_glue;
    show_box(g);
    REQUIRE(ostr.view() == "\n\\glue(\\mskip) 1.0mu\n");
  }
  SECTION("Conditional math glue node") {
    pointer gs = new_glue_spec(unity, 0, normal, 0, normal);
    pointer g = new_glue(gs);
    subtype(g) = cond_math_glue;
    show_box(g);
    REQUIRE(ostr.view() == "\n\\glue(nonscript)\n");
  }
  SECTION("Leaders glue nodes with hlist") {
    pointer gs = new_glue_spec(unity, 0, normal, 0, normal);
    pointer g = new_glue(gs);
    leader_ptr(g) = new_hlist('g');
    SECTION("A-leaders") {
      subtype(g) = a_leaders;
      show_box(g);
      REQUIRE(ostr.view() == "\n\\leaders 1.0\n.\\hbox(0.0+0.0)x0.0\n..\\Default font g\n");
    }
    SECTION("C-leaders") {
      subtype(g) = c_leaders;
      show_box(g);
      REQUIRE(ostr.view() == "\n\\cleaders 1.0\n.\\hbox(0.0+0.0)x0.0\n..\\Default font g\n");
    }
    SECTION("X-leaders") {
      subtype(g) = x_leaders;
      show_box(g);
      REQUIRE(ostr.view() == "\n\\xleaders 1.0\n.\\hbox(0.0+0.0)x0.0\n..\\Default font g\n");
    }
  }
  SECTION("new_param_glue") {
    pointer gs = new_glue_spec(unity, 0, normal, 0, normal, 2);
    glue_par(halfword(line_skip_code)) = gs;
    pointer g = new_param_glue(line_skip_code);
    show_box(g);
    REQUIRE(ostr.view() == "\n\\glue(\\skip of type 0) 1.0\n");
    REQUIRE(glue_ref_count(gs) == 3);
  }
  SECTION("new_spec") {
    pointer gs1 = new_glue_spec(unity, two, normal, two, fill, 4);
    pointer gs2 = new_spec(gs1);
    REQUIRE(glue_ref_count(gs1) == 4);
    REQUIRE(gs1 != gs2);
    REQUIRE(type(gs1) == type(gs2));
    REQUIRE(subtype(gs1) == subtype(gs2));
    REQUIRE(glue_ref_count(gs2) == null);
    REQUIRE(width(gs2) == unity);
    REQUIRE(stretch(gs2) == two);
    REQUIRE(shrink(gs2) == two);
  }
  SECTION("new_skip_param") {
    pointer gs1 = new_glue_spec(unity, 0, normal, 0, normal, 2);
    glue_par(halfword(line_skip_code)) = gs1;
    pointer g = new_skip_param(line_skip_code);
    REQUIRE(glue_ref_count(gs1) == 2);
    REQUIRE(type(g) == glue_node);
    REQUIRE(subtype(g) == std::byte(int(line_skip_code) + 1));
    pointer gs2 = glue_ptr(g);
    REQUIRE(gs1 != gs2);
    REQUIRE(type(gs1) == type(gs2));
    REQUIRE(subtype(gs1) == subtype(gs2));
    REQUIRE(glue_ref_count(gs2) == null);
    REQUIRE(width(gs2) == unity);
    REQUIRE(stretch(gs2) == 0);
    REQUIRE(shrink(gs2) == 0);
  }
  SECTION("Kern node") {
    pointer k = new_kern(two);
    SECTION("Normal kern") {
      show_box(k);
      REQUIRE(ostr.view() == "\n\\kern2.0\n");
    }
    SECTION("Explicit kern") {
      subtype(k) = explicit_kern;
      show_box(k);
      REQUIRE(ostr.view() == "\n\\kern 2.0\n");
    }
    SECTION("Accent kern") {
      subtype(k) = acc_kern;
      show_box(k);
      REQUIRE(ostr.view() == "\n\\kern 2.0 (for accent)\n");
    }
    SECTION("mkern") {
      subtype(k) = mu_glue;
      show_box(k);
      REQUIRE(ostr.view() == "\n\\mkern2.0mu\n");
    }
  }
  SECTION("Penalty node") {
    pointer p = new_penalty(5);
    show_box(p);
    REQUIRE(ostr.view() == "\n\\penalty 5\n");
  }
  SECTION("Unset node") {
    pointer p = new_hlist();
    subtype(p) = unset_node;
    glue_stretch(p) = two;
    glue_order(p) = normal;
    glue_shrink(p) = unity;
    glue_sign(p) = fil;
    span_count(p) = std::byte(5);
    show_box(p);
    REQUIRE("\n\\unsetbox(0.0+0.0)x0.0 (6 columns), stretch 2.0, shrink 1.0fil\n");
  }
}

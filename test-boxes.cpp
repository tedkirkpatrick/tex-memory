#include <format>
#include <iostream>
#include <sstream>

#include "boxes.hpp"
#include "catch2.hpp"
#include "display.hpp"
#include "printing.hpp"
#include "string_handling.hpp"

using namespace std::literals;

using std::cout, std::dec, std::flush, std::hex;

TEST_CASE("Printing char") {
  std::ostringstream ostr;
  set_str(&ostr);
  print_char('a');
  REQUIRE(ostr.view() == "a"sv);
}


TEST_CASE("Printing ASCII 0") {
  std::ostringstream ostr;
  set_str(&ostr);
  print(0);
  REQUIRE(ostr.view() == "^^@"sv);
}

TEST_CASE("Printing ASCII A") {
  std::ostringstream ostr;
  set_str(&ostr);
  print(65);
  REQUIRE(ostr.view() == "A"sv);
}

TEST_CASE("Printing ASCII <del>") {
  std::ostringstream ostr;
  set_str(&ostr);
  print(127);
  REQUIRE(ostr.view() == "^^?"sv);
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
  REQUIRE(ostr.view() == "ab"sv);
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
  REQUIRE(ostr.view() == "ab\ncd"sv);
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
  REQUIRE(ostr.view() == "ef"sv);
}

static pointer new_vlist() {
  pointer v = new_null_box();
  type(v) = vlist_node;
  pointer h = new_null_box();
  list_ptr(v) = h;
  pointer c = get_avail();
  font(c) = std::byte(0);
  character(c) = std::byte('a');
  list_ptr(h) = c;
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
  REQUIRE(ostr.view() == "\n\\vbox(0.0+0.0)x0.0\n.\\hbox(0.0+0.0)x0.0\n..\\Default font a\n"sv);
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
    REQUIRE(ostr.view() == "\n\\rule(2.0+4.0)x1.0\n"sv);
  }
  SECTION("Create ins node with basic glue spec") {
    std::byte boxn {11};
    pointer gs = new_glue_spec(unity, 0, std::byte{0}, 0, std::byte{0});
    pointer p = new_ins(boxn, two, unity, gs, 0, null);
    show_box(p);
    REQUIRE(ostr.view() ==
            std::format("\n\\insert{}, natural size 2.0; split(1.0,1.0) float cost 0\n", int(boxn)));
  }
  SECTION("Test more complete glue spec from an ins node containing a vlist") {
    std::byte boxn {43};
    pointer gs= new_glue_spec(nx_plus_y(1, unity, half), unity, fil, two, fill);
    pointer v = new_vlist();
    pointer p = new_ins(boxn, two, unity, gs, 3, v);
    show_box(p);
    REQUIRE(ostr.view() ==
            std::format("\n\\insert{}, natural size 2.0; split(1.5 plus 1.0fil minus 2.0fill,1.0) float cost 3\n.\\vbox(0.0+0.0)x0.0\n..\\hbox(0.0+0.0)x0.0\n...\\Default font a\n", int(boxn)));
  }
}

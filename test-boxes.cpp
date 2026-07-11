#include <iostream>
#include <sstream>

#include "boxes.hpp"
#include "catch2.hpp"
#include "display.hpp"
#include "printing.hpp"
#include "string_handling.hpp"

using namespace std::literals;

using std::cout, std::flush;

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
  append_char('a');
  append_char('b');
  int a = make_string();
  REQUIRE(a == 128);
  print(a);
  REQUIRE(ostr.view() == "ab"sv);
}

TEST_CASE("Create several strings") {
  std::ostringstream ostr;
  set_str(&ostr);
  str_start.reset_strings();
  append_char('a');
  append_char('b');
  int s1 = make_string();
  append_char('c');
  append_char('d');
  int s2 = make_string();
  REQUIRE(s1 == 128);
  REQUIRE(s2 == 129);
  print(s1);
  print_nl(s2);
  REQUIRE(ostr.view() == "ab\ncd"sv);
}

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
  append_char('a');
  append_char('b');
  print(make_string());
  REQUIRE(ostr.view() == "ab"sv);
}

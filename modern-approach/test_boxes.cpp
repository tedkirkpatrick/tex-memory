/*
  Test implementation of TeX data structures in modern C++.

  See ../README.md for details of the overall project.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "box_classes.hpp"
#include "catch2.hpp"
#include "sample_lists.hpp"


TEST_CASE("create/delete") {
  boxes::VListPtr v = boxes::new_basic_list();
  REQUIRE(bool(v)); // Simple existence test
}  // v will be deleted at end of scope

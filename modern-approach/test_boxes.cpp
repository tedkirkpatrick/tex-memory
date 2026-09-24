#include "box_classes.hpp"
#include "catch2.hpp"
#include "sample_lists.hpp"


TEST_CASE("create/delete") {
  boxes::VListPtr v = boxes::new_basic_list();
  REQUIRE(bool(v)); // Simple existence test
}  // v will be deleted at end of scope

#include "boxes.hpp"
#include "catch2.hpp"

TEST_CASE("Just testing compile") {
  init_table_entries();
  REQUIRE(type(0) == static_cast<std::byte>(1)); // Not a real type
}

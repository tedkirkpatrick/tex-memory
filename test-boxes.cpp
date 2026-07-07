#include <iostream>

#include "boxes.hpp"
#include "catch2.hpp"
#include "display.hpp"

TEST_CASE("Just testing compile") {
  init_table_entries();
  pointer hl = new_null_box();
  type(hl) = hlist_node;
  pointer p = new_rule();
  short_display(p);
  std::cout << "---- Done output ---\n";
}

#include "box_classes.hpp"

#include "catch2.hpp"

namespace boxes {

  TEST_CASE("Basic hlist create/delete") {
    types::NodePtr zch {new CharNode(nullptr, 'z', 0)};
    types::NodePtr ach {new CharNode(std::move(zch), 'a', 0)};
    types::NodePtr hl {new HList(nullptr, std::move(ach))};
  }

} // namespace boxes

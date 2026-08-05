#include "box_types.hpp"

namespace types {

  int Node::traverse_node() {
    if (m_link != nullptr) {
      return m_link->traverse_node();
    }
    else {
      return 0;
    }
  }

} // namespace types

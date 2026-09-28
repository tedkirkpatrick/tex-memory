/*
  Define the basic classes. All boxes are descended from Nodes. GlueSpecs
  descend from Nodes independently.

  See ../README.md for details of the overall project.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/  
 */

#include "box_types.hpp"

namespace types {

  // Used to test performance of pure traversal of a Node hierarchy
  int Node::traverse_node() {
    if (m_link != nullptr) {
      return m_link->traverse_node();
    }
    else {
      return 0;
    }
  }

} // namespace types

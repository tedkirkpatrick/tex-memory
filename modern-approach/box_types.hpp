#ifndef BOX_TYPES_CPP
#define BOX_TYPES_CPP

#include <memory>

namespace types {

  using scaled = int;
  using glue_ratio = float;

  class Node {
  public:
    Node(std::unique_ptr<Node> link_) : link(std::move(link_)) {};
  private:
    std::unique_ptr<Node> link;
  };
  using NodePtr = std::unique_ptr<Node>;

  enum class GlueSign {
    normal,
    stretching,
    shrinking
  };

} // namespace types

#endif

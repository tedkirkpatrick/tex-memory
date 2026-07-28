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

  enum class TokenType {
    escape,
    relax,
    left_brace,
    right_brace,
    math_shift,
    tab_mark,
    car_ret,
    out_param,
    mac_param,
    sup_mark,
    sub_mark,
    ignore,
    endv,
    spacer,
    letter,
    other_char,
    active_char,
    par_end,
    match,
    comment,
    end_match,
    stop,
    invalid_char,
    delim_num,
    max_char_code,
  };

} // namespace types

#endif

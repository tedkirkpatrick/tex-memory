#ifndef BOX_TYPES_CPP
#define BOX_TYPES_CPP

#include <memory>

namespace types {

  using scaled = int;
  namespace scaled_literals {
    constexpr scaled operator ""_sc(unsigned long long int t_v) { return 0x1'00'00 * scaled(t_v); }
  } // namespace scaled_literals

  using glue_ratio = float;

  class Node {
  public:
    Node(std::unique_ptr<Node> t_link) : m_link(std::move(t_link)) {};
    virtual ~Node() = default;
  private:
    std::unique_ptr<Node> m_link;
  };
  using NodePtr = std::unique_ptr<Node>;

  enum class Font {
    default_font,
  };

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

  // explicit is a C++ keyword so we add a "_kern" suffix to all entries
  enum class KernType {
    normal_kern,
    explicit_kern,
    acc_kern,
  };

  enum class InfinityOrder {
    normal,
    fil, // I didn't come up with these names, people
    fill,
    filll
  };

} // namespace types

#endif

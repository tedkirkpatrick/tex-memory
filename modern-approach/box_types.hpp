#ifndef BOX_TYPES_CPP
#define BOX_TYPES_CPP

/*
  Define the basic classes. All boxes are descended from Nodes. GlueSpecs
  descend from Nodes independently.

  See ../README.md for details of the overall project.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/  
 */

#include <memory>

namespace types {

  using scaled = int;
  namespace scaled_literals {
    constexpr scaled operator ""_sc(unsigned long long int t_v) { return 0x1'00'00 * scaled(t_v); }
  } // namespace scaled_literals
  using namespace scaled_literals;

  using glue_ratio = float;

  // Nodes form a singly-linked list. All boxes are derived from this base.
  class Node {
  public:
    Node(std::unique_ptr<Node> t_link) : m_link(std::move(t_link)) {}
    virtual ~Node() = default;
    virtual int traverse_node(); // Just used for performance testing

  private:
    std::unique_ptr<Node> m_link;
  };
  using NodePtr = std::unique_ptr<Node>;

  enum class InfinityOrder {
    normal,
    fil, // I didn't come up with these names, people
    fill,
    filll
  };

  // A GlueSpec has no links to other objects but is potentially linked *from* several others.
  class GlueSpec {
  public:
    GlueSpec(scaled t_width, scaled t_stretch, InfinityOrder t_stretch_order, scaled t_shrink,
             InfinityOrder t_shrink_order) :
      m_width(t_width), m_stretch(t_stretch), m_shrink(t_shrink),
      m_stretch_o(t_stretch_order), m_shrink_o(t_shrink_order) {}
    scaled get_width() { return m_width; }

  private:
    scaled m_width {0_sc};
    scaled m_stretch {0_sc};
    scaled m_shrink {0_sc};
    InfinityOrder m_stretch_o {InfinityOrder::normal};
    InfinityOrder m_shrink_o {InfinityOrder::normal};
  };
  using GlueSpecPtr = std::shared_ptr<GlueSpec>;

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

} // namespace types

#endif

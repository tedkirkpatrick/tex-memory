#ifndef BOX_CLASSES_HPP
#define BOX_CLASSES_HPP

#include "box_types.hpp"

#include <memory>

namespace boxes {

  class CharNode : public types::Node {
  public:
    CharNode(types::NodePtr t_next, char t_c, int t_font) : types::Node(std::move(t_next)),
                                                         m_font(t_font), m_char {t_c} 
    {};
  private:
    int m_font;
    char m_char;
  };
  using CharNodePtr = types::NodePtr;

  class List : public types::Node {
  public:
    List(types::NodePtr t_next, types::NodePtr t_contents) : types::Node(std::move(t_next)),
                                     m_contents(std::move(t_contents))
                                     {};
  private:
    types::NodePtr m_contents;
    types::scaled m_height {0};
    types::scaled m_width {0};
    types::scaled m_depth {0};
    types::scaled m_shift_amount {0};
    types::glue_ratio m_glue_set {0.0f};
    types::GlueSign m_glue_sign {types::GlueSign::normal};
  };

  class HList : public List {
  public:
    HList(types::NodePtr t_next, types::NodePtr t_contents) :
      List(std::move(t_next), std::move(t_contents)) {};
  };
  using HListPtr = types::NodePtr;

  class Unset : public types::Node {
  public:
    Unset(types::NodePtr t_next, types::NodePtr t_contents) : types::Node(std::move(t_next)),
                                      m_contents(std::move(t_contents))
                                      {};
  private:
    types::NodePtr m_contents {nullptr};
    types::scaled m_height {0};
    types::scaled m_width {0};
    types::scaled m_depth {0};
    types::scaled m_glue_shrink {0};
    types::GlueSign m_glue_sign {types::GlueSign::normal};
    int m_span_count;
  };
  using UnsetPtr = types::NodePtr;

  class Disc : public types::Node {
  public:
    Disc(types::NodePtr t_next, types::NodePtr t_pre_break, types::NodePtr t_post_break) :
      types::Node(std::move(t_next)), m_pre_break(std::move(t_pre_break)), m_post_break(std::move(t_post_break)) {};
  private:
    types::NodePtr m_pre_break {nullptr};
    types::NodePtr m_post_break {nullptr};
    int m_replace_count {0};
  };
  using DiscPtr = types::NodePtr;

  class Token : public types::Node {
  public:
    Token(types::NodePtr t_next, types::TokenType t_tt, int t_val) : types::Node(std::move(t_next)),
                                                                 m_type(t_tt), m_val(t_val) {};
  private:
    types::TokenType m_type {types::TokenType::letter};
    int m_val {0};
  };
  using TokenPtr = types::NodePtr; // So we can pass TokenPtr to NodePtr

  class TokenList : public types::Node {
  public:
    TokenList(types::NodePtr t_next, TokenPtr t_first_token) : types::Node(std::move(t_next)),
                                                                   m_first_token(std::move(t_first_token)) {};
  private:
    TokenPtr m_first_token {nullptr};
  };
  using TokenListPtr = types::NodePtr; // So we can pass TokenListPtr to NodePtr
  
  class Mark : public types::Node {
  public:
    Mark(types::NodePtr t_next, TokenListPtr t_tl) : types::Node(std::move(t_next)),
                                                   m_token_list(std::move(t_tl)) {};
  private:
    TokenListPtr m_token_list {nullptr};
  };
  using MarkPtr = types::NodePtr;

  class Ligature : public types::Node {
  public:
    Ligature(types::NodePtr t_next, int t_font, char t_lig, CharNodePtr t_cc) :
      types::Node(std::move(t_next)), m_component_chars(std::move(t_cc)), m_font(t_font), m_lig(t_lig) {};
  private:
    CharNodePtr m_component_chars {nullptr};
    int m_font {0};
    char m_lig {'a'};
  };
  using LigaturePtr = types::NodePtr;

  class Kern : public types::Node {
  public:
    Kern(types::NodePtr t_next, types::scaled t_width, types::KernType t_type) :
      types::Node(std::move(t_next)), m_width(t_width), m_type(t_type) {}
  private:
    types::scaled m_width {0};
    types::KernType m_type {types::KernType::normal_kern};
  };
  using KernPtr = types::NodePtr;

  class GlueSpec { // A GlueSpec is not a Node because it doesn't have a link field
  public:
    GlueSpec(types::scaled t_width, types::scaled t_stretch, types::InfinityOrder t_stretch_order, types::scaled t_shrink,
             types::InfinityOrder t_shrink_order) : m_width(t_width), m_stretch(t_stretch), m_shrink(t_shrink),
                                                   m_stretch_o(t_stretch_order), m_shrink_o(t_shrink_order) {};
  private:
    types::scaled m_width {0};
    types::scaled m_stretch {0};
    types::scaled m_shrink {0};
    types::InfinityOrder m_stretch_o {types::InfinityOrder::normal};
    types::InfinityOrder m_shrink_o {types::InfinityOrder::normal};
  };
  using GlueSpecPtr = std::shared_ptr<GlueSpec>;

  class Glue : public types::Node {
  public:
    Glue(types::NodePtr t_next, GlueSpecPtr t_gs, types::NodePtr t_leader) :
      types::Node(std::move(t_next)), m_leader(std::move(t_leader)), m_gs(t_gs) {};
  private:
    types::NodePtr m_leader {nullptr};
    GlueSpecPtr m_gs {nullptr};
  };
  using GluePtr = types::NodePtr;

  class WhatsItWrite : public types::Node {
  public:
    WhatsItWrite(types::NodePtr t_next, TokenListPtr t_tl) : types::Node(std::move(t_next)), m_write_tokens(std::move(t_tl)) {}
  private:
    TokenListPtr m_write_tokens {nullptr};
  };
  using WhatsItWritePtr = types::NodePtr;

} // namespace boxes

#endif

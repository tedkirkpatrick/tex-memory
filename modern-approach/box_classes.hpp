#ifndef BOX_CLASSES_HPP
#define BOX_CLASSES_HPP

#include <memory>

#include "box_types.hpp"

namespace boxes {

  class CharNode : public types::Node {
  public:
    CharNode(types::NodePtr next_, char c_, int font_) : types::Node(std::move(next_)),
                                                         font_v(font_), char_v {c_} 
    {};
  private:
    int font_v;
    char char_v;
  };
  using CharNodePtr = types::NodePtr;

  class List : public types::Node {
  public:
    List(types::NodePtr next_, types::NodePtr contents_) : types::Node(std::move(next_)),
                                     contents(std::move(contents_))
                                     {};
  private:
    types::NodePtr contents;
    types::scaled height {0};
    types::scaled width {0};
    types::scaled depth {0};
    types::scaled shift_amount {0};
    types::glue_ratio glue_set {0.0f};
    types::GlueSign glue_sign {types::GlueSign::normal};
  };

  class HList : public List {
  public:
    HList(types::NodePtr next_, types::NodePtr contents_) :
      List(std::move(next_), std::move(contents_)) {};
  };
  using HListPtr = types::NodePtr;

  class Unset : public types::Node {
  public:
    Unset(types::NodePtr next_, types::NodePtr contents_) : types::Node(std::move(next_)),
                                      contents(std::move(contents_))
                                      {};
  private:
    types::NodePtr contents {nullptr};
    types::scaled height {0};
    types::scaled width {0};
    types::scaled depth {0};
    types::scaled glue_shrink {0};
    types::GlueSign glue_sign {types::GlueSign::normal};
    int span_count;
  };
  using UnsetPtr = types::NodePtr;

  class Disc : public types::Node {
  public:
    Disc(types::NodePtr next_, types::NodePtr pre_break, types::NodePtr post_break) :
      types::Node(std::move(next_)), pre_break(std::move(pre_break)), post_break(std::move(post_break)) {};
  private:
    types::NodePtr pre_break {nullptr};
    types::NodePtr post_break {nullptr};
    int replace_count {0};
  };
  using DiscPtr = types::NodePtr;

  class Token : public types::Node {
  public:
    Token(types::NodePtr next_, types::TokenType tt_, int val_) : types::Node(std::move(next_)),
                                                                 type(tt_), val(val_) {};
  private:
    types::TokenType type {types::TokenType::letter};
    int val {0};
  };
  using TokenPtr = types::NodePtr; // So we can pass TokenPtr to NodePtr

  class TokenList : public types::Node {
  public:
    TokenList(types::NodePtr next_, TokenPtr first_token_) : types::Node(std::move(next_)),
                                                                   first_token(std::move(first_token_)) {};
  private:
    TokenPtr first_token {nullptr};
  };
  using TokenListPtr = types::NodePtr; // So we can pass TokenListPtr to NodePtr
  
  class Mark : public types::Node {
  public:
    Mark(types::NodePtr next_, TokenListPtr tl_) : types::Node(std::move(next_)),
                                                   token_list(std::move(tl_)) {};
  private:
    TokenListPtr token_list {nullptr};
  };
  using MarkPtr = types::NodePtr;

  class Ligature : public types::Node {
  public:
    Ligature(types::NodePtr next_, int font_, char lig_, CharNodePtr cc_) :
      types::Node(std::move(next_)), component_chars(std::move(cc_)), font(font_), lig(lig_) {};
  private:
    CharNodePtr component_chars {nullptr};
    int font {0};
    char lig {'a'};
  };
  using LigaturePtr = types::NodePtr;

  class Kern : public types::Node {
  public:
    Kern(types::NodePtr next_, types::scaled width_, types::KernType type_) :
      types::Node(std::move(next_)), width(width_), type(type_) {}
  private:
    types::scaled width {0};
    types::KernType type {types::KernType::normal_kern};
  };
  using KernPtr = types::NodePtr;

  class GlueSpec { // A GlueSpec is not a Node because it doesn't have a link field
  public:
    GlueSpec(types::scaled width_, types::scaled stretch_, types::InfinityOrder stretch_order_, types::scaled shrink_,
             types::InfinityOrder shrink_order_) : width(width_), stretch(stretch_), shrink(shrink_),
                                                   stretch_o(stretch_order_), shrink_o(shrink_order_) {};
  private:
    types::scaled width {0};
    types::scaled stretch {0};
    types::scaled shrink {0};
    types::InfinityOrder stretch_o {types::InfinityOrder::normal};
    types::InfinityOrder shrink_o {types::InfinityOrder::normal};
  };
  using GlueSpecPtr = std::shared_ptr<GlueSpec>;

  class Glue : public types::Node {
  public:
    Glue(types::NodePtr next_, GlueSpecPtr gs_, types::NodePtr leader_) :
      types::Node(std::move(next_)), leader(std::move(leader_)), gs(gs_) {};
  private:
    types::NodePtr leader {nullptr};
    GlueSpecPtr gs {nullptr};
  };
  using GluePtr = types::NodePtr;

} // namespace boxes

#endif

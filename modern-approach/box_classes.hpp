#ifndef BOX_CLASSES_HPP
#define BOX_CLASSES_HPP

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

  class Disc : public types::Node {
  public:
    Disc(types::NodePtr next_, types::NodePtr pre_break, types::NodePtr post_break) :
      types::Node(std::move(next_)), pre_break(std::move(pre_break)), post_break(std::move(post_break)) {};
  private:
    types::NodePtr pre_break {nullptr};
    types::NodePtr post_break {nullptr};
    int replace_count {0};
  };

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

} // namespace boxes

#endif

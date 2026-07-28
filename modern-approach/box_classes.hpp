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

} // namespace boxes

#endif

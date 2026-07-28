#ifndef BOX_CLASSES_HPP
#define BOX_CLASSES_HPP

#include "box_types.hpp"

namespace boxes {

  class List : public types::Node {
  public:
    List(types::NodePtr contents_) : types::Node(nullptr),
                                     contents(std::move(contents_)), height {0}, width {0},
                                     depth {0}, shift_amount {0}, glue_set {0.0f},
                                     glue_sign {types::GlueSign::normal}
                                     {};
  private:
    types::NodePtr contents;
    types::scaled height;
    types::scaled width;
    types::scaled depth;
    types::scaled shift_amount;
    types::glue_ratio glue_set;
    types::GlueSign glue_sign;
  };

  class Unset : public types::Node {
  public:
    Unset(types::NodePtr contents_) : types::Node(nullptr),
                                      contents(std::move(contents_)),
                                      height {0}, width {0},
                                      depth {0}, glue_shrink {0}, glue_sign {types::GlueSign::normal},
                                      span_count {0}
                                      {};
  private:
    types::NodePtr contents;
    types::scaled height;
    types::scaled width;
    types::scaled depth;
    types::scaled glue_shrink;
    types::GlueSign glue_sign;
    int span_count;
  };

} // namespace boxes

#endif

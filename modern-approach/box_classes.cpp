#include "box_classes.hpp"

#include "benchmark_utils.hpp"
// Enable benchmarking code in this translation unit
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2.hpp"

namespace boxes {

  // These routines are only called in benchmarks

  int List::traverse_node() {
    int v = m_height;
    if (m_contents != nullptr) {
      m_contents->traverse_node();
    }
    Node::traverse_node();
    return v;
  }

  int Unset::traverse_node() {
    int v = m_height;
    if (m_contents != nullptr) {
      m_contents->traverse_node();
    }
    Node::traverse_node();
    return v;
  }

  int Disc::traverse_node() {
    int v = m_replace_count;
    if (m_pre_break != nullptr) {
      m_pre_break->traverse_node();
    }
    if (m_post_break != nullptr) {
      m_post_break->traverse_node();
    }
    Node::traverse_node();
    return v;
  }

  int TokenList::traverse_node() {
    if (m_first_token != nullptr) {
      m_first_token->traverse_node();
    }
    return Node::traverse_node();
  }

  int Mark::traverse_node() {
    if (m_token_list != nullptr) {
      m_token_list->traverse_node();
    }
    return Node::traverse_node();
  }

  int Ligature::traverse_node() {
    int v = m_font;
    if (m_component_chars != nullptr) {
      m_component_chars->traverse_node();
    }
    Node::traverse_node();
    return v;
  }

  int Glue::traverse_node() {
    int v = 0;
    if (m_gs != nullptr) {
      v = m_gs->get_width();
    }
    if (m_leader != nullptr) {
      m_leader->traverse_node();
    }
    Node::traverse_node();
    return v;
  }
  
  int WhatsItWrite::traverse_node() {
    if (m_write_tokens != nullptr) {
      m_write_tokens->traverse_node();
    }
    return Node::traverse_node();
  }

  int Ins::traverse_node() {
    int v = 0;
    if (m_gs != nullptr) {
      v = m_gs->get_width();
    }
    if (m_list != nullptr) {
      m_list->traverse_node();
    }
    Node::traverse_node();
    return v;
  }
} // namespace boxes

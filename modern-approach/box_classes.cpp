#include "box_classes.hpp"

// Enable benchmarking code in this translation unit
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch2.hpp"

namespace boxes {

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

  namespace {
    using namespace types::scaled_literals;

    // Build a vlist semantically equivalent to new_basic_list() in original
    VListPtr new_basic_list () {
      DiscPtr d {new Disc(nullptr, nullptr, nullptr)};

      TokenPtr tz {new Token(nullptr, types::TokenType::letter, 'z')};
      TokenPtr ta {new Token(std::move(tz), types::TokenType::letter, 'a')};
      TokenListPtr tl {new TokenList(nullptr, std::move(ta))};
      MarkPtr m {new Mark(std::move(d), std::move(tl))};

      CharNodePtr i {new CharNode(nullptr, 'i', types::Font::default_font)};
      CharNodePtr f {new CharNode(std::move(i), 'f', types::Font::default_font)};
      LigaturePtr lig {new Ligature(std::move(m), 0, 'F', std::move(f))};

      KernPtr k {new Kern(std::move(lig), 0_sc, types::KernType::normal_kern)};

      types::GlueSpecPtr gs {std::make_shared<types::GlueSpec>(2_sc,
                                   1_sc, types::InfinityOrder::normal,
                                   2_sc, types::InfinityOrder::fil)};
      HListPtr leader {new HList(nullptr, nullptr)};
      GluePtr g {new Glue(std::move(k), gs, std::move(leader))};

      TokenPtr tr {new Token(nullptr, types::TokenType::letter, 'r')};
      TokenPtr tq {new Token(std::move(tr), types::TokenType::letter, 'q')};
      TokenListPtr tlw {new TokenList(nullptr, std::move(tq))};
      WhatsItWritePtr w {new WhatsItWrite(std::move(g), std::move(tlw))};

      types::GlueSpecPtr ins_gs {std::make_shared<types::GlueSpec>(1_sc,
                                   1_sc, types::InfinityOrder::fill,
                                   2_sc, types::InfinityOrder::filll)};
      HListPtr ins_l {new HList(nullptr, nullptr)};
      InsPtr ins {new Ins(std::move(w), ins_gs, 4, std::move(ins_l))};

      RulePtr rule {new Rule(std::move(ins), 1_sc, 3_sc, 0_sc)};

      CharNodePtr bch {new CharNode(nullptr, 'b', types::Font::default_font)};
      CharNodePtr ach {new CharNode(std::move(bch), 'a', types::Font::default_font)};
      HListPtr hl {new HList(std::move(rule), std::move(ach))};

      VListPtr vl {new VList(nullptr, std::move(hl))};
      return vl;
    }
  } // unnamed namespace

  TEST_CASE("Benchmarks") {
    BENCHMARK("Basic vlist create/traverse/delete") {
      VListPtr v = new_basic_list();
      for (int i = 0; i < 1000; i++) {
        v->traverse_node();
      }
    }; // v will be deleted at end of scope for each run
  }
} // namespace boxes

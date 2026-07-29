#include "box_classes.hpp"

// Enable benchmarking code in this translation unit
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch2.hpp"

namespace boxes {

  namespace {
    using namespace types::scaled_literals;

    // Build a vlist semantically equivalent to new_basic_list() in original
    void new_basic_list () {
      DiscPtr d {new Disc(nullptr, nullptr, nullptr)};

      TokenPtr tz {new Token(nullptr, types::TokenType::letter, 'z')};
      TokenPtr ta {new Token(std::move(tz), types::TokenType::letter, 'a')};
      TokenListPtr tl {new TokenList(nullptr, std::move(ta))};
      MarkPtr m {new Mark(std::move(d), std::move(tl))};

      CharNodePtr i {new CharNode(nullptr, 'i', types::Font::default_font)};
      CharNodePtr f {new CharNode(std::move(i), 'f', types::Font::default_font)};
      LigaturePtr lig {new Ligature(std::move(m), 0, 'F', std::move(f))};

      KernPtr k {new Kern(std::move(lig), 0_sc, types::KernType::normal_kern)};

      GlueSpecPtr gs {std::make_shared<GlueSpec>(2_sc,
                                   1_sc, types::InfinityOrder::normal,
                                   2_sc, types::InfinityOrder::fil)};
      HListPtr leader {new HList(nullptr, nullptr)};
      GluePtr g {new Glue(std::move(k), gs, std::move(leader))};

      TokenPtr tr {new Token(nullptr, types::TokenType::letter, 'r')};
      TokenPtr tq {new Token(std::move(tr), types::TokenType::letter, 'q')};
      TokenListPtr tlw {new TokenList(nullptr, std::move(tq))};
      WhatsItWritePtr w {new WhatsItWrite(std::move(g), std::move(tlw))};

      GlueSpecPtr ins_gs {std::make_shared<GlueSpec>(1_sc,
                                   1_sc, types::InfinityOrder::fill,
                                   2_sc, types::InfinityOrder::filll)};
      HListPtr ins_l {new HList(nullptr, nullptr)};
      InsPtr ins {new Ins(std::move(w), ins_gs, 4, std::move(ins_l))};

      RulePtr rule {new Rule(std::move(ins), 1_sc, 3_sc, 0_sc)};

      CharNodePtr bch {new CharNode(nullptr, 'b', types::Font::default_font)};
      CharNodePtr ach {new CharNode(std::move(bch), 'a', types::Font::default_font)};
      HListPtr hl {new HList(std::move(rule), std::move(ach))};

      VListPtr vl {new VList(nullptr, std::move(hl))};
    }
  } // unnamed namespace

  TEST_CASE("Benchmarks") {
    BENCHMARK("Basic hlist create/delete") {
      new_basic_list();
    };
  }
} // namespace boxes

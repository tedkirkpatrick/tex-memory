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

      CharNodePtr i {new CharNode(nullptr, 'i', 0)};
      CharNodePtr f {new CharNode(std::move(i), 'f', 0)};
      LigaturePtr lig {new Ligature(std::move(m), 0, 'F', std::move(f))};

      KernPtr k {new Kern(std::move(lig), 0, types::KernType::normal_kern)};

      GlueSpecPtr gs {std::make_shared<GlueSpec>(2_sc,
                                   1_sc, types::InfinityOrder::normal,
                                   2_sc, types::InfinityOrder::fil)};
      HListPtr leader {new HList(nullptr, nullptr)};
      GluePtr g {new Glue(std::move(k), gs, std::move(leader))};

      TokenPtr tr {new Token(nullptr, types::TokenType::letter, 'r')};
      TokenPtr tq {new Token(std::move(tr), types::TokenType::letter, 'q')};
      TokenListPtr tlw {new TokenList(nullptr, std::move(tq))};
      WhatsItWritePtr w {new WhatsItWrite(std::move(g), std::move(tlw))};

      CharNodePtr zch {new CharNode(nullptr, 'z', 0)};
      CharNodePtr ach {new CharNode(std::move(zch), 'a', 0)};
      HListPtr hl {new HList(nullptr, std::move(ach))};
    }
  } // unnamed namespace

  TEST_CASE("Benchmarks") {
    BENCHMARK("Basic hlist create/delete") {
      new_basic_list();
    };
  }
} // namespace boxes

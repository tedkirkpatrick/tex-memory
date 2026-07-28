#include "box_classes.hpp"

// Enable benchmarking code in this routine
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch2.hpp"

namespace boxes {

  namespace {
    // Build a vlist semantically equivalent to new_basic_list() in original
    void new_basic_list () {
      types::NodePtr d {new Disc(nullptr, nullptr, nullptr)};
      TokenPtr tz {new Token(nullptr, types::TokenType::letter, 'z')};
      TokenPtr az {new Token(std::move(tz), types::TokenType::letter, 'a')};
      TokenListPtr tl {new TokenList(nullptr, std::move(az))};
      Mark m {std::move(d), std::move(tl)};
      types::NodePtr zch {new CharNode(nullptr, 'z', 0)};
      types::NodePtr ach {new CharNode(std::move(zch), 'a', 0)};
      types::NodePtr hl {new HList(nullptr, std::move(ach))};
    }
  } // unnamed namespace

  TEST_CASE("Benchmarks") {
    BENCHMARK("Basic hlist create/delete") {
      new_basic_list();
    };
  }
} // namespace boxes

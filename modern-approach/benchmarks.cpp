/*
  Benchmark the modern style of TeX memory management.

  See ../README.md for details of the overall project.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "benchmark_utils.hpp"
#include "box_classes.hpp"
// Enable benchmarking code in this translation unit
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2.hpp"
#include "sample_lists.hpp"

TEST_CASE("Benchmarks") {
  BENCHMARK(benchmark_utils::create_benchmark_name("Basic vlist create/delete",
                                                   "modern",
                                                   0,
                                                   OPT_LEVEL, // Preprocessor symbol set on command line
                                                   std::chrono::utc_clock::now()))
    {
      boxes::VListPtr v = boxes::new_basic_list();
    }; // v will be deleted at end of scope for each run
  BENCHMARK(benchmark_utils::create_benchmark_name("Basic vlist create/traverse/delete",
                                                   "modern",
                                                   benchmark_utils::traversals,
                                                   OPT_LEVEL, // Preprocessor symbol set on command line
                                                   std::chrono::utc_clock::now()))
    {
      boxes::VListPtr v = boxes::new_basic_list();
      for (int i = 0; i < benchmark_utils::traversals; i++) {
        v->traverse_node();
      }
    }; // v will be deleted at end of scope for each run
}

/*
  Benchmark the original style of TeX memory manageent
  Does not correspond to any code in TeX.

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines are NOT transliterations of TeX code. They are purpose-written
  for this demonstration. They are written in modern C++.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <chrono>

#include "benchmark_utils.hpp"
// Enable benchmarking code in this file
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2.hpp"
#include "destroying_boxes.hpp"
#include "sample_lists.hpp"
#include "string_handling.hpp"
#include "traverse.hpp"

TEST_CASE("Run benchmarks") {
  init_table_entries();
  init_eqtb();
  str_start.reset_strings();

  BENCHMARK(benchmark_utils::create_benchmark_name("Basic vlist create/delete",
                                                   "original",
                                                   0,
                                                   OPT_LEVEL, // Preprocessor symbol set on command line
                                                   std::chrono::utc_clock::now()))
  {
    pointer nbl = new_basic_list();
    flush_node_list(nbl);
  };

  BENCHMARK(benchmark_utils::create_benchmark_name("Basic vlist create/traverse/delete",
                                                   "original",
                                                   benchmark_utils::traversals,
                                                   OPT_LEVEL, // Preprocessor symbol set on command line
                                                   std::chrono::utc_clock::now()))
  {
    pointer nbl = new_basic_list();
    for (int v = 0; v < benchmark_utils::traversals; v++) {
      traverse_node(nbl);
    }
    flush_node_list(nbl);
  };
}

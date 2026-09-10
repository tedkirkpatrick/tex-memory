/*
  Benchmark the original style of TeX memory manageent
 */

// Enable benchmarking code in this routine
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

  BENCHMARK("Basic vlist create/delete") {
    pointer nbl = new_basic_list();
    flush_node_list(nbl);
  };
  BENCHMARK("Basic vlist create/traverse/delete") {
    pointer nbl = new_basic_list();
    for (int v = 0; v < 1000; v++) {
      traverse_node(nbl);
    }
    flush_node_list(nbl);
  };
}

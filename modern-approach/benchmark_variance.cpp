/*
  Benchmark with a specified level of consistency.

  Used for exploring the several measures of spread computed by Catch2 benchmarks.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
*/

#include <chrono>
#include <thread>
using namespace std::chrono_literals;


#include "benchmark_utils.hpp"
// Enable benchmarking code in this translation unit
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2.hpp"

TEST_CASE("Variance-test") {
  BENCHMARK(benchmark_utils::create_benchmark_name("Highly consistent wait 100ms",
                                                   "modern",
                                                   0,
                                                   OPT_LEVEL,
                                                   benchmark_utils::externals,
                                                   std::chrono::utc_clock::now()))
    {
      static unsigned count {0};
      auto wait {100ms};
      /*
        if (count < 1) {
        wait = 750ms;
        }
        else if (count < 5) {
        wait = 200ms;
        }
      */
      std::this_thread::sleep_for(wait);
      ++count;
    };
}

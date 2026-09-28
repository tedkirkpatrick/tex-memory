/*
  Common utilities for benchmarking.

  This code is in the public domain.
  See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "benchmark_utils.hpp"

#include <format>

namespace benchmark_utils {
  constinit int traversals {1};
  std::string create_benchmark_name(std::string_view name, std::string_view code, int traversals,
                                    std::string_view optimization, std::chrono::utc_clock::time_point utc) {
    return std::format("{}, code='{}', traversals={}, optimization='{}', UTC='{:%F %T}'",
                       name, code, traversals, optimization, utc);
  }
} // namespace benchmark_utils

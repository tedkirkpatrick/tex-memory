/*
  Common utilities for benchmarking.

  This code is in the public domain.
  See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "benchmark_utils.hpp"

#include <format>
#include <stdexcept>

#include "catch2.hpp"

namespace benchmark_utils {
  namespace {
    bool contains_delimiter(std::string_view s) {
      auto is_delimiter = [] (char c) { return c == '|' || c == ','; };
      return std::find_if(s.begin(), s.end(), is_delimiter) != s.end();
    }
    bool check_parameter(std::string_view s, std::string_view name) {
      if (contains_delimiter(s)) {
        throw std::invalid_argument(std::format("Parameter {} contains a delimiter ('|' or ','): {}", name, s));
      }
      return true;
    }
  } // unnamed namespace
  constinit int traversals {1};
  std::string externals {};
  std::string create_benchmark_name(std::string_view name, std::string_view code, int traversals,
                                    std::string_view optimization, std::string_view externals,
                                    std::chrono::utc_clock::time_point utc) {
    REQUIRE(check_parameter(name, "name"));
    REQUIRE(check_parameter(code, "code"));
    REQUIRE(check_parameter(externals, "externals"));

    return std::format("{}| code='{}'| traversals={}| optimization='{}'| externals='{}'| UTC='{:%F %T}'",
                       name, code, traversals, optimization, externals, utc);
  }
} // namespace benchmark_utils

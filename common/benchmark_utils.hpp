#ifndef BENCHMARK_UTILS_HPP
#define BENCHMARK_UTILS_HPP

#include <chrono>
#include <string>
#include <string_view>

namespace benchmark_utils {
  extern constinit int traversals;
  std::string create_benchmark_name(std::string_view name, std::string_view code, int traversals,
                                    std::string_view optimization, std::chrono::utc_clock::time_point utc);
} // namespace benchmark_utils

#endif

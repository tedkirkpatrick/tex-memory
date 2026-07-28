#include "reporting_errors.hpp"

#include <format>
#include <sstream>
#include <stdexcept>
#include <string_view>


// Section 94

// This implementation uses C++ exceptions rather than the internal TeX error handlers

void overflow(std::string_view s, int n) {
  throw std::runtime_error(std::format("TeX capacity exceeded, Sorry [{}={}].  Exiting", s, n));
}

// Section 95

void confusion(std::string_view s) {
  throw std::runtime_error(std::format("Internal error: {}", s));
}

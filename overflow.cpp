#include "overflow.hpp"

#include <format>
#include <sstream>
#include <stdexcept>
#include <string_view>


// Section 118

// This implementation uses C++ exceptions rather than the internal TeX error handlers

std::stringstream stream;

void overflow(std::string_view s, int n) {
  std::println(stream, "TeX capacity exceeded, Sorry [{}={}].  Exiting", s, n);
  throw std::runtime_error(stream.str());
}

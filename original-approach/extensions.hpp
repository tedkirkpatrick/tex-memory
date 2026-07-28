#ifndef EXTENSIONS_HPP
#define EXTENSIONS_HPP

#include <cstddef>

#include "basic_memory.hpp"

// Section 1341

constexpr halfword write_node_size = 2;
constexpr halfword open_node_size = 3;

constexpr std::byte open_node {0};
constexpr std::byte write_node {1};
constexpr std::byte close_node {2};
constexpr std::byte special_node {3};

constexpr halfword& write_tokens(pointer p) { return link(p + 1); }

#endif

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <cstddef>

#include "basic_memory.hpp"

// Section 173

extern std::byte font_in_short_display;

// Section 174

extern void short_display(int p);

// Section 182

extern void show_node_list(pointer p);

// Section 198

extern void show_box(pointer p);

#endif

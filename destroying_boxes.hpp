#ifndef DESTROYING_BOXES_HPP
#define DESTROYING_BOXES_HPP

#include "basic_memory.hpp"
#include "boxes.hpp"

// Section 200

constexpr halfword& token_ref_count(pointer p) { return info(p); }
constexpr void delete_token_ref(pointer p) {
  if (token_ref_count(p) == null) {
    std::println("delete_token_ref of {}", p);
    flush_list(p);
  }
  else
    token_ref_count(p)--;
}

// Section 201

constexpr void fast_delete_glue_ref(pointer p) {
  if (glue_ref_count(p) == null)
    free_node(p, glue_spec_size);
  else
    decr(glue_ref_count(p));
}

extern void delete_glue_ref(pointer p);

// Section 202

extern void flush_node_list(pointer p);

#endif

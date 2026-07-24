#include "destroying_boxes.hpp"

#include "boxes.hpp"
#include "extensions.hpp"
#include "reporting_errors.hpp"

// For debugging
#include <print>

// Section 201

void delete_glue_ref(pointer p) { fast_delete_glue_ref(p); }

// Section 202

void flush_node_list(pointer p) {
  pointer q;
  while (p != null) {
    q = link(p);
    if (p == 957) {
      std::println("*** flush_node_list on 957");
    }
    if (is_char_node(p)) {
      if (p == 957) {
        std::println("*** flush_node_list on 957---free_avail, is_char_node {}", is_char_node(p));
      }
      free_avail(p);
    }
    else {
      if (p == 957) {
        std::println("*** flush_node_list on 957---node type {}", int(type(p)));
      }
      switch(type(p)) {
      case hlist_node:
      case vlist_node:
      case unset_node:
        flush_node_list(list_ptr(p));
        free_node(p, box_node_size);
        goto done;
      case rule_node:
        free_node(p, rule_node_size);
        goto done;
      case ins_node:
        flush_node_list(ins_ptr(p));
        delete_glue_ref(split_top_ptr(p));
        free_node(p, ins_node_size);
        goto done;
      case whatsit_node:
        // Begin Section 1358
        switch (subtype(p)) {
        case open_node:
          free_node(p, open_node_size);
          break;
        case write_node:
        case special_node:
          std::println("^^^^ Deleting whatsit write_node at {}, write_tokens {}", p, write_tokens(p));
          delete_token_ref(write_tokens(p));
          free_node(p, write_node_size);
          goto done;
        case close_node:
          free_node(p, small_node_size);
          break;
        default:
          confusion("ext3");
          break;
        }
        goto done;
        // End Section 1358
       case glue_node:
        fast_delete_glue_ref(glue_ptr(p));
        if (leader_ptr(p) != null)
          flush_node_list(leader_ptr(p));
        break;
      case kern_node:
      case math_node:
      case penalty_node:
        break;
      case ligature_node:
        flush_node_list(lig_ptr(p));
        break;
      case mark_node:
        std::println("&&&& Deleting mark_node at {}, mark_ptr {}", p, mark_ptr(p));
        delete_token_ref(mark_ptr(p));
        break;
      case disc_node:
        flush_node_list(pre_break(p));
        flush_node_list(post_break(p));
        break;
      case adjust_node:
        flush_node_list(adjust_ptr(p));
        break;
      // cases in 698 not implemented
      default:
        confusion("Unknown node type, exiting");
        break;
      }
      free_node(p, small_node_size);
    }
  done:
    p = q;
  }
}

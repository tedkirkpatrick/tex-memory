/*
  A function to touch an element of every node linked from pointer p.
  Logic based on node_display() except we don't display, we simply
  grab an integer value and recurse down into any linked nodes.
 */


#include "traverse.hpp"

#include <cassert>

#include "basic_memory.hpp"
#include "boxes.hpp"

int traverse_node(pointer p) {
  int v {0};

  while (p != null) {
    if (is_char_node(p)) {
      v = int(font(p));
    }
    else switch (type(p)) {
      case hlist_node:
      case vlist_node:
        v = glue_set(p);
        traverse_node(list_ptr(p));
        break;
      case unset_node:
        v = int(span_count(p));
        traverse_node(list_ptr(p));
        break;
      case rule_node:
        v = height(p);
        break;
      case ins_node:
        v = height(p);
        traverse_node(ins_ptr(p));
        break;
      case whatsit_node:
        break;
      case glue_node:
        if (int(subtype(p)) >= int(a_leaders)) {
          v = width(glue_ptr(p));
          traverse_node(leader_ptr(p));
        }
        else {
          if (subtype(p) != cond_math_glue) {
            v = width(glue_ptr(p));
          }
        }
        break;
      case kern_node:
        v = width(p);
        break;
      case math_node:
        v = width(p);
        break;
      case ligature_node:
        v = int(font(lig_char(p)));
        traverse_node(lig_ptr(p));
        break;
      case penalty_node:
        v = int(penalty(p));
        break;
      case disc_node:
        v = int(replace_count(p));
        traverse_node(pre_break(p));
        traverse_node(post_break(p));
        break;
      case mark_node:
        v = traverse_node(link(mark_ptr(p)));
        break;
      case adjust_node:
        v = traverse_node(adjust_ptr(p));
        break;
      default:
        assert(false);
        break;
      }
    p = link(p);
  }
  return v;
}

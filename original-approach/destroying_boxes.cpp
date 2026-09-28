/*
  Destroying boxes.
  Part 13 of TeX: The Program

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines adopt TeX's Pascal style as closely as possible in C++. The primary differnce
  from TeX is that WEB macros are all implemented via inline constexpr constants or functions,
  which retain type safety, rather than C-style preprocessor macros.

  As a result, this code does not conform to typical C++ best practices. For example,
  namespaces are only used for testing and debugging functions, variables are declared
  without initialization, and other good practices are not followed.

  Function subsections that in WEB are separated out are here embedded in the containing function.

  COPYRIGHT
  The code in this file is a transliteration of the original Pascal TeX routines into a C-style C++.
  The file tex.web containing the original code features the following notice:

      This program is copyright (C) 1982 by D. E. Knuth; all rights are reserved.
      Unlimited copying and redistribution of this file are permitted as long
      as this file is not modified. Modifications are permitted, but only if
      the resulting file is not named tex.web

   This modification of the original file is public domain.
   See See https://creativecommons.org/publicdomain/zero/1.0/

   The copyright of the original versions remains.
 */

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
    if (is_char_node(p)) {
      free_avail(p);
    }
    else {
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

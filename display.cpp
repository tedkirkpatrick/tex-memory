#include "display.hpp"

#include <iostream>
#include <print>

#include "boxes.hpp"
#include "hash.hpp"
#include "print.hpp"

// Section 173

std::byte font_in_short_display;

// Section 174

void short_display(int p) {
  while (p > null) {
    if (is_char_node(p)) {
      if (p <= mem_end) {
        if (font(p) != font_in_short_display) {
          if (font(p) < std::byte(font_base) || font(p) > std::byte(font_max))
            std::cout << '*';
          else {
            // Begin Section 267
            print_esc(font_id_text(font(p)));
            // End Section 267
          }
          std::cout << ' ';
          font_in_short_display = font(p);
        }
        std::cout << char(character(p)); // Removed call to qo because it caused constexpr error
      }
    }
    else {
      halfword n; // Defined at procedure block in TeX but only used in this block
      // Begin Section 175
      switch (type(p))
        {
          case hlist_node:
          case vlist_node:
          case ins_node:
          case whatsit_node:
          case mark_node:
          case adjust_node:
          case unset_node:
            std::cout << "[]";
            break;
          case rule_node:
            std::cout << '|';
            break;
          case glue_node:
            if (glue_ptr(p) != zero_glue)
              std::cout << ' ';
            break;
          case math_node:
            std::cout << '$';
            break;
          case ligature_node:
            short_display(lig_ptr(p));
            break;
          case disc_node:
            short_display(pre_break(p));
            short_display(post_break(p));
            n = halfword(replace_count(p));
            while (n > 0) {
              if (link(p) != null)
                p = link(p);
              decr(n);
            }
            break;
          default:
            break;
        }
      // End Section 175
    }
    p = link(p);
  }
}

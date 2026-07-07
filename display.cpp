#include "display.hpp"

#include <iostream>
#include <print>

#include "boxes.hpp"
#include "hash.hpp"
#include "print.hpp"
#include "string_handling.hpp"
#include "token_list.hpp"

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

// Section 176

static void print_font_and_char(int p) {
  if (p > mem_end)
    print_esc("CLOBBERED.");
  else {
    if (font(p) < std::byte(font_base) || font(p) > std::byte(font_max))
      std::cout << '*';
    else {
      // Begin Section 267
      print_esc(font_id_text(font(p)));
      // End Section 267
      std::cout << ' ';
      std::cout << char(character(p)); // Removed call to qo because it caused constexpr error      
    }
  }
}

static void print_mark(int p) {
  std::cout << '{';
  if (p < hi_mem_min || p > mem_end)
    print_esc("CLOBBERED.");
  else {
    show_token_list(link(p), null, max_print_line - 10);
    std::cout << '}';
  }
}

// Section 181

static int depth_threshold;
static int breadth_max;

// Section 182

void show_node_list(pointer p) {
  int n;
  float g;
  if (cur_length() > depth_threshold) {
    if (p > null)
      print(" []");
    return;
  }
  n = 0;
  while (p > null) {
    std::cout << '\n';
    print_current_string();
    if (p > mem_end) {
      print("Bad link, display aborted.");
      return;
    }
    n++;
    if (n > breadth_max) {
      print("etc.");
      return;
    }
    // Begin Section 183
    if (is_char_node(p))
      print_font_and_char(p);
    else switch (type(p)) {
      case hlist_node:
      case vlist_node:
      case unset_node:
        // Begin Section 184
        // ...
        // Begin Section 186
        g = float(glue_set(p));
        if (g != 0.0F && glue_sign(p) != normal) {
          print(", glue set ");
          // MORE ...
        }
        // End Section 186
        // ...
        // End Section 184
        break;
        //...
      case mark_node:
        // Begin Section 196
        {
          print_esc("mark");
          print_mark(mark_ptr(p));
        }
        // End Section 196
        break;
      case adjust_node:
        // Begin Section 197
        // End Section 197
        break;
        // Begin Section 690
        // End Section 690
      default:
        print("Uknown node type!");
        break;
      }
    // End Section 183
    p = link(p);
  }
}

#include "display.hpp"

#include <cmath>
#include <iostream>
#include <print>

#include "basic-memory.hpp"
#include "boxes.hpp"
#include "eqtb.hpp"
#include "extensions.hpp"
#include "hash.hpp"
#include "printing.hpp"
#include "string_handling.hpp"
#include "token_list.hpp"

using std::cout, std::flush;

// Section 173

std::byte font_in_short_display;

// Section 174

void short_display(int p) {
  while (p > null) {
    if (is_char_node(p)) {
      if (p <= mem_end) {
        if (font(p) != font_in_short_display) {
          if (font(p) < std::byte(font_base) || font(p) > std::byte(font_max))
            print_char('*');
          else {
            // Begin Section 267
            print_esc(font_id_text(font(p)));
            // End Section 267
          }
          print_char(' ');
          font_in_short_display = font(p);
        }
        print_ASCII(int(character(p))); // Removed call to qo because it caused constexpr error
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
            print("[]");
            break;
          case rule_node:
            print_char('|');
            break;
          case glue_node:
            if (glue_ptr(p) != zero_glue)
              print_char(' ');
            break;
          case math_node:
            print_char('$');
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
      print_char('*');
    else {
      // Begin Section 267
      print_esc(font_id_text(font(p)));
      // End Section 267
      print_char(' ');
      print_ASCII(int(character(p))); // Removed call to qo because it caused constexpr error      
    }
  }
}

static void print_mark(int p) {
  print_char('{');
  if (p < hi_mem_min || p > mem_end)
    print_esc("CLOBBERED.");
  else
    show_token_list(link(p), null, max_print_line - 10);
  print_char('}');
}

static void print_rule_dimen(scaled d) {
  if (is_running(d))
    print_char('*');
  else
    print_scaled(d);
}

// Section 177

static void print_glue(scaled d, std::byte order, str_number s) {
  print_scaled(d);
  if (order < normal || order > fill)
    print("foul");
  else if (order > normal) {
    print("fil");
    while (order > fil) {
      print_char('l');
      order = std::byte(int(order) - 1);
    }
  }
  else if (s != 0)
    print(s);
}

// Section 178

static void print_spec(pointer p, str_number s) {
  if (p < mem_min || p >= lo_mem_max)
    print_char('*');
  else {
     print_scaled(width(p));
    if (s != 0)
      print(s);
    if (stretch(p) != 0) {
      print(" plus ");
      print_glue(stretch(p), stretch_order(p), s);
    }
    if (shrink(p) != 0) {
      print(" minus ");
      print_glue(shrink(p), shrink_order(p), s);
    }
  }
}

// Section 180

static void node_list_display(pointer p) {
  append_char('.');
  show_node_list(p);
  flush_char();
}

// Section 181

static int depth_threshold;
static int breadth_max;

// Section 182

// The style of this function is the worst outcome of the
// style choice to embed WEB subsection macros directly
// into their parent code rather than make them separate
// functions.
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
    print_ln();
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
        if (type(p) == hlist_node)
          print_esc("h");
        else if (type(p) == vlist_node)
          print_esc("v");
        else
          print_esc("unset");
        print("box(");
        print_scaled(height(p));
        print_char('+');
        print_scaled(depth(p));
        print(")x");
        print_scaled(width(p));
        if (type(p) == unset_node) {
          // Begin Section 185
          if (span_count(p) != min_quarterword) {
            print(" (");
            print_int(int(span_count(p)) + 1); // Eliminated qo call to remove type error
            print(" columns)");
          }
          if (glue_stretch(p) != 0) {
            print(", stretch ");
            print_glue(glue_stretch(p), glue_order(p), 0);
          }
          if (glue_shrink(p) != 0) {
            print(", shrink ");
            print_glue(glue_shrink(p), glue_sign(p), 0);
          }
          // End Section 185
        }
        else {
          // Begin Section 186
          g = float(glue_set(p));
          if (g != 0.0F && glue_sign(p) != normal) {
            print(", glue set ");
            if (glue_sign(p) == shrinking)
              print("- ");
            if (abs(mem[p + glue_offset].intv) < 04'000'000)
              print("?.?");
            else if (abs(g) > float_constant(20'000)) {
              if (g > float_constant(0))
                print_char('>');
              else
                print("< -");
              print_glue(20'000 * unity, glue_order(p), 0);
            }
            else
              print_glue(std::round(unity * g), glue_order(p), 0);
          }
          // End Section 186
          if (shift_amount(p) != 0) {
            print(", unshifted ");
            print_scaled(shift_amount(p));
          }
        }
        node_list_display(list_ptr(p));
        // End Section 184
        break;
      case rule_node:
        // Begin Section 187
        print_esc("rule(");
        print_rule_dimen(height(p));
        print_char('+');
        print_rule_dimen(depth(p));
        print(")x");
        print_rule_dimen(width(p));
        // End Section 187
        break;
      case ins_node:
        // Begin Section 188
        print_esc("insert");
        print_int(int(subtype(p)));
        print(", natural size ");
        print_scaled(height(p));
        print("; split(");
        print_spec(split_top_ptr(p), 0);
        print_char(',');
        print_scaled(depth(p));
        print(") float cost ");
        print_int(float_cost(p));
        node_list_display(ins_ptr(p));
        // End Section 188
        break;
      case whatsit_node:
        // Begin Section 1356
        switch (subtype(p)) {
        case open_node:
          print("open_node subtype of whatsit_node");
          break;
        case write_node:
          print("write_node subtype of whatsit_node");
          break;
        case close_node:
          print("close_node subtype of whatsit_node");
          break;
        case special_node:
          print("special_node subtype of whatsit_node");
          break;
        default:
          print("Unknown subtype of whatsit_node");
          break;
        }
        // End Section 1356
       break;
      case glue_node:
        // Begin Section 189
        if (int(subtype(p)) > int(a_leaders)) {
          // Begin Section 190
          print_esc("");
          if (subtype(p) == c_leaders)
            print_char('c');
          else if (subtype(p) == x_leaders)
            print_char('x');
          print("leaders");
          print_spec(glue_ptr(p), 0);
          node_list_display(leader_ptr(p));
          // End Section 190
        }
        else {
          if (subtype(p) != normal) {
            print_char('(');
            if (subtype(p) < cond_math_glue)
              print_skip_param(int(subtype(p)) - 1);
            else if (subtype(p) == cond_math_glue)
              print("nonscript");
            else
              print_esc("mskip");
            print_char(')');
          }
        }
        if (subtype(p) != cond_math_glue) {
          print_char(' ');
          if (subtype(p) < cond_math_glue)
            print_spec(glue_ptr(p), 0);
          else
            print_spec(glue_ptr(p), mu_string);
        }
        // End Section 189
        break;
      case kern_node:
        // Begin Section 191
        if (subtype(p) != mu_glue) {
          print_esc("kern");
          if (subtype(p) != normal)
            print_char(' ');
          print_scaled(width(p));
          if (subtype(p) == acc_kern)
            print(" (for accent)");
        }
        else {
          print_esc("mkern");
          print_scaled(width(p));
          print(mu_string);
        }
        // End Section 191
        break;
      case math_node:
        // Begin Sectio 192
        print_esc("math");
        if (subtype(p) == before)
          print("on");
        else
          print("off");
        if (width(p) != 0) {
          print(", surrounded ");
          print_scaled(width(p));
        }
        // End Section 192
        break;
      case ligature_node:
        // Begin Section 193
        print_font_and_char(lig_char(p));
        print(" (ligature ");
        font_in_short_display = font(lig_char(p));
        short_display(lig_ptr(p));
        print_char(')');
        // End Section
        break;
      case penalty_node:
        // Begin Section 194
        print_esc("penalty ");
        print_int(penalty(p));
        // End Section 194
        break;
      case disc_node:
        // Begin Section 195
        print_esc("discretionary");
        if (int(replace_count(p)) > 0) {
          print(" replacing ");
          print_int(int(replace_count(p)));
        }
        node_list_display(pre_break(p));
        append_char('|');
        show_node_list(post_break(p));
        flush_char();
        // End Section 195
       break;
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
        print_esc("vadjust");
        node_list_display(adjust_ptr(p));
        // End Section 197
        break;
      // Begin Section 690
      // Not implemented---math mode nodes will fall through to default case
      // End Section 690
      default:
        print("Uknown node type!");
        break;
      }
    // End Section 183
    p = link(p);
  }
}

// Section 198

void show_box(pointer p) {
  // Begin Section 236
  depth_threshold = show_box_depth();
  breadth_max = show_box_breadth();
  // End Section 236
  if (breadth_max <= 0)
    breadth_max = 5;
  // Code for pool_size not included because we have no string pool size limit
  show_node_list(p);
  print_ln();
}

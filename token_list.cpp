#include "token_list.hpp"

#include <iostream> // Only for debugging
#include <string>

#include "basic-memory.hpp"
#include "command_codes.hpp"
#include "hash.hpp"
#include "print.hpp"
#include "trick_count.hpp"

// Section 292

void show_token_list(int p, int q, int l) {
  int m, c;
  char match_chr;
  char n;
  match_chr = '#';
  n = '0';
  std::cout << match_chr << ' ' << n; // Just to prevent errors
  tally = 0;
  while (p != null && tally < l) {
    if (p == q) {
      // Begin Section 320
      set_trick_count();
      // End Section 320
    }
    // Begin Section 293
    if (p < hi_mem_min || p > mem_end) {
      print_esc("CLOBBERED.");
      return;
    }
    if (info(p) >= cs_token_flag)
      print_cs(info(p) - cs_token_flag);
    else {
      m = info(p) % 0x100;
      c = info(p) % 0x100;
      std::cout  << c; // Just to prevent errors
      if (info(p) < 0 || c > 127)
        print_esc("BAD.");
      else {
        // Begin Section 294
        switch (m) {
        case left_brace:
        case right_brace:
        case math_shift:
        case tab_mark:
        case sup_mark:
        case sub_mark:
        case spacer:
        case letter:
        case other_char:
          print(c);
          break;
        case mac_param:
          print(c);
          print(c);
          break;
        case out_param:
          std::cout << match_chr;
          if (c <= 9)
            std::cout << char(c + int('0'));
          else {
            std::cout << '!';
            return;
          }
          break;
        case match:
          match_chr = char(c);
          std::cout << match_chr; // TeX uses c but std::cout needs the type so we use match_chr
          n++;
          std::cout << char(n);
          if (n > '9')
            return;
          break;
        case end_match:
          print("->");
          break;
        default:
          print_esc("BAD.");
          break;
        }
        // End Section 294
      }
    }
    // End Section 293
    p = link(p);
  }
  if (p != null)
    print_esc("ETC.");
}

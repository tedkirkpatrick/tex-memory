#include "token_list.hpp"

#include <iostream> // Only for debugging

#include "basic-memory.hpp"
#include "hash.hpp"
#include "print.hpp"

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
      std::cout << m << ' ' << c; // Just to prevent errors
      if (info(p) < 0 || c > 127)
        print_esc("BAD.");
      else {
        // Begin Section 294
        // End Section 294
      }
    }
    // End Section 293
    p = link(p);
  }
  if (p != null)
    print_esc("ETC.");
}

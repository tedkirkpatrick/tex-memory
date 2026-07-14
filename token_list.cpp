#include "token_list.hpp"

#include <string>

#include "basic-memory.hpp"
#include "command_codes.hpp"
#include "hash.hpp"
#include "printing.hpp"
#include "trick_count.hpp"

// Not in TeX but useful for testing and debugging

[[nodiscard]] pointer new_token_list(int refc) {
  pointer p = get_avail();
  info(p) = refc;
  return p;
}

pointer add_token_to_list(pointer p, halfword token) {
  while (link(p) != null)
    p = link(p);
  pointer t = get_avail();
  info(t) = token;
  link(p) = t;
  return p;
}


// Section 292

void show_token_list(int p, int q, int l) {
  int m, c;
  char match_chr;
  char n;
  match_chr = '#';
  n = '0';
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
      m = info(p) / 0x1'00;
      c = info(p) % 0x1'00;
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
          print(match_chr);
          if (c <= 9)
            print_char(char(c + int('0')));
          else {
            print_char('!');
            return;
          }
          break;
        case match:
          match_chr = char(c);
          print(c);
          n++;
          print(n);
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

// Section 295

void token_show(pointer p) {
  if (p != null)
    show_token_list(link(p), null, 1'000);
}

// Section 296

// We don't expect to be using print_meaning() as we aren't defining commands.
// So we fake the following three variables

// Next two defined in Section 297
static std::byte cur_cmd;
static halfword cur_chr;

// Next routine defined in Section 298

static void print_cmd_chr(quarterword cmd, halfword chr_code) {
  return;
}

// Next array defined in Section 382
static pointer cur_mark[5];

void print_meaning() {
  print_cmd_chr(cur_cmd, cur_chr);
  if (int(cur_cmd) >= call) {
    print_char(':');
    print_ln();
    token_show(cur_chr);
  }
  else {
    if (int(cur_cmd) == top_bot_mark) {
      print_char(':');
      print_ln();
      token_show(cur_mark[cur_chr]);
    }
  }
}

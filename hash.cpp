#include "hash.hpp"

#include <format>
#include <iostream>
#include <print>

#include "command_codes.hpp"
#include "eqtb.hpp"
#include "hash.hpp"
#include "print.hpp"
#include "string_handling.hpp"

// Section 256

two_halves hash[undefined_control_sequence];

// Section 262

void print_cs(int p) {
  if (p < hash_base) {
    if (p >= single_base)
      if (p == null_cs) {
        print_esc("csname");
        print_esc("endcsname");
      }
      else {
        print_esc(std::format("Whatever control sequence is located at string {}", p - single_base));
        if (cat_code(p - single_base) == letter)
          std::cout << ' ';
      }
    else if (p < active_base)
      print_esc("IMPOSSIBLE.");
    else
      std::cout << p - active_base;
  }
  else if (p >= undefined_control_sequence)
    print_esc("IMPOSSIBLE.");
  else if (text(p) < 0 || text(p) >= str_ptr)
    print_esc("NONEXISTENT.");
  else {
    print_esc("");
    std::print("String referenced by hash entry {}", text(p));
    std::cout << ' ';
  }
}

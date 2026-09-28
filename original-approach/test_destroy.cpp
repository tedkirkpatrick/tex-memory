/*
  Test routines for destroying boxes.
  Does not correspond to any code in TeX.

  See ../README.md for details of the overall project.

  CODE STYLE
  These routines are NOT transliterations of TeX code. They are purpose-written
  for this demonstration. They are written in modern C++.

  COPYRIGHT
  The code in this file is public domain.
  See See https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <algorithm>
#include <cstddef>
#include <format>
#include <iostream>
#include <print>
#include <sstream>
#include <vector>

#include "basic_memory.hpp"
#include "boxes.hpp"
#include "catch2.hpp"
#include "command_codes.hpp"
#include "destroying_boxes.hpp"
#include "display.hpp"
#include "printing.hpp"
#include "sample_lists.hpp"

// Set to true to print memory traces
constexpr bool trace_memory {false};

using namespace dynmemdbg;

namespace {

  void print_single_word(pointer p) {
    int m = info(p) / 0x1'00;
    int c = info(p) % 0x1'00;
    std::print("{} -> {}: ", p, link(p));
    if (font(p) == std::byte{0})
      std::print("default font {} ({:x})", char(character(p)), int(character(p)));
    else if (m == letter)
      std::print("letter token {} ({:x})", char(c), int(c));
    else
      std::print("other {:x}", info(p));
  }

  void short_display_avail_list(std::ostringstream& ostr) {
    memory_word *mem;
    pointer avail;
    pointer mem_end;
    pointer hi_mem_min;
    dynmemdbg::expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    std::vector<pointer> used {};

    std::println("--- Short display of avail list ---");
    for (pointer p=avail; p != null; p=link(p)) {
      used.push_back(p);
      print_single_word(p);
      std::println("");
    }
    std::println("---");
    std::println("--- Short display of occupied single-word items ---");
    for (pointer p=hi_mem_min; p < mem_end; p++) {
      if (std::none_of(used.cbegin(), used.cend(), [&p](pointer p1) { return p1 == p; })) {
        print_single_word(p);
        std::println("");
      }
    }
  }

  [[nodiscard]] int avail_len(pointer avail) {
    int count = 0;
    while (avail != null) {
      count++;
      avail = link(avail);
    }
    return count;
  }

  void short_display_free_list(pointer rover) {
    std::println("Rover {} size {}", rover, node_size(rover));
    for (pointer p=rlink(rover); p != rover; p=rlink(p)) {
      std::println("{} size {}", p, node_size(p));
    }
  }

  void set_free(std::vector<char>& used, pointer start, int size) {
    for (pointer p=start; p < start+size; p++)
      used[p] = '0';
  }

  // Map used space over [0, highest_used]
  [[nodiscard]] std::vector<char>  map_occupied_nodes(pointer rover, pointer highest_used) {
    std::vector<char> used (std::size_t(highest_used), '1');
    set_free(used, rover, node_size(rover));
    for (pointer p=rlink(rover); p != rover; p=rlink(p)) {
      set_free(used, p, node_size(p));
    }
    return used;
  }

  void print_used(std::vector<char>& used) {
    bool used_run_started {false};
    int count {0};
    int run_begin {0};
    for(char c : used) {
      if (c == '1' && ! used_run_started) {
        used_run_started = true;
        run_begin = count;
        std::print("Used starts at {}", count); 
      }
      else if (c == '0' && used_run_started) {
        std::println(" for {} words", count - run_begin);
        used_run_started = false;
      }
      count++;
    }
    if (used_run_started) {
      std::println(" for {} words", count - run_begin);
    }
  }

  [[nodiscard]] int free_list_len(pointer rover) {
    int total = node_size(rover);
    for (pointer p=rlink(rover); p != rover; p=rlink(p)) {
      total += node_size(p);
    }
    return total;
  }

} // unnamed namespace

TEST_CASE("Exercise all branches of flush_node_list()") {
  init_table_entries();
  init_eqtb();
  str_start.reset_strings();
  std::ostringstream ostr;
  set_str(&ostr);
  
  memory_word* mem;
  pointer avail;
  pointer mem_end;
  pointer hi_mem_min;
  
  pointer rover;
  pointer lo_mem_max;
  halfword node_increment;

  SECTION("Tests on a fresh data structure should show empty memory") {
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    REQUIRE(avail_len(avail) == mem_max - hi_mem_min);
    expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
    REQUIRE(free_list_len(rover) == lo_mem_max);    
  }

  SECTION("Basic list") {
    pointer nbl = new_basic_list();
    if constexpr (trace_memory) {
      short_display_avail_list(ostr);
      show_box(nbl);
      std::print("{}\n", ostr.view());
      expose_avail_vars(mem, avail, mem_end, hi_mem_min);
      std::print("avail {} avail_len(avail) {} mem_max {} hi_mem_min {}\n",
                 avail, avail_len(avail), mem_max, hi_mem_min);
    }
    flush_node_list(nbl);
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    if constexpr (trace_memory) {
      short_display_avail_list(ostr);
      std::print("avail {} avail_len(avail) {} mem_max {} hi_mem_min {}\n",
                 avail, avail_len(avail), mem_max, hi_mem_min);
    }
    REQUIRE(avail_len(avail) == mem_max - hi_mem_min);
 
    expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
    if constexpr (trace_memory) {
      std::println("--- Free node list before merge ---");
      short_display_free_list(rover);
      std::println("---- Free node list after merge ---");
    }
    (void) get_node(merge_only);
    expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
    if constexpr (trace_memory) {
      short_display_free_list(rover);
      std::vector<char> used = map_occupied_nodes(rover, node_increment);
      print_used(used);
    }
    REQUIRE(free_list_len(rover) == lo_mem_max);
  }
}

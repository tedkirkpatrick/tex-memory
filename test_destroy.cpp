#include <algorithm>
#include <format>
#include <iostream>
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

// Debugging
#include <print>

using namespace dynmemdbg;

static void print_single_word(pointer p) {
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

static void short_display_avail_list(std::ostringstream& ostr) {
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


[[nodiscard]] static int avail_len(pointer avail) {
  int count = 0;
  while (avail != null) {
    count++;
    avail = link(avail);
  }
  return count;
}

static void short_display_free_list(pointer rover) {
  std::println("Rover {} size {}", rover, node_size(rover));
  for (pointer p=rlink(rover); p != rover; p=rlink(p)) {
    std::println("{} size {}", p, node_size(p));
  }
}

[[nodiscard]] static int free_list_len(pointer rover) {
  int total = node_size(rover);
  for (pointer p=rlink(rover); p != rover; p=rlink(p)) {
    total += node_size(p);
  }
  return total;
}

TEST_CASE("Destroying list") {
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

  SECTION("Basic list") {
    pointer nbl = new_basic_list();
    short_display_avail_list(ostr);
    show_box(nbl);
    std::print("{}\n", ostr.view());
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    std::print("avail {} avail_len(avail) {} mem_max {} hi_mem_min {}\n",
               avail, avail_len(avail), mem_max, hi_mem_min);
    flush_node_list(nbl);
    short_display_avail_list(ostr);
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    std::print("avail {} avail_len(avail) {} mem_max {} hi_mem_min {}\n",
               avail, avail_len(avail), mem_max, hi_mem_min);
    REQUIRE(avail_len(avail) == mem_max - hi_mem_min);
 
    expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
    short_display_free_list(rover);
    ostr.str("");
    show_box(953);
    std::println("^^^^box 953\n{}", ostr.view());
    REQUIRE(free_list_len(rover) == lo_mem_max);
  }
}

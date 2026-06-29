#include <print>
//#include <iostream>

#include "basic-memory.hpp"
#include "catch2.hpp"

using std::print, std::println;

#if 0

void test_avail() {
  memory_word *mem;
  pointer avail;
  pointer mem_end;
  pointer rover;

  pointer n = get_node(100);
  std::print("Node {}\n", n);
  pointer n2 = get_node(5);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  std::print("Node {} rover {}\n", n2, hi_mem_min);
  free_node(n, 100);
  free_node(n2, 5);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  std::print("Before merge rover {}, size {}, rlink {}, llink {}\n", rover, node_size(rover), llink(rover), rlink(rover));
  (void) get_node(merge_only);
  expose_avail_vars(mem, avail, mem_end, rover);
  std::print("After merge  rover {}, size {}, rlink {}, llink {}\n", rover, node_size(rover), llink(rover), rlink(rover));
}

#endif

TEST_CASE("Memory word is 4", "[packed memory]") {
  REQUIRE(sizeof(memory_word) == 4);
}

TEST_CASE("flush_list(null) null is idempotent") {
  memory_word *mem;
  pointer avail;
  pointer mem_end;
  pointer hi_mem_min;

  init_table_entries();
  flush_list(null);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(avail == null);
  REQUIRE(mem_end == mem_top);
  REQUIRE(hi_mem_min == mem_top);
}

TEST_CASE("Single-word allocation works", "[free list]") {
  memory_word *mem;
  pointer avail;
  pointer mem_end;
  pointer hi_mem_min;

  init_table_entries();
  pointer p;
  fast_get_avail(p);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p == 29'999);
  REQUIRE(link(p) == 0);
  REQUIRE(mem_end == 30'000);
  REQUIRE(hi_mem_min == p);

  SECTION("Freeing single-word list") {
    flush_list(p);
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    REQUIRE(avail == p);
    REQUIRE(link(avail) == null);
    REQUIRE(hi_mem_min == p);
 }
  
  SECTION("Allocating second word to list and freeing") {
    pointer p2;
    fast_get_avail(p2);
    link(p) = p2;
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    REQUIRE(p2 == 29'998);
    REQUIRE(link(p2) == null);
    REQUIRE(link(p) == p2);
    REQUIRE(avail == null);
    REQUIRE(mem_end == 30'000);

    flush_list(p);
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    REQUIRE(avail == 29'999);
    REQUIRE(link(avail) == p2);
    REQUIRE(link(p2) == null);
    REQUIRE(hi_mem_min == p2);
  }
}

TEST_CASE("Allocating from available list works") {
  memory_word *mem;
  pointer avail;
  pointer mem_end;
  pointer hi_mem_min;

  init_table_entries();
  pointer p1;
  fast_get_avail(p1);
  pointer p2;
  fast_get_avail(p2);
  link(p2) = p1;
  pointer p3;
  fast_get_avail(p3);
  link(p3) = p2;
  flush_list(p3);
  // The next three words should pull from the available list
  pointer p4;
  fast_get_avail(p4);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p4 == p3);
  REQUIRE(avail == p2);
  REQUIRE(hi_mem_min == 29'997);
  pointer p5;
  fast_get_avail(p5);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p5 == p2);
  REQUIRE(avail == p1);
  REQUIRE(hi_mem_min == 29'997);
  pointer p6;
  fast_get_avail(p6);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p6 == p1);
  REQUIRE(avail == null);
  REQUIRE(hi_mem_min == 29'997);
  // But the fourth word should expand the single-word memory range
  pointer p7;
  fast_get_avail(p7);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p7 == 29'996);
  REQUIRE(avail == null);
  REQUIRE(hi_mem_min == 29'996);  
}

TEST_CASE("Allocating a single two-word node") {
  pointer rover;
  pointer lo_mem_max;
  pointer hi_mem_min;
  halfword node_increment;

  constexpr halfword node_sz = 2;
  init_table_entries();

  // Allocate one node of size node_sz
  pointer p = get_node(node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == mem_min);
  REQUIRE(node_size(rover) == node_increment - node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == rover);
  REQUIRE(rlink(rover) == rover);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(p == node_increment - node_sz);
  REQUIRE(link(p) == null);

  // Free the node
  free_node(p, node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == mem_min);
  REQUIRE(node_size(rover) == node_increment - node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(link(p) == empty_flag);
  REQUIRE(((llink(p) == rover && rlink(rover) == p) ||
           (rlink(p) == rover && llink(rover) == p)));
  REQUIRE(node_size(p) == node_sz);

  // Merge the free list
  (void) get_node(merge_only);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == mem_min);
  REQUIRE(node_size(rover) == node_increment);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == null);
  REQUIRE(rlink(rover) == null);
  REQUIRE(lo_mem_max == node_increment);
}

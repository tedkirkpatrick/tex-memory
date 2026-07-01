/*
  Unit tests for TeX-style dynamic memory.
  These tests adopt a "glass-box" approach, where we check that allocations occur
  in the exact places and sequence used internally in the original TeX algorithms and
  access internal variable values via the functions in the dynmemdbg namespace. Such
  an approach is appropriate for this code, which is intended to exactly match the behaviour
  of the TeX Pascal code.
 */

#include <cassert>
#include <print>
//#include <iostream>

#include "basic-memory.hpp"
#include "catch2.hpp"

using std::print, std::println;

using namespace dynmemdbg;
using Catch::Matchers::Contains;

TEST_CASE("Memory word is 4", "[packed memory]") {
  REQUIRE(sizeof(memory_word) == 4);
}

TEST_CASE("flush_list(null) is idempotent") {
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
  REQUIRE(p == mem_max - 1);
  REQUIRE(link(p) == 0);
  REQUIRE(mem_end == mem_max);
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
    REQUIRE(p2 == mem_max - 2);
    REQUIRE(link(p2) == null);
    REQUIRE(link(p) == p2);
    REQUIRE(avail == null);
    REQUIRE(mem_end == mem_max);

    flush_list(p);
    expose_avail_vars(mem, avail, mem_end, hi_mem_min);
    REQUIRE(avail == mem_max - 1);
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
  REQUIRE(hi_mem_min == mem_max - 3);
  pointer p5;
  fast_get_avail(p5);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p5 == p2);
  REQUIRE(avail == p1);
  REQUIRE(hi_mem_min == mem_max - 3);
  pointer p6;
  fast_get_avail(p6);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p6 == p1);
  REQUIRE(avail == null);
  REQUIRE(hi_mem_min == mem_max - 3);
  // But the fourth word should expand the single-word memory range
  pointer p7;
  fast_get_avail(p7);
  expose_avail_vars(mem, avail, mem_end, hi_mem_min);
  REQUIRE(p7 == mem_max - 4);
  REQUIRE(avail == null);
  REQUIRE(hi_mem_min == mem_max - 4);
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

TEST_CASE("Allocating multiple nodes") {
  pointer rover;
  pointer lo_mem_max;
  pointer hi_mem_min;
  halfword node_increment;

  constexpr halfword node_sz = 5;
  init_table_entries();

  pointer p1 = get_node(node_sz);
  pointer p2 = get_node(node_sz);
  pointer p3 = get_node(node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);

  REQUIRE(rover == mem_min);
  REQUIRE(node_size(rover) == node_increment - 3 * node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == rover);
  REQUIRE(rlink(rover) == rover);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(p1 == node_increment - node_sz);
  REQUIRE(link(p1) == null);
  REQUIRE(p2 == node_increment - 2 * node_sz);
  REQUIRE(link(p2) == null);
  REQUIRE(p3 == node_increment - 3 * node_sz);
  REQUIRE(link(p3) == null);

  // Free the middle one
  free_node(p2, node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == mem_min);
  REQUIRE(node_size(rover) == node_increment - 3 * node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == p2);
  REQUIRE(rlink(rover) == p2);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(rlink(p2) == rover);
  REQUIRE(llink(p2) == rover);
  REQUIRE(node_size(p2) == node_sz);
  REQUIRE(link(p2) == empty_flag);

  // Allocate another node--it will be taken from rover node
  pointer p4 = get_node(node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == mem_min);
  REQUIRE(node_size(rover) ==  node_increment - 4 * node_sz); // The only change from above
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == p2);
  REQUIRE(rlink(rover) == p2);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(rlink(p2) == rover);
  REQUIRE(llink(p2) == rover);
  REQUIRE(node_size(p2) == node_sz);
  REQUIRE(link(p2) == empty_flag);

  REQUIRE(p4 == node_increment - 4 * node_sz);
  REQUIRE(link(p4) == null);
}

TEST_CASE("Allocating a node larger than node_increment") {
  pointer rover;
  pointer lo_mem_max;
  pointer hi_mem_min;
  halfword node_increment;

  init_table_entries();
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  const halfword node_sz = node_increment + 5;

  pointer p1 = get_node(node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == mem_min);
  REQUIRE(node_size(rover) == 2 * node_increment - node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == null);
  REQUIRE(rlink(rover) == null);
  REQUIRE(lo_mem_max == 2 * node_increment);

  REQUIRE(p1 == mem_min + 2 * node_increment - node_sz);
  REQUIRE(link(p1) == null);
}

TEST_CASE("Allocate a node exactly equal to the rover's size") {
  pointer rover;
  pointer lo_mem_max;
  pointer hi_mem_min;
  halfword node_increment;

  init_table_entries();
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  const halfword p1_node_sz = 5;
  pointer p1 = get_node(p1_node_sz);
  const halfword p2_node_sz = 5;
  pointer p2 = get_node(p2_node_sz);
  free_node(p1, p1_node_sz);

  // The free list now has two entries: the rover and the freed p1, separated by p2
  const halfword both_nodes_sz = p1_node_sz + p2_node_sz;
  pointer p3 = get_node(node_increment - both_nodes_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == mem_min + node_increment - p1_node_sz);
  REQUIRE(node_size(rover) == p1_node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == rover);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(p3 == mem_min);
  REQUIRE(link(p3) == null);
}

TEST_CASE("Force rover to move to rlink to complete an allocation") {
  pointer rover;
  pointer lo_mem_max;
  pointer hi_mem_min;
  halfword node_increment;

  init_table_entries();
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  const halfword p1_node_sz = node_increment / 10;
  const halfword other_free_sz = 5;
  const halfword p2_node_sz = node_increment - (p1_node_sz + other_free_sz);
  const halfword min_node_sz = 2;
  const halfword p3_node_sz = p1_node_sz - (other_free_sz - min_node_sz);
  const halfword p4_node_sz = other_free_sz;
  assert(p3_node_sz > other_free_sz); // We want to force rover to move to second free node
  pointer p1 = get_node(p1_node_sz);
  pointer p2 = get_node(p2_node_sz);
  free_node(p1, p1_node_sz);

  // Now the rover points to a free node of other_free_sz, followed by p2, followed by a free node of p1_node_size > p3_node_size.
  // A node of size p3_node_sz must be allocated from rlink(rover).
  pointer p3 = get_node(p3_node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  pointer other_free = llink(rover);
  REQUIRE(rover == p1);
  REQUIRE(node_size(rover) == p1_node_sz - p3_node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == other_free);
  REQUIRE(rlink(rover) == other_free);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(other_free == mem_min);
  REQUIRE(node_size(other_free) == other_free_sz);
  REQUIRE(link(other_free) == empty_flag);
  REQUIRE(llink(other_free) == rover);
  REQUIRE(rlink(other_free) == rover);

  REQUIRE(p3 == mem_min + node_increment - p3_node_sz);
  REQUIRE(link(p3) == null);

  // Now the rover points to a free node of other_free_sz - 1, linked to a free node of other_free_sz.
  // Allocating a node of other_free_sz will cause memory to be expanded and the node allocated there.
  // TeX's memory heuristics force memory expansion rather than filling the penultimate free node.
  pointer previous_rover = rover;
  pointer p4 = get_node(p4_node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  REQUIRE(rover == node_increment);
  REQUIRE(node_size(rover) == node_increment - p4_node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == other_free);
  REQUIRE(rlink(rover) == previous_rover);
  REQUIRE(lo_mem_max == node_increment * 2);

  REQUIRE(p4 == 2 * node_increment - p4_node_sz);
  REQUIRE(link(p4) == null);
}

TEST_CASE("Allocate part of a non-rover free node") {
  pointer rover;
  pointer lo_mem_max;
  pointer hi_mem_min;
  halfword node_increment;

  init_table_entries();
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  const halfword min_node_sz = 2;
  const halfword free_sz = min_node_sz;
  const halfword p1_node_sz = min_node_sz;
  const halfword p2_node_sz = 3 * min_node_sz;
  const halfword p3_node_sz = node_increment - (p1_node_sz + p2_node_sz + free_sz);
  const halfword p4_node_sz = 2 * min_node_sz;

  pointer p1 = get_node(p1_node_sz);
  pointer p2 = get_node(p2_node_sz);
  pointer p3 = get_node(p3_node_sz);
  free_node(p2, p2_node_sz);

  // rover points to a free node of size free_sz and rlink(rover) == p2 (which was freed).
  // p4 will be allocated from the freed p2 node and rover will be moved to point to the residue.
  pointer p4 = get_node(p4_node_sz);
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  const pointer other_free_node  = mem_min;
  REQUIRE(rover == mem_min + node_increment - (p1_node_sz + p2_node_sz));
  REQUIRE(node_size(rover) == p2_node_sz - p4_node_sz);
  REQUIRE(link(rover) == empty_flag);
  REQUIRE(llink(rover) == other_free_node);
  REQUIRE(rlink(rover) == other_free_node);
  REQUIRE(lo_mem_max == node_increment);

  REQUIRE(node_size(other_free_node) == free_sz);
  REQUIRE(link(other_free_node) == empty_flag);
  REQUIRE(llink(other_free_node) == rover);
  REQUIRE(rlink(other_free_node) == rover);

  REQUIRE(p4 == p2 + p2_node_sz - p4_node_sz);
  REQUIRE(link(p4) == null);
}

TEST_CASE("Throw exception when get_avail() exceeds memory") {
  init_table_entries();
  pointer p;
  fast_get_avail(p);
  fast_get_avail(p);
  fast_get_avail(p);
  const halfword all_memory = mem_max - mem_min;
  REQUIRE_THROWS_WITH(get_node(all_memory), Contains("main memory size"));
}

TEST_CASE("Throw exception when get_node() exceeds memory") {
  pointer rover;
  pointer lo_mem_max;
  pointer hi_mem_min;
  halfword node_increment;

  init_table_entries();
  expose_node_vars(rover, lo_mem_max, hi_mem_min, node_increment);
  halfword leave_sz = 5;
  halfword p1_node_sz = mem_max - mem_min - leave_sz;
  pointer p1 = get_node(p1_node_sz);
  // The dynamic allocation for p1 will leave one word for the avail list
  pointer p;
  fast_get_avail(p); // This will succeed

  REQUIRE_THROWS_WITH(fast_get_avail(p), Contains("main memory size"));
}

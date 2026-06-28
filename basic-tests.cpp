#include <print>
#include <iostream>

#include "basic-memory.hpp"
#include "catch2.hpp"

void test_avail() {
  memory_word *mem;
  pointer avail;
  pointer mem_end;
  pointer rover;

  pointer p;
  fast_get_avail(p);
  expose_variables(mem, avail, mem_end, rover);  
  std::cout << p << ' ' << link(p) << ' ' << mem_end << '\n';
  pointer p2;
  fast_get_avail(p2);
  link(p) = p2;
  expose_variables(mem, avail, mem_end, rover);
  std::cout << p2 << ' ' << link(p2) << ' ' << avail << ' ' << mem_end << '\n';
  
  flush_list(p);
  std::cout << p << ' ' << avail << '\n';

  fast_get_avail(p);
  std::cout << "After pull from avail " << p << " avail " << avail << '\n';
  free_avail(p);
  std::cout << "After free avail " << p << " avail " << avail << '\n';

  if (mem_max < 50) {
    std::cout << "Testing to exhaustion\n";
    pointer r = null;
    for (int i = 0; i <= mem_max; i++) {
      fast_get_avail(p);
      link(p) = r;
      r = p;
    }
  }

  pointer n = get_node(100);
  std::print("Node {}\n", n);
  pointer n2 = get_node(5);
  expose_variables(mem, avail, mem_end, rover);
  std::print("Node {} rover {}\n", n2, rover);
  free_node(n, 100);
  free_node(n2, 5);
  expose_variables(mem, avail, mem_end, rover);
  std::print("Before merge rover {}, size {}, rlink {}, llink {}\n", rover, node_size(rover), llink(rover), rlink(rover));
  (void) get_node(merge_only);
  expose_variables(mem, avail, mem_end, rover);
  std::print("After merge  rover {}, size {}, rlink {}, llink {}\n", rover, node_size(rover), llink(rover), rlink(rover));
}

TEST_CASE("Memory word is 4", "[packed memory]") {
  REQUIRE(sizeof(memory_word) == 4);
}

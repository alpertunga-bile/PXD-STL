#define ENABLE_ASSERTS

#include "sources/test/array_tests.hpp"
#include "sources/test/binary_search_tree_tests.hpp"
#include "sources/test/linked_list_tests.hpp"
#include "sources/test/test_manager.hpp"

#include "sources/binary_search_tree.hpp"

int main() {
  pxd::TestManager test_manager;

  pxd::ArrayTests array_tests;
  pxd::LinkedListTests linked_list_tests;
  pxd::BinarySearchTreeTests binary_search_tree_tests;

  test_manager.add_test("Array Tests", array_tests);
  test_manager.add_test("Linked Link Tests", linked_list_tests);
  test_manager.add_test("Binary Search Tree Tests", binary_search_tree_tests);

  test_manager.print_results();

  return 0;
}
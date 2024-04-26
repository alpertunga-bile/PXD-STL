#define ENABLE_ASSERTS

#include "sources/test/array_tests.hpp"
#include "sources/test/binary_search_tree_tests.hpp"
#include "sources/test/linked_list_tests.hpp"
#include "sources/test/queue_tests.hpp"
#include "sources/test/stack_tests.hpp"

#include "sources/test/test_manager.hpp"

int main() {
  pxd::TestManager test_manager;

  pxd::ArrayTests array_tests;
  pxd::LinkedListTests linked_list_tests;
  pxd::BinarySearchTreeTests binary_search_tree_tests;
  pxd::StackTests stack_tests;
  pxd::QueueTests queue_tests;

  test_manager.add_test("Array Tests", array_tests);
  test_manager.add_test("Linked Link Tests", linked_list_tests);
  test_manager.add_test("Binary Search Tree Tests", binary_search_tree_tests);
  test_manager.add_test("Stack Tests", stack_tests);
  test_manager.add_test("Queue Tests", queue_tests);

  test_manager.print_results();

  return 0;
}
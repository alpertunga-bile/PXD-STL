#include "test/array_tests.hpp"
#include "test/binary_search_tree_tests.hpp"
#include "test/double_linked_list_tests.hpp"
#include "test/dynamic_array_tests.hpp"
#include "test/linked_list_tests.hpp"
#include "test/matrix_tests.hpp"
#include "test/priority_queue_tests.hpp"
#include "test/queue_tests.hpp"
#include "test/regex_tests.hpp"
#include "test/stack_tests.hpp"
#include "test/xor_double_linked_list_tests.hpp"

#include "test/test_manager.hpp"

#include "logger.hpp"

void do_test() {
  PXD_LOG_WARNING("WARNING");
  PXD_LOG_ERROR("ERROR");
  PXD_LOG_INFO("Starting tests");

  pxd::TestManager test_manager;

  pxd::ArrayTests array_tests;
  pxd::LinkedListTests linked_list_tests;
  pxd::BinarySearchTreeTests binary_search_tree_tests;
  pxd::StackTests stack_tests;
  pxd::QueueTests queue_tests;
  pxd::DynamicArrayTests dynamic_array_tests;
  pxd::MatrixTests matrix_tests;
  pxd::DoubleLinkedListTests double_linked_list_tests;
  pxd::XORDoubleLinkedListTests xor_double_linked_list_tests;
  pxd::PriorityQueueTests priority_queue_tests;
  pxd::RegexTests regex_tests;

  test_manager.add_test("Array Tests", array_tests);
  test_manager.add_test("Linked List Tests", linked_list_tests);
  test_manager.add_test("Binary Search Tree Tests", binary_search_tree_tests);
  test_manager.add_test("Stack Tests", stack_tests);
  test_manager.add_test("Queue Tests", queue_tests);
  test_manager.add_test("Dynamic Array Tests", dynamic_array_tests);
  test_manager.add_test("Matrix Tests", matrix_tests);
  test_manager.add_test("Double Linked List Tests", double_linked_list_tests);
  test_manager.add_test("XOR Double Linked List Tests",
                        xor_double_linked_list_tests);
  test_manager.add_test("Priority Queue Tests", priority_queue_tests);
  test_manager.add_test("Regex Tests", regex_tests);

  test_manager.print_results();
  test_manager.save_results();
}

#include "filesystem.hpp"

auto main() -> int {
  do_test();

  return 0;
}

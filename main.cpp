#include "sources/test/test_manager.hpp"

#include "sources/test/array_tests.hpp"

int
main()
{
  pxd::TestManager test_manager;

  pxd::ArrayTests array_tests;

  test_manager.add_test("Array Tests", array_tests);

  test_manager.print_results();

  return 0;
}
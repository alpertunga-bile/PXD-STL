#pragma once

#include "binary_search_tree.hpp"
#include "test_utils.hpp"

namespace pxd {
class BinarySearchTreeTests : public ITest {
public:
  void start_test() override {
    int *temp_arr = new int[N];
    int *check_arr = new int[N];

    for (int i = 0; i < N; i++) {
      temp_arr[i] = i + 1;
      check_arr[i] = 0;
    }

    start_array_ctor_test(temp_arr, check_arr);

    delete[] check_arr;
    delete[] temp_arr;
  }

private:
  void start_array_ctor_test(int *temp_arr, int *check_arr) {
    BinarySearchTree<int> bst(temp_arr, N);

    bst.get_order(check_arr, eBST_ORDER::INORDER);

    test_results["array default ctor"] = check_arrays(temp_arr, check_arr, N);
  }

private:
  int N = 10;
};
} // namespace pxd
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
    start_copy_ctor_test(temp_arr, check_arr);
    start_move_ctor_test(temp_arr, check_arr);
    start_assign_ctor_test(temp_arr, check_arr);
    start_balanced_tree_test(temp_arr, check_arr);
    start_is_contain_test(temp_arr);
    start_min_max_test(temp_arr);
    start_remove_test(temp_arr);

    delete[] check_arr;
    delete[] temp_arr;
  }

private:
  void start_array_ctor_test(int *temp_arr, int *check_arr) {
    BinarySearchTree<int> bst(temp_arr, N);

    bst.get_order(check_arr, eBST_ORDER::INORDER);

    test_results["array default ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_copy_ctor_test(int *temp_arr, int *check_arr) {
    BinarySearchTree<int> bst(temp_arr, N);
    BinarySearchTree<int> temp_bst(bst);

    temp_bst.get_order(check_arr, eBST_ORDER::INORDER);

    test_results["copy ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_move_ctor_test(int *temp_arr, int *check_arr) {
    BinarySearchTree<int> bst(temp_arr, N);
    BinarySearchTree<int> temp_bst(std::move(bst));

    temp_bst.get_order(check_arr, eBST_ORDER::INORDER);

    test_results["move ctor"] = check_arrays(temp_arr, check_arr, N);

    BinarySearchTree<int> temp;
    temp = std::move(temp_bst);
    temp.get_order(check_arr, eBST_ORDER::INORDER);

    test_results["assign move ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_assign_ctor_test(int *temp_arr, int *check_arr) {
    BinarySearchTree<int> bst(temp_arr, N);
    BinarySearchTree<int> temp_bst = bst;

    temp_bst.get_order(check_arr, eBST_ORDER::INORDER);

    test_results["assign ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_balanced_tree_test(int *temp_arr, int *check_arr) {
    BinarySearchTree<int> bst(temp_arr, N);
    bst.get_balanced_tree().get_order(check_arr, eBST_ORDER::PREORDER);

    int calc_arr[10] = {5, 2, 1, 3, 4, 8, 6, 7, 9, 10};

    test_results["get balanced tree"] = check_arrays(check_arr, calc_arr, N);
  }

  void start_is_contain_test(int *temp_arr) {
    BinarySearchTree<int> bst(temp_arr, N);

    test_results["is contain"] = bst.is_contain(5) && !bst.is_contain(12);
  }

  void start_min_max_test(int *temp_arr) {
    BinarySearchTree<int> bst(temp_arr, N);

    test_results["min max"] =
        bst.get_min_value() == 1 && bst.get_max_value() == 10;
  }

  void start_remove_test(int *temp_arr) {
    BinarySearchTree<int> bst(temp_arr, N, true);

    bst.remove(1);
    bst.remove(3);
    bst.remove(6);
    bst.remove(9);
    bst.remove(5);
    bst.remove(4);
    bst.remove(2);
    bst.remove(7);
    bst.remove(10);
    bst.remove(8);

    test_results["remove"] = bst.get_total_node_count() == 0;
  }

private:
  int N = 10;
};
} // namespace pxd
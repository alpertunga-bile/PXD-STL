#pragma once

#include "linked_list.hpp"
#include "test_utils.hpp"

namespace pxd {
class LinkedListTests : public ITest {
public:
  void start_test() override {
    int *temp = new int[N];
    temp_arr = new int[N];

    for (int i = 0; i < N; i++) {
      temp[i] = i + 1;
      temp_arr[i] = 0;
    }

    start_normal_array_init_test(temp);
    start_reverse_array_init_test(temp);
    start_index_test(temp);
    start_reverse_test(temp);
    start_copy_ctor_test(temp);
    start_move_ctor_test(temp);
    start_assign_ctor_test(temp);
    start_remove_test(temp);

    delete[] temp_arr;
    delete[] temp;
  }

private:
  void start_normal_array_init_test(int *arr) {
    LinkedList<int> ll(arr, N);
    ll.to_array(temp_arr);

    test_results["normal array init"] = check_arrays<int>(temp_arr, arr, N);
  }

  void start_reverse_array_init_test(int *arr) {
    LinkedList<int> ll(arr, N, true);
    ll.to_array(temp_arr);

    test_results["reverse array init"] =
        check_reverse_arrays<int>(arr, temp_arr, N);
  }

  void start_index_test(int *arr) {
    LinkedList<int> ll(arr, N);

    test_results["indexing"] = ll[3] == arr[3];
    test_results["negative indexing"] = ll[-1] == arr[N - 1];
  }

  void start_reverse_test(int *arr) {
    LinkedList<int> ll(arr, N);
    ll.reverse();
    ll.to_array(temp_arr);

    test_results["reverse"] = check_reverse_arrays(arr, temp_arr, N);
  }

  void start_assign_ctor_test(int *arr) {
    LinkedList<int> ll(arr, N);
    ll.to_array(temp_arr);

    LinkedList<int> tll;

    int *temp_new_arr = new int[N];

    tll = ll;
    tll.to_array(temp_new_arr);

    test_results["assign constructor"] =
        check_arrays<int>(temp_new_arr, temp_arr, N);

    delete[] temp_new_arr;
  }

  void start_copy_ctor_test(int *arr) {
    LinkedList<int> ll(arr, N);
    ll.to_array(temp_arr);

    LinkedList<int> tll(ll);

    int *temp_new_arr = new int[N];
    tll.to_array(temp_new_arr);

    test_results["copy constructor"] =
        check_arrays<int>(temp_new_arr, temp_arr, N);

    delete[] temp_new_arr;
  }

  void start_move_ctor_test(int *arr) {
    LinkedList<int> ll(arr, N);
    ll.to_array(temp_arr);

    LinkedList<int> tll(std::move(ll));

    int *temp_new_arr = new int[N];
    tll.to_array(temp_new_arr);

    test_results["move constructor"] =
        check_arrays<int>(temp_new_arr, temp_arr, N);

    delete[] temp_new_arr;
  }

  void start_remove_test(int *arr) {
    LinkedList<int> ll(arr, N);
    LinkedList<int> ell(ll);
    LinkedList<int> mll(ll);

    ll.remove(0);

    ll.to_array(temp_arr);

    test_results["remove_head"] = check_arrays<int>(temp_arr, 0, arr, 1, N - 1);

    ell.remove(-1);
    ell.to_array(temp_arr);

    test_results["remove_end"] = check_arrays<int>(temp_arr, 0, arr, 0, N - 1);

    mll.remove(5);
    mll.to_array(temp_arr);
    test_results["remove_between"] =
        !check_arrays<int>(temp_arr, 0, arr, 0, N - 1);
  }

private:
  int *temp_arr = nullptr;
  int N = 10;
};
} // namespace pxd
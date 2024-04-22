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
    start_assign_ctor_test(temp);

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

    test_results["indexing"] = ll[3] == arr[3] && ll[-1] == arr[N - 1];
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

private:
  int *temp_arr = nullptr;
  int N = 10;
};
} // namespace pxd
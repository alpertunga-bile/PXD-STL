#pragma once

#include "dynamic_array.hpp"
#include "test_utils.hpp"

namespace pxd {
class DynamicArrayTests : public ITest {
public:
  void start_test() override {
    int *temp_arr = new int[N];

    for (int i = 0; i < N; i++) {
      temp_arr[i] = i + 1;
    }

    start_copy_ctor_test(temp_arr);
    start_move_ctor_test(temp_arr);
    start_assign_ctor_test(temp_arr);
    start_expand_test(temp_arr);
    start_add_test(temp_arr);
    start_shrink_test(temp_arr);
    start_index_test(temp_arr);
    start_resize_test(temp_arr);

    delete[] temp_arr;
  }

private:
  void start_copy_ctor_test(int *temp_arr) {
    DynamicArray<int> darray;
    darray.expand(temp_arr, N);

    DynamicArray<int> dtemp(darray);

    test_results["copy ctor"] =
        check_arrays<int>(darray.get_data(), dtemp.get_data(), N);
  }

  void start_move_ctor_test(int *temp_arr) {
    DynamicArray<int> darray;
    darray.expand(temp_arr, N);

    DynamicArray<int> dtemp(std::move(darray));

    test_results["move ctor"] =
        check_arrays<int>(temp_arr, dtemp.get_data(), N);

    DynamicArray<int> dtemp_2;
    dtemp_2 = std::move(dtemp);

    test_results["assign move ctor"] =
        check_arrays<int>(temp_arr, dtemp_2.get_data(), N);
  }

  void start_assign_ctor_test(int *temp_arr) {
    DynamicArray<int> darray;
    darray.expand(temp_arr, N);

    DynamicArray<int> dtemp = darray;

    test_results["assign ctor"] =
        check_arrays<int>(darray.get_data(), dtemp.get_data(), N);
  }

  void start_expand_test(int *temp_arr) {
    DynamicArray<int> darray;
    darray.expand(temp_arr, N);

    test_results["expand from 0"] =
        check_arrays<int>(darray.get_data(), temp_arr, N);

    darray.expand(temp_arr, N);

    test_results["expand with exist"] =
        check_arrays<int>(darray.get_data(), N, temp_arr, 0, N);
  }

  void start_add_test(int *temp_arr) {
    DynamicArray<int> darray;

    for (int i = 0; i < N; i++) {
      darray.add(temp_arr[i]);
    }

    test_results["add"] = check_arrays<int>(darray.get_data(), temp_arr, N);
  }

  void start_shrink_test(int *temp_arr) {
    DynamicArray<int> darray;

    for (int i = 0; i < 7; i++) {
      darray.add(temp_arr[i]);
    }

    darray.shrink();

    test_results["shrink"] = check_arrays<int>(darray.get_data(), temp_arr,
                                               darray.get_element_count());
  }

  void start_index_test(int *temp_arr) {
    DynamicArray<int> darray;
    darray.expand(temp_arr, N);

    test_results["index"] = darray[5] == temp_arr[5];
    test_results["negative index"] = darray[-1] == temp_arr[N - 1];
  }

  void start_resize_test(int *temp_arr) {
    DynamicArray<int> darray(20);

    for (int i = 0; i < N; i++) {
      darray[i] = temp_arr[i];
    }

    test_results["resize"] = check_arrays<int>(darray.get_data(), temp_arr, N);
  }

private:
  int N = 10;
};
} // namespace pxd
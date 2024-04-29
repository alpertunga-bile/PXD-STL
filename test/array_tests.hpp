#pragma once

#include "array.hpp"
#include "test_utils.hpp"
namespace pxd {
class ArrayTests : public ITest {
public:
  void start_test() override {
    int *temp = new int[N];

    for (int i = 0; i < N; i++) {
      temp[i] = i;
    }

    start_init_with_array_test(temp);
    start_copy_to_test(temp);
    start_bool_compare_test();
    start_expand_test(temp);
    start_index_test(temp);
    start_resize_test(temp);
    start_copy_ctor_test(temp);
    start_move_ctor_test(temp);
    start_assign_ctor_test(temp);

    delete[] temp;
  }

private:
  void start_init_with_array_test(int *temp) {
    Array<int> arr(temp, N);

    test_results["init with array"] = check_arrays<int>(temp, arr.get_ptr(), N);
  }

  void start_copy_to_test(int *temp) {
    Array<int> arr(temp, N);

    Array<int> t(N);
    arr.copy_to(t);

    test_results["copy_to"] = check_arrays<int>(t.get_ptr(), arr.get_ptr(), N);
  }

  void start_bool_compare_test() {
    Array<int> arr(10);
    Array<int> temp(10);

    test_results["bool compare"] = arr != temp && arr == arr;
  }

  void start_expand_test(int *temp) {
    Array<int> arr(temp, N);
    Array<int> t(10);

    arr.copy_to(t);

    t.expand(arr);

    test_results["expand"] = check_arrays(t.get_ptr(), arr.get_ptr(), N) &&
                             check_arrays(t.get_ptr(), N, arr.get_ptr(), 0, N);
  }

  void start_index_test(int *temp) {
    Array<int> arr(temp, N);

    test_results["negative index"] = arr[-1] == arr[N - 1];
    test_results["index"] = arr[2] == 2;
  }

  void start_resize_test(int *temp) {
    int new_size = 20;
    Array<int> arr(temp, N);

    arr.resize(new_size);
    test_results["resize much"] = check_arrays<int>(temp, arr.get_ptr(), N);

    new_size = 5;

    arr.resize(new_size);
    test_results["resize less"] =
        check_arrays<int>(temp, arr.get_ptr(), new_size);
  }

  void start_copy_ctor_test(int *temp) {
    Array<int> arr(temp, N);

    Array<int> t(arr);

    test_results["copy constructor"] =
        check_arrays(arr.get_ptr(), t.get_ptr(), N);
  }

  void start_move_ctor_test(int *temp) {
    Array<int> arr(temp, N);

    Array<int> t(std::move(arr));

    test_results["move constructor"] =
        check_arrays(arr.get_ptr(), t.get_ptr(), N);
  }

  void start_assign_ctor_test(int *temp) {
    Array<int> arr(temp, N);

    Array<int> t = arr;

    test_results["assign constructor"] =
        check_arrays(arr.get_ptr(), t.get_ptr(), N);
  }

private:
  int N = 10;
};
} // namespace pxd
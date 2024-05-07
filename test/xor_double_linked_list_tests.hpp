#pragma once

#include "test_utils.hpp"
#include "xor_double_linked_list.hpp"

namespace pxd {
class XORDoubleLinkedListTests : public ITest {
public:
  void start_test() override {
    int *temp_arr = new int[N];
    int *check_arr = new int[N];

    for (int i = 0; i < N; i++) {
      temp_arr[i] = i + 1;
      check_arr[i] = 0;
    }

    start_add_test(temp_arr, check_arr);
    start_array_ctor_test(temp_arr, check_arr);
    start_copy_ctor_test(temp_arr, check_arr);
    start_move_ctor_test(temp_arr, check_arr);
    start_index_test(temp_arr);
    start_remove_test(temp_arr);

    delete[] temp_arr;
    delete[] check_arr;
  }

  void start_array_ctor_test(int *temp_arr, int *check_arr) {
    XORDoubleLinkedList<int> xdll(temp_arr, N);
    xdll.to_array(check_arr);

    test_results["array ctor"] = check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_copy_ctor_test(int *temp_arr, int *check_arr) {
    XORDoubleLinkedList<int> xdll(temp_arr, N);
    XORDoubleLinkedList<int> t_xdll(xdll);
    t_xdll.to_array(check_arr);

    test_results["copy ctor"] = check_arrays<int>(temp_arr, check_arr, N);

    t_xdll = xdll;
    t_xdll.to_array(check_arr);
    test_results["copy assign ctor"] =
        check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_move_ctor_test(int *temp_arr, int *check_arr) {
    XORDoubleLinkedList<int> xdll(temp_arr, N);
    XORDoubleLinkedList<int> t_xdll(std::move(xdll));
    t_xdll.to_array(check_arr);

    test_results["move ctor"] = check_arrays<int>(temp_arr, check_arr, N);

    XORDoubleLinkedList<int> t2_xdll = std::move(xdll);
    t2_xdll.to_array(check_arr);

    test_results["move assign ctor"] =
        check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_add_test(int *temp_arr, int *check_arr) {
    XORDoubleLinkedList<int> xdll;

    for (int i = 0; i < N; i++) {
      xdll.add(i + 1);
    }

    xdll.to_array(check_arr);

    test_results["add"] = check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_index_test(int *temp_arr) {
    XORDoubleLinkedList<int> xdll(temp_arr, N);

    test_results["index"] = xdll[2] == temp_arr[2] && xdll[8] == temp_arr[8];

    test_results["negative index"] =
        xdll[-1] == temp_arr[N - 1] && xdll[-7] && xdll[N - 7];
  }

  void start_remove_test(int *temp_arr) {
    XORDoubleLinkedList<int> xdll(temp_arr, N);

    xdll.remove(5);
    test_results["remove between"] = xdll[4] == 6;

    xdll.remove(1);
    test_results["remove head"] = xdll[0] == 2;

    xdll.remove(10);
    test_results["remove end"] = xdll[6] == 9;
  }

private:
private:
  int N = 10;
};
} // namespace pxd
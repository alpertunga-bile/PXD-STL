#pragma once

#include "double_linked_list.hpp"
#include "test_utils.hpp"

namespace pxd {
class DoubleLinkedListTests : public ITest {
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
    start_add_test(temp_arr, check_arr);
    start_from_array_test(temp_arr, check_arr);
    start_add_reverse_test(temp_arr, check_arr);
    start_reverse_test(temp_arr, check_arr);
    start_index_test(temp_arr);
    start_remove_test(temp_arr);
    start_where_test(temp_arr);

    delete[] temp_arr;
    delete[] check_arr;
  }

private:
  void start_array_ctor_test(int *temp_arr, int *check_arr) {
    DoubleLinkedList<int> dll(temp_arr, N);
    dll.to_array(check_arr);

    test_results["array ctor"] = check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_copy_ctor_test(int *temp_arr, int *check_arr) {
    DoubleLinkedList<int> dll(temp_arr, N);

    DoubleLinkedList<int> t_dll(dll);
    t_dll.to_array(check_arr);

    test_results["copy ctor"] = check_arrays<int>(temp_arr, check_arr, N);

    DoubleLinkedList<int> temp_dll = dll;
    temp_dll.to_array(check_arr);

    test_results["copy assign ctor"] =
        check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_move_ctor_test(int *temp_arr, int *check_arr) {
    DoubleLinkedList<int> dll(temp_arr, N);

    DoubleLinkedList<int> t_dll(std::move(dll));
    t_dll.to_array(check_arr);
    t_dll.release();

    test_results["move ctor"] = check_arrays<int>(temp_arr, check_arr, N);

    dll.from_array(temp_arr, N);

    DoubleLinkedList<int> temp_dll = std::move(dll);
    temp_dll.to_array(check_arr);

    test_results["move assign ctor"] =
        check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_add_test(int *temp_arr, int *check_arr) {
    DoubleLinkedList<int> dll;

    for (int i = 0; i < N; i++) {
      dll.add(i + 1);
    }

    dll.to_array(check_arr);

    test_results["add"] = check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_from_array_test(int *temp_arr, int *check_arr) {
    DoubleLinkedList<int> dll;
    dll.from_array(temp_arr, N);
    dll.to_array(check_arr);

    test_results["from array"] = check_arrays<int>(temp_arr, check_arr, N);
  }

  void start_add_reverse_test(int *temp_arr, int *check_arr) {
    DoubleLinkedList<int> dll;

    for (int i = 0; i < N; i++) {
      dll.add(i + 1, false);
    }

    dll.to_array(check_arr);

    test_results["add reverse"] =
        check_reverse_arrays<int>(temp_arr, check_arr, N);
  }

  void start_reverse_test(int *temp_arr, int *check_arr) {
    DoubleLinkedList<int> dll(temp_arr, N);
    dll.reverse();

    test_results["reverse"] = check_reverse_arrays<int>(temp_arr, check_arr, N);
  }

  void start_index_test(int *temp_arr) {
    DoubleLinkedList<int> dll(temp_arr, N);

    test_results["index"] = dll[2] == temp_arr[2] && dll[8] == temp_arr[8];

    test_results["negative index"] =
        dll[-1] == temp_arr[N - 1] && dll[-7] && dll[N - 7];
  }

  void start_remove_test(int *temp_arr) {
    DoubleLinkedList<int> dll(temp_arr, N);
    dll.remove(2);

    test_results["remove between"] = dll[1] == 3;

    dll.from_array(temp_arr, N);
    dll.remove_at(0);

    test_results["remove head"] = dll[0] == 2;

    dll.from_array(temp_arr, N);
    dll.remove_at(9);

    test_results["remove end"] = dll[8] == 9;

    dll.from_array(temp_arr, N);
    dll.remove_at(4);

    test_results["remove at"] = dll[4] == 6;
  }

  void start_where_test(int *temp_arr) {
    DoubleLinkedList<int> dll(temp_arr, N);

    test_results["where"] = dll.where(6) == 5;
    test_results["where 2"] = dll.where(5) == 4;
  }

private:
  int N = 10;
};
} // namespace pxd
#pragma once

#include "queue.hpp"
#include "test_utils.hpp"

namespace pxd {
class QueueTests : public ITest {
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
    start_peek_test(temp_arr);
    start_push_test();
    start_pop_test(temp_arr);
    start_reverse_test(temp_arr, check_arr);

    delete[] check_arr;
    delete[] temp_arr;
  }

private:
  void start_array_ctor_test(int *temp_arr, int *check_arr) {
    Queue<int> queue(temp_arr, N);
    queue.to_array(check_arr);

    test_results["array ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_copy_ctor_test(int *temp_arr, int *check_arr) {
    Queue<int> queue(temp_arr, N);
    Queue<int> q(queue);
    q.to_array(check_arr);

    test_results["copy ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_move_ctor_test(int *temp_arr, int *check_arr) {
    Queue<int> queue(temp_arr, N);
    Queue<int> q(std::move(queue));
    q.to_array(check_arr);

    test_results["move ctor"] = check_arrays(temp_arr, check_arr, N);

    Queue<int> temp;
    temp = std::move(q);
    temp.to_array(check_arr);

    test_results["assign move ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_assign_ctor_test(int *temp_arr, int *check_arr) {
    Queue<int> queue(temp_arr, N);
    Queue<int> q = queue;
    q.to_array(check_arr);

    test_results["assign ctor"] = check_arrays(temp_arr, check_arr, N);
  }

  void start_peek_test(int *temp_arr) {
    Queue<int> queue(temp_arr, N);
    int value = queue.peek();

    test_results["peek"] = value == temp_arr[0];
  }

  void start_push_test() {
    Queue<int> queue;
    queue.push(123);

    test_results["push"] = queue.peek() == 123;
  }

  void start_pop_test(int *temp_arr) {
    Queue<int> queue(temp_arr, N);
    int value = queue.pop();

    test_results["pop"] = value == 1 && queue.get_length() == N - 1;
  }

  void start_reverse_test(int *temp_arr, int *check_arr) {
    Queue<int> queue(temp_arr, N);
    queue.reverse();
    queue.to_array(check_arr);

    test_results["reverse"] = check_reverse_arrays(temp_arr, check_arr, N);
  }

private:
  int N = 10;
};
} // namespace pxd
#pragma once

#include "stack.hpp"
#include "test_utils.hpp"

namespace pxd {
class StackTests : public ITest {
public:
  void start_test() {
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

  void start_array_ctor_test(int *temp_arr, int *check_arr) {
    Stack<int> stack(temp_arr, N);
    stack.to_array(check_arr);

    test_results["array ctor"] = check_reverse_arrays(temp_arr, check_arr, N);
  }

  void start_copy_ctor_test(int *temp_arr, int *check_arr) {
    Stack<int> stack(temp_arr, N);
    Stack<int> st(stack);
    st.to_array(check_arr);

    test_results["copy ctor"] = check_reverse_arrays(temp_arr, check_arr, N);
  }

  void start_move_ctor_test(int *temp_arr, int *check_arr) {
    Stack<int> stack(temp_arr, N);
    Stack<int> st(std::move(stack));
    st.to_array(check_arr);

    test_results["move ctor"] = check_reverse_arrays(temp_arr, check_arr, N);
  }

  void start_assign_ctor_test(int *temp_arr, int *check_arr) {
    Stack<int> stack(temp_arr, N);
    Stack<int> st = stack;
    st.to_array(check_arr);

    test_results["assign ctor"] = check_reverse_arrays(temp_arr, check_arr, N);
  }

  void start_peek_test(int *temp_arr) {
    Stack<int> stack(temp_arr, N);
    int peek_value = stack.peek();

    test_results["peek"] = peek_value == temp_arr[N - 1];
  }

  void start_push_test() {
    Stack<int> stack;
    stack.push(123);

    int peek_value = stack.peek();

    test_results["push"] = peek_value == 123;
  }

  void start_pop_test(int *temp_arr) {
    Stack<int> stack(temp_arr, N);
    int value = stack.pop();

    test_results["pop"] =
        value == temp_arr[N - 1] && stack.get_length() == (N - 1);
  }

  void start_reverse_test(int *temp_arr, int *check_arr) {
    Stack<int> stack(temp_arr, N);
    stack.reverse();
    stack.to_array(check_arr);

    test_results["reverse"] = check_arrays(temp_arr, check_arr, N);
  }

private:
  int N = 10;
};
} // namespace pxd
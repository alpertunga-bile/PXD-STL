#pragma once

#include "priority_queue.hpp"
#include "test_utils.hpp"

namespace pxd {
class PriorityQueueTests : public ITest
{
public:
  void start_test() override
  {
    int* temp_arr = new int[N];
    for (int i = 0; i < N; i++) {
      temp_arr[i] = i + 1;
    }

    start_peek_test(temp_arr);
    start_top_test(temp_arr);
    start_update_test(temp_arr);
    start_remove_test(temp_arr);

    delete[] temp_arr;
  }

private:
  void start_peek_test(int* temp_arr)
  {
    PriorityQueue<int, 4, true> pq;

    for (int i = 0; i < N; i++) {
      pq.insert(temp_arr[i], temp_arr[i]);
    }

    test_results["peek"] = pq.peek() == N;
  }

  void start_top_test(int* temp_arr)
  {
    PriorityQueue<int, 4, true> pq;

    for (int i = 0; i < N; i++) {
      pq.insert(temp_arr[i], temp_arr[i]);
    }

    int val = pq.top();

    test_results["top"] = val == N;
  }

  void start_update_test(int* temp_arr)
  {
    PriorityQueue<int, 4, true> pq;

    for (int i = 0; i < N; i++) {
      pq.insert(temp_arr[i], temp_arr[i]);
    }

    pq.update_priority(1, 1234);

    test_results["update priority"] = pq.peek() == 1;
  }

  void start_remove_test(int* temp_arr)
  {
    PriorityQueue<int, 4, true> pq;

    for (int i = 0; i < N; i++) {
      pq.insert(temp_arr[i], temp_arr[i]);
    }

    pq.remove(10);

    test_results["remove"] = pq.peek() == 9;
  }

private:
  int N = 10;
};
}
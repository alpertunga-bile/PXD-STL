#pragma once

namespace pxd {

template <typename T, int D, bool is_max_heap> class PriorityQueue;

template <typename T> void swap(T &left, T &right) {
  T temp = left;
  left = right;
  right = temp;
}

template <typename T, int D = 4>
PriorityQueue<T, D, false> topK(T *values, int values_size, int k) {
  PriorityQueue<T, D, false> pq(k);

  for (int i = 0; i < values_size; i++) {
    if (pq.get_size() == k && pq.peek() < values[i]) {
      pq.top();
    }

    if (pq.get_size() < k) {
      pq.insert(values[i]);
    }
  }

  return pq;
}
} // namespace pxd
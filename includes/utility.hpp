#pragma once

#include <algorithm>
#include <vector>

namespace pxd {

constexpr int INDEX_NONE = -1;

template <typename T> class Array;
template <typename T> class DynamicArray;

template <typename T> class LinkedList;
template <typename T> class DoubleLinkedList;

template <typename T, int D, bool is_max_heap> class DHeap;
template <typename T, int D, bool is_max_heap> class PriorityQueue;

template <typename T> void swap(T &left, T &right) {
  T temp = left;
  left = right;
  right = temp;
}

constexpr inline double byte2kbyte(size_t size) { return size / 1024.0; }
constexpr inline double byte2mbyte(size_t size) {
  return size / (1024.0 * 1024.0);
}
constexpr inline double byte2gbyte(size_t size) {
  return size / (1024.0 * 1024.0 * 1024.0);
}
constexpr inline double kbyte2mbyte(size_t size) { return size / 1024.0; }
constexpr inline double kbyte2gbyte(size_t size) {
  return size / (1024.0 * 1024.0);
}
constexpr inline double mbyte2gbyte(size_t size) { return size / 1024.0; }

// return the top K elements in ascend order from a raw array
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// find functions

template <typename T> inline int find(Array<T> &&arr, T &&value) {
  const int length = arr.get_length();

  for (int i = 0; i < length; i++) {
    if (value == arr[i]) {
      return i;
    }
  }

  return INDEX_NONE;
}

template <typename T> inline int find(DynamicArray<T> &&arr, T &&value) {
  const int length = arr.get_element_count();

  for (int i = 0; i < length; i++) {
    if (value == arr[i]) {
      return i;
    }
  }

  return INDEX_NONE;
}

template <typename T> inline int find_index(std::vector<T> &&vec, T &&value) {
  auto &&it = std::find(vec.begin(), vec.end(), value);

  return it != vec.end() ? it - vec.begin() : INDEX_NONE;
}

template <typename T> inline int find(LinkedList<T> &&ll, T &&value) {
  auto &&current_node = ll.get_head_node();
  const int length = ll.get_length();

  for (int i = 0; i < length; i++) {
    if (value == current_node->value) {
      return i;
    }

    current_node = current_node->next;
  }

  return INDEX_NONE;
}

template <typename T> inline int find(DoubleLinkedList<T> &&dll, T &&value) {
  auto current_node = dll.get_head_node();
  const int length = dll.get_length();

  for (int i = 0; i < length; i++) {
    if (value == current_node->value) {
      return i;
    }

    current_node = current_node->next;
  }

  return INDEX_NONE;
}

template <typename T, int D = 4, bool is_max_heap = true>
inline int find(DHeap<T, D, is_max_heap> &&dheap, T &&value) {
  return find<T>(dheap.get_values(), std::forward<T>(value));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// append functions

// append raw array to the end of the vector
template <typename T>
inline void append(std::vector<T> &vec, T *array, int arr_size) {
  std::vector<T> temp_vec(array, arr_size);

  vec.reserve(vec.size() + arr_size);
  vec.insert(vec.end(), temp_vec.begin(), temp_vec.end());
}

template <typename T>
inline void append(std::vector<T> &vec, const std::vector<T> &given_vector) {
  vec.reserve(vec.size() + given_vector.size());
  vec.insert(vec.end(), given_vector.begin(), given_vector.end());
}

} // namespace pxd

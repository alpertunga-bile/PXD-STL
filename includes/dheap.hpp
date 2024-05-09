#pragma once

#include "checks.hpp"

namespace pxd {
template <typename T> class DynamicArray;
template <typename T> class Array;

template <typename T, int D = 4, bool is_max_heap = true> class DHeap {
public:
  DHeap() { values.resize(D + D * D + 1); }
  DHeap(int wanted_size) { values.resize(wanted_size); }
  DHeap(T *values, int size) {
    release();
    values.expand(values, size);
    heapify();
  }
  DHeap(Array<T> &values) {
    release();
    values.expand(values);
    heapify();
  }
  DHeap(DynamicArray<T> &values) {
    release();
    values.expand(values.get_array());
    heapify();
  }
  DHeap(const DHeap<T, D, is_max_heap> &other) = default;
  DHeap &operator=(const DHeap<T, D, is_max_heap> &other) = default;
  DHeap(DHeap<T, D, is_max_heap> &&other) = default;
  DHeap &operator=(DHeap<T, D, is_max_heap> &&other) = default;
  inline ~DHeap() noexcept { values.release(); }

  inline void release() noexcept { values.release(); }

  void insert(T &element) {
    values.add(element);
    ascend(values.get_element_count() - 1);
  }

  inline void insert(T &&element) { insert(element); }

  void remove(T &value) {
    const int current_length = values.get_element_count();

    int index = values.where(value);

    if (index < 0) {
      return;
    }

    if (index == 0 && current_length == 1) {
      values.release();
      return;
    }

    T *new_values = new T[current_length - 1];
    int node_index = 0;

    for (int i = 0; i < current_length; i++) {
      if (i == index) {
        continue;
      }

      new_values[node_index++] = values[i];
    }

    values.release();
    values.expand(new_values, current_length - 1);

    heapify();

    delete[] new_values;
  }
  inline void remove(T &&value) { remove(value); }

  void remove_at(int index) {
    const int current_length = values.get_element_count();

    if (index == 0 && current_length == 1) {
      values.release();
      return;
    }

    T *new_values = new T[current_length - 1];
    int node_index = 0;

    for (int i = 0; i < current_length; i++) {
      if (i == index) {
        continue;
      }

      new_values[node_index++] = values[i];
    }

    values.release();
    values.expand(new_values, current_length - 1);

    heapify();

    delete[] new_values;
  }

  T top() {
    PXD_ASSERT(values.get_element_count() > 0);

    T last_value = values.remove_last();

    if (values.get_element_count() == 0) {
      values.release();
      return last_value;
    }

    T root_node = values[0];
    values[0] = last_value;
    descend(0);

    return root_node.value;
  }

  inline T peek() {
    PXD_ASSERT(values.get_element_count() > 0);

    return values[0].value;
  }

  void update(T &value) {
    int index = values.where(value);

    if (index < 0) {
      return;
    }

    T old_value = values[index];
    values[index] = value;

    if (compare_bigger(value, old_value)) {
      ascend(index);
    } else if (compare_lower(value, old_value)) {
      descend(index);
    }
  }
  inline void update(T &&value) { update(value); }

  void update_at(int index, T &value) {
    T old_value = values[index];
    values[index] = value;

    if (compare_bigger(value, old_value)) {
      ascend(index);
    } else if (compare_lower(value, old_value)) {
      descend(index);
    }
  }

  inline void shrink() { values.shrink(); }
  inline int get_size() const { return values.get_element_count(); }
  inline T at(int index) { return values[index]; }

private:
  void heapify() {
    int i = (values.get_element_count() - 1) / D;

    for (; i >= 0; i--) {
      descend(i);
    }
  }

  void ascend(int index) {
    int current_index = index;
    T current_value = values[index];

    while (current_index > 0) {
      int parent_index = get_parent_index(current_index);

      if (compare_lower(values[parent_index], current_value)) {
        values[current_index] = values[parent_index];
        current_index = parent_index;
      } else {
        break;
      }
    }

    values[current_index] = current_value;
  }

  void descend(int index = 0) {
    int current_index = index;
    const int first_leaf_index = get_first_leaf_index();

    T current_value = values[current_index];
    T child_value;

    while (current_index < first_leaf_index) {
      int child_index = get_highest_priority_leaf(current_index);
      child_value = values[child_index];

      if (compare_bigger(child_value, current_value)) {
        values[current_index] = values[child_index];
        current_index = child_index;
      } else {
        break;
      }
    }

    values[current_index] = current_value;
  }

  inline int get_highest_priority_leaf(int parent_index) {
    const int child_index = D * parent_index;
    T child_value = values[child_index];
    int highest_child_index = child_index + 1;

    for (int i = 2; i < D; i++) {
      if (child_index + i >= values.get_element_count()) {
        return highest_child_index;
      }

      if (compare_bigger(values[child_index + i], child_value)) {
        child_value = values[child_index + i];
        highest_child_index = child_index + i;
      }
    }

    if ((D * parent_index + 1) >= values.get_element_count()) {
      return highest_child_index;
    }

    if ((compare_bigger(values[D * (parent_index + 1)], child_value))) {
      child_value = values[D * (parent_index + 1)];
      highest_child_index = D * (parent_index + 1);
    }

    return highest_child_index;
  }

  inline int get_parent_index(int index) noexcept { return (index - 1) / D; }
  inline int get_first_leaf_index() noexcept {
    return (values.get_element_count() - 2) / D + 1;
  }
  inline bool compare_lower(T &first_val, T &second_val) noexcept {
    return is_max_heap ? first_val < second_val : first_val > second_val;
  }
  inline bool compare_bigger(T &first_val, T &second_val) noexcept {
    return is_max_heap ? first_val > second_val : first_val < second_val;
  }

private:
  DynamicArray<T> values;
};
} // namespace pxd
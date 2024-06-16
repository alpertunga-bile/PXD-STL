#pragma once

#include "checks.hpp"
#include "utility.hpp"

namespace pxd {

template <typename T, int D = 4, bool is_max_heap = true> class DHeap {
public:
  DHeap() { values.reserve(D + D * D + 1); } // allocate the first 2 level
  DHeap(int wanted_size) { values.reserve(wanted_size); }
  DHeap(const DHeap<T, D, is_max_heap> &other) = default;
  DHeap &operator=(const DHeap<T, D, is_max_heap> &other) = default;
  DHeap(DHeap<T, D, is_max_heap> &&other) = default;
  DHeap &operator=(DHeap<T, D, is_max_heap> &&other) = default;
  inline ~DHeap() noexcept { release(); }

  inline void release() noexcept { values.clear(); }

  decltype(auto) operator[](int index) { return values[index]; }

  void insert(T &element) {
    values.push_back(element);
    ascend(values.size() - 1);
  }

  inline void insert(T &&element) { insert(element); }

  void remove(T &value) {
    const int current_length = values.size();

    int index = find_index(values, value);

    if (index < 0) {
      return;
    }

    if (index == 0 && current_length == 1) {
      values.clear();
      return;
    }

    values.erase(values.begin() + index);

    heapify();
  }

  inline void remove(T &&value) { remove(value); }

  void remove_at(int index) {
    if (index < 0) {
      return;
    }

    const size_t current_length = values.size();

    if (index == 0 && current_length == 1) {
      values.clear();
      return;
    }

    values.erase(values.begin() + index);
    heapify();
  }

  T top() {
    PXD_ASSERT(values.size() > 0);

    T last_value = values[values.size() - 1];
    values.erase(values.end() - 1);

    if (values.size() == 0) {
      values.clear();
      return last_value;
    }

    T root_node = values[0];
    values[0] = last_value;
    descend(0);

    return root_node;
  }

  inline T peek() {
    PXD_ASSERT(values.size() > 0);

    return values[0];
  }

  void update(T &value) {
    int index = find_index(values, value);

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

  inline int where(T &value) { return find_index(values, value); }

  inline void shrink() { values.shrink_to_fit(); }
  inline size_t get_size() const { return values.size(); }
  inline std::vector<T> get_values() { return values; }

private:
  void heapify() {
    int i = static_cast<int>((values.size() - 1) / D);

    for (; i >= 0; --i) {
      descend(i);
    }
  }

  void ascend(size_t index) {
    size_t current_index = index;
    T current_value = values[index];

    while (current_index > 0) {
      size_t parent_index = get_parent_index(current_index);

      if (compare_lower(values[parent_index], current_value)) {
        values[current_index] = values[parent_index];
        current_index = parent_index;
      } else {
        break;
      }
    }

    values[current_index] = current_value;
  }

  void descend(size_t index = 0) {
    size_t current_index = index;
    const size_t first_leaf_index = get_first_leaf_index();

    T current_value = values[current_index];
    T child_value;

    while (current_index < first_leaf_index) {
      size_t child_index = get_highest_priority_leaf(current_index);
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

  inline size_t get_highest_priority_leaf(size_t parent_index) {
    const size_t child_index = D * parent_index;
    T child_value = values[child_index];
    size_t highest_child_index = child_index + 1;

    for (int i = 2; i < D; i++) {
      if (child_index + i >= values.size()) {
        return highest_child_index;
      }

      if (compare_bigger(values[child_index + i], child_value)) {
        child_value = values[child_index + i];
        highest_child_index = child_index + i;
      }
    }

    if ((D * parent_index + 1) >= values.size()) {
      return highest_child_index;
    }

    if ((compare_bigger(values[D * (parent_index + 1)], child_value))) {
      child_value = values[D * (parent_index + 1)];
      highest_child_index = D * (parent_index + 1);
    }

    return highest_child_index;
  }

  inline size_t get_parent_index(size_t index) noexcept {
    return (index - 1) / D;
  }
  inline size_t get_first_leaf_index() noexcept {
    return (values.size() - 2) / D + 1;
  }
  inline bool compare_lower(T &first_val, T &second_val) noexcept {
    return is_max_heap ? first_val < second_val : first_val > second_val;
  }
  inline bool compare_bigger(T &first_val, T &second_val) noexcept {
    return is_max_heap ? first_val > second_val : first_val < second_val;
  }

private:
  std::vector<T> values;
};
} // namespace pxd
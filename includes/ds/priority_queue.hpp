#pragma once

#include "dheap.hpp"

namespace pxd {

template <typename T, int D = 4, bool is_max_heap = true> class PriorityQueue {
private:
  struct Node {
    T value;
    int priority = is_max_heap ? std::numeric_limits<int>::min()
                               : std::numeric_limits<int>::max();

    auto operator<(const Node &other) -> bool {
      return priority < other.priority;
    }
    auto operator>(const Node &other) -> bool {
      return priority > other.priority;
    }
    auto operator==(const Node &other) -> bool {
      return value == other.value && priority == other.priority;
    }
  };

public:
  void release() noexcept { nodes.release(); }

  void insert(T &element, int priority) {
    Node new_node;
    new_node.value = element;
    new_node.priority = is_max_heap ? priority : -1 * priority;

    nodes.insert(new_node);
  }

  constexpr void insert(T &&element, int priority) {
    insert(element, priority);
  }

  void remove(T &value) {
    int index = find_index(value);

    if (index < 0) {
      return;
    }

    nodes.remove_at(index);
  }

  constexpr void remove(T &&value) { remove(value); }

  auto top() -> T {
    PXD_ASSERT(nodes.get_size() > 0);

    Node last_element = nodes.top();
    return last_element.value;
  }

  auto peek() -> T {
    PXD_ASSERT(nodes.get_size() > 0);

    return nodes.peek().value;
  }

  void update_priority(T &value, int new_priority) {
    int index = find_index(value);

    if (index < 0) {
      return;
    }

    Node new_node;
    new_node.value = value;
    new_node.priority = new_priority;

    nodes.update_at(index, new_node);
  }

  void update_priority(T &&value, int new_priority) {
    update_priority(value, new_priority);
  }

  void to_array(T *array) {
    const int size = nodes.get_size();

    for (int i = 0; i < size; i++) {
      array[i] = nodes[i].value;
    }
  }

  void shrink() { nodes.shrink(); }
  constexpr auto get_size() -> size_t const { return nodes.get_size(); }

private:
  auto find_index(T &value) -> int {
    const size_t size = nodes.get_size();

    for (int i = 0; i < size; i++) {
      if (nodes[i].value == value) {
        return i;
      }
    }

    return -1;
  }

private:
  DHeap<Node, D, is_max_heap> nodes;
};
} // namespace pxd

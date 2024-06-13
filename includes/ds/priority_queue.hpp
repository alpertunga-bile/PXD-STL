#pragma once

#include "checks.hpp"

#include "dheap.hpp"

namespace pxd {
template <typename T, int D = 4, bool is_max_heap = true> class PriorityQueue {
private:
  struct Node {
    T value;
    int priority = is_max_heap ? INT_MIN : INT_MAX;

    inline bool operator<(const Node &other) {
      return priority < other.priority;
    }
    inline bool operator>(const Node &other) {
      return priority > other.priority;
    }
    inline bool operator==(const Node &other) {
      return value == other.value && priority == other.priority;
    }
  };

public:
  inline void release() noexcept { nodes.release(); }

  void insert(T &element, int priority) {
    Node new_node;
    new_node.value = element;
    new_node.priority = is_max_heap ? priority : -1 * priority;

    nodes.insert(new_node);
  }

  inline void insert(T &&element, int priority) { insert(element, priority); }

  void remove(T &value) {
    int index = find_index(value);

    if (index < 0) {
      return;
    }

    nodes.remove_at(index);
  }

  inline void remove(T &&value) { remove(value); }

  T top() {
    PXD_ASSERT(nodes.get_size() > 0);

    Node last_element = nodes.top();
    return last_element.value;
  }

  inline T peek() {
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

  void to_array(T *array) {
    const int size = nodes.get_size();

    for (int i = 0; i < size; i++) {
      array[i] = nodes[i].value;
    }
  }

  inline void update_priority(T &&value, int new_priority) {
    update_priority(value, new_priority);
  }

  inline void shrink() { nodes.shrink(); }
  inline int get_size() const { return nodes.get_size(); }

private:
  int find_index(T &value) {
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

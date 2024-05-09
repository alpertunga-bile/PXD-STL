#pragma once

#include "checks.hpp"

namespace pxd {

template <typename T> class DynamicArray;

template <typename T, int D = 3, bool is_max_heap = true> class PriorityQueue {
private:
  struct Node {
    T value;
    int priority = is_max_heap ? INT_MIN : INT_MAX;
  };

public:
  void insert(T &element, int priority) {
    Node new_node;
    new_node.value = element;
    new_node.priority = is_max_heap ? priority : -1 * priority;

    nodes.add(new_node);

    ascend_node(nodes.get_element_count() - 1);
  }

  inline void insert(T &&element, int priority) { insert(element, priority); }

  void remove(T &value) {
    const int current_length = nodes.get_element_count();

    int index;
    find(value, index, 0, current_length);

    if (index < 0) {
      return;
    }

    if (index == 0 && current_length == 1) {
      nodes.release();
    }

    Node *new_nodes = new Node[current_length - 1];
    int node_index = 0;

    for (int i = 0; i < current_length; i++) {
      if (nodes[i].value == value) {
        continue;
      }

      new_nodes[node_index++] = nodes[i];
    }

    nodes.release();
    nodes.expand(new_nodes, current_length - 1);

    heapify();

    delete[] new_nodes;
  }

  inline void remove(T &&value) { remove(value); }

  T top() {
    PXD_ASSERT(nodes.get_element_count() > 0);

    Node last_element = nodes.remove_last();

    if (nodes.get_element_count() == 0) {
      nodes.release();
      return last_element.value;
    }

    Node root_node = nodes[0];
    nodes[0] = last_element;
    descend_node(0);

    return root_node.value;
  }

  inline T peek() {
    PXD_ASSERT(nodes.get_element_count() > 0);

    return nodes[0].value;
  }

  void update_priority(T &value, int new_priority) {
    int index;
    find(value, index, 0, nodes.get_element_count() - 1);

    if (index < 0) {
      return;
    }

    int old_priority = nodes[index].priority;
    nodes[index].value = value;
    nodes[index].priority = new_priority;

    if (new_priority > old_priority) {
      ascend_node(index);
    } else if (new_priority < old_priority) {
      descend_node(index);
    }
  }

  inline void update_priority(T &&value, int new_priority) {
    update_priority(value, new_priority);
  }

private:
  void heapify() {
    int i = (nodes.get_element_count() - 1) / D;

    for (; i >= 0; i--) {
      descend_node(i);
    }
  }

  void ascend_node(int index) {
    int current_index = index;
    Node current_node = nodes[index];

    while (current_index > 0) {
      int parent_index = get_parent_index(current_index);

      if (nodes[parent_index].priority < current_node.priority) {
        nodes[current_index] = nodes[parent_index];
        current_index = parent_index;
      } else {
        break;
      }
    }

    nodes[current_index] = current_node;
  }

  void descend_node(int index = 0) {
    int current_index = index;
    const int first_leaf_index = get_first_leaf_index();

    Node current_node = nodes[current_index];
    Node child_node;

    while (current_index < first_leaf_index) {
      int child_index = get_highest_priority_leaf(current_index);
      child_node = nodes[child_index];

      if (child_node.priority > current_node.priority) {
        nodes[current_index] = nodes[child_index];
        current_index = child_index;
      } else {
        break;
      }
    }

    nodes[current_index] = current_node;
  }

  inline int get_highest_priority_leaf(int parent_index) {
    const int child_index = D * parent_index;
    Node child_node = nodes[child_index];
    int highest_child_index = child_index + 1;

    for (int i = 2; i < D; i++) {
      if (child_index + i >= nodes.get_element_count()) {
        return highest_child_index;
      }

      if (nodes[child_index + i].priority > child_node.priority) {
        child_node = nodes[child_index + i];
        highest_child_index = child_index + i;
      }
    }

    if ((D * parent_index + 1) >= nodes.get_element_count()) {
      return highest_child_index;
    }

    if (nodes[D * (parent_index + 1)].priority > child_node.priority) {
      child_node = nodes[D * (parent_index + 1)];
      highest_child_index = D * (parent_index + 1);
    }

    return highest_child_index;
  }

  inline int get_parent_index(int index) noexcept { return (index - 1) / D; }
  inline int get_first_leaf_index() noexcept {
    return (nodes.get_element_count() - 2) / D + 1;
  }

  void find(T &value, int &index, int start, int end) {
    if (start > end) {
      index = -1;
      return;
    }

    int mid = (end - start) / 2;
    if (nodes[mid].value == value) {
      index = mid;
      return;
    }

    find(value, index, start, mid - 1);
    find(value, index, mid + 1, end);
  }

private:
  DynamicArray<Node> nodes;
};
} // namespace pxd

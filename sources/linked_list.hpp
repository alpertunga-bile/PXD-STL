#pragma once

#include "checks.hpp"

namespace pxd {
template <typename T> class LinkedList {
private:
  struct Node {
    T value;
    Node *next = nullptr;
  };

public:
  LinkedList() {
    head = nullptr;
    end = nullptr;
    length = 0;
  } // default constructor
  LinkedList(T *node_list, int size) {}

  LinkedList(const LinkedList<T> &other) = delete;
  LinkedList(LinkedList<T> &&other) = delete;
  LinkedList &operator=(const LinkedList<T> &other) = delete;

  ~LinkedList() {
    if (head == nullptr) {
      return;
    }

    Node *current_node = head;

    do {
      Node *prev_node = current_node;
      current_node = current_node->next;
      delete prev_node;
    } while (current_node != nullptr);

    head = nullptr;
    end = nullptr;
  }

  decltype(auto) operator[](int index) {
    PXD_ASSERT(index < length);

    int calc_index = index;

    if (index < 0) {
      PXD_ASSERT((index * -1) <= length);
      calc_index = length - (index * -1);
    }

    Node *current_node = head;

    for (int i = 0; i < calc_index; i++) {
      current_node = head->next;
    }

    return current_node->value;
  }

  void release() {
    if (head == nullptr) {
      return;
    }

    Node *current_node = head;

    do {
      Node *prev_node = current_node;
      current_node = current_node->next;
      delete prev_node;
    } while (current_node != nullptr);

    head = nullptr;
    end = nullptr;
  }

  // for left values
  void add(T &new_value, bool add_back = true) {
    Node *new_node = new Node();
    new_node->value = new_value;

    if (add_back) {
      add_to_back(new_node);
    } else {
      add_to_front(new_node);
    }
  }

  // for right values
  void add(T &&new_value, bool add_back = true) {
    Node *new_node = new Node();
    new_node->value = new_value;

    if (add_back) {
      add_to_back(new_node);
    } else {
      add_to_front(new_node);
    }
  }

  void to_array(T *array) {
    if (is_empty()) {
      return;
    }

    Node *current_node = head;
    int index = 0;

    do {
      array[index] = current_node->value;
      current_node = head->next;
      index++;
    } while (index != length);
  }

  void print() {
    if (is_empty()) {
      return;
    }

    Node *current_node = head;

    do {
      std::cout << "    -> " << current_node->value << "\n";
      current_node = current_node->next;
    } while (current_node != nullptr);
  }

  inline int get_length() { return length; }
  inline bool is_empty() {
    return head == nullptr && length == 0 ? true : false;
  }

private:
  void add_to_back(Node *new_node) {
    if (head == nullptr) {
      head = new_node;
      end = new_node;
    } else {
      end->next = new_node;
      end = new_node;
    }

    length++;
  }

  void add_to_front(Node *new_node) {
    if (head == nullptr) {
      head = new_node;
      end = new_node;
    } else {
      Node *temp = head;
      head = new_node;
      new_node->next = temp;

      end = temp;
    }

    length++;
  }

private:
  Node *head = nullptr;
  Node *end = nullptr;
  int length = 0;
};
} // namespace pxd
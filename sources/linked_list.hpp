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
  LinkedList(T *node_list, int size, bool is_reverse = false) {
    from_array(node_list, size, is_reverse);
  }

  LinkedList(const LinkedList<T> &other) { from_linked_list(other); }
  LinkedList(LinkedList<T> &&other) = delete;
  LinkedList &operator=(const LinkedList<T> &other) {
    from_linked_list(other);

    return *this;
  }

  ~LinkedList() { release(); }

  T &operator[](int index) {
    PXD_ASSERT(index < length);

    int calc_index = index;

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      calc_index = length + index;
    }

    Node *current_node = head;

    for (int i = 0; i < calc_index; i++) {
      current_node = current_node->next;
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

  void from_array(T *node_list, int size, bool is_reverse = false) {
    head = nullptr;
    end = nullptr;
    length = 0;

    for (int i = 0; i < size; i++) {
      Node *new_node = new Node();
      new_node->value = node_list[i];

      if (is_reverse) {
        add_to_front(new_node);
      } else {
        add_to_back(new_node);
      }
    }
  }

  void reverse() {
    if (is_empty() && length == 1) {
      return;
    }

    // O(n) for memory and speed
    T *temp_array = new T[length];

    to_array(temp_array);
    from_array(temp_array, length, true);

    delete[] temp_array;

    // O(nk) for speed, O(1) for memory
    /*
    Node *temp_head = end;
    Node *temp_end = head;

    Node *current_node = get_node_at(-2);
    temp_head->next = current_node;

    for (int index = 3; index < length; index++) {
      Node *next_node = get_node_at(-1 * index);
      current_node->next = next_node;
      current_node = next_node;
    }

    current_node->next = temp_end;

    head = temp_head;
    end = temp_end;

    end->next = nullptr;
    */
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
      current_node = current_node->next;
      index++;
    } while (index != length);
  }

  void to_array(T *array) const {
    if (is_empty()) {
      return;
    }

    Node *current_node = head;
    int index = 0;

    do {
      array[index] = current_node->value;
      current_node = current_node->next;
      index++;
    } while (index != length);
  }

  inline int get_length() { return length; }
  inline bool is_empty() {
    return head == nullptr && length == 0 ? true : false;
  }
  inline Node *get_head_node() { return head; }
  inline Node *get_end_node() { return end; }
  inline void set_head_node(Node *_head) { head = _head; }
  inline void set_end_node(Node *_end) { end = _end; }

  inline int get_length() const { return length; }
  inline bool is_empty() const {
    return head == nullptr && length == 0 ? true : false;
  }
  inline Node *get_head_node() const { return head; }
  inline Node *get_end_node() const { return end; }
  inline void set_head_node(Node *_head) const { head = _head; }
  inline void set_end_node(Node *_end) const { end = _end; }

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
    }

    length++;
  }

  Node *get_node_at(int index) {
    PXD_ASSERT(index < length);

    int calc_index = index;

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      calc_index = length + index;
    }

    Node *current_node = head;

    for (int i = 0; i < calc_index; i++) {
      current_node = current_node->next;
    }

    return current_node;
  }

  void from_linked_list(const LinkedList<T> &other) {
    release();

    Node *this_current_node = new Node();
    Node *other_current_node = other.get_head_node();

    this_current_node->value = other_current_node->value;
    head = this_current_node;
    other_current_node = other_current_node->next;

    length = other.get_length();

    for (int i = 1; i < length; i++) {
      Node *new_node = new Node();
      new_node->value = other_current_node->value;

      this_current_node->next = new_node;

      this_current_node = new_node;
      other_current_node = other_current_node->next;
    }

    end = this_current_node;
  }

private:
  Node *head = nullptr;
  Node *end = nullptr;
  int length = 0;
};
} // namespace pxd
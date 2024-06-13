#pragma once

#include "checks.hpp"

namespace pxd {

template <typename T> class Array;
template <typename T> class DoubleLinkedList;
template <typename T> class DynamicArray;

template <typename T> class LinkedList {
public:
  struct Node {
    T value;
    Node *next = nullptr;
  };

public:
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors

  constexpr LinkedList() noexcept {
    head = nullptr;
    end = nullptr;
    length = 0;
  } // default constructor
  LinkedList(T *node_list, int size, bool is_reverse = false) {
    from_array(node_list, size, is_reverse);
  }
  LinkedList(Array<T> &node_list, bool is_reverse = false) {
    from_array(node_list, is_reverse);
  }
  LinkedList(DynamicArray<T> &node_list, bool is_reverse = false) {
    from_array(node_list, is_reverse);
  }
  LinkedList(DoubleLinkedList<T> &node_list, bool is_reverse = false) {
    from_double_linked_list(node_list, is_reverse);
  }
  constexpr LinkedList(const LinkedList<T> &other) { from_linked_list(other); }
  constexpr LinkedList(LinkedList<T> &&other) noexcept {
    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();
    other.exec_move();
  }
  constexpr LinkedList &operator=(LinkedList<T> &&other) {
    release();

    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();

    other.exec_move();

    return *this;
  }
  constexpr LinkedList &operator=(const LinkedList<T> &other) {
    from_linked_list(other);

    return *this;
  }
  inline ~LinkedList() noexcept { release(); }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Operator Overloads

  T &operator[](int index) noexcept {
    const int calc_index = get_calc_index(index);

    Node *current_node = head;

    for (int i = 0; i < calc_index; i++) {
      current_node = current_node->next;
    }

    return current_node->value;
  }

  // actually; head, end and length will be same and inner nodes may be
  // different if the user tries hard enough but whatever
  constexpr bool operator==(LinkedList<T> &other) noexcept {
    return head == other.get_head_node() && end == other.get_end_node() &&
                   length == other.get_length()
               ? true
               : false;
  }

  constexpr bool operator!=(LinkedList<T> &other) noexcept {
    return head != other.get_head_node() && end != other.get_end_node() &&
                   length != other.get_length()
               ? true
               : false;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // DS Functionalities

  inline void release() noexcept {
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
    length = 0;
  }

  void reverse() noexcept {
    if (is_empty() || length == 1) {
      return;
    }

    Node *current_node = head->next;

    Node *prev_node = head;
    prev_node->next = nullptr;
    end = prev_node;

    Node *forw_node = nullptr;

    while (current_node != nullptr) {
      forw_node = current_node->next;
      current_node->next = prev_node;

      prev_node = current_node;
      current_node = forw_node;
    }

    head = prev_node;
  }

  // returns length because negative indexing is supported
  // so this will lead to unwanted situations
  constexpr int where(T &value) noexcept {
    Node *current_node = head;
    int index = length;

    for (int i = 0; i < length; i++) {
      if (current_node->value == value) {
        index = i;
        break;
      }

      current_node = current_node->next;
    }

    return index;
  }

  // for left values
  inline void add(T &new_value, bool add_back = true) noexcept {
    Node *new_node = new Node();
    new_node->value = new_value;

    if (head == nullptr) {
      head = new_node;
      end = new_node;
      length++;
      return;
    }

    if (add_back) {
      end->next = new_node;
      end = new_node;
    } else {
      new_node->next = head;
      head = new_node;
    }

    length++;
  }

  // for right values
  inline void add(T &&new_value, bool add_back = true) {
    add(new_value, add_back);
  }

  void remove_at(int index) {
    if (head == nullptr) {
      return;
    }

    int calc_index = get_calc_index(index);

    Node *prev_node = nullptr;
    Node *current_node = head;

    for (int i = 0; i < calc_index; i++) {
      prev_node = current_node;
      current_node = current_node->next;
    }

    remove_node(prev_node, current_node);
  }

  void remove(T &value) noexcept {
    if (head == nullptr) {
      return;
    }

    Node *prev_node = nullptr;
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      if (current_node->value == value) {
        break;
      }

      prev_node = current_node;
      current_node = current_node->next;
    }

    remove_node(prev_node, current_node);
  }

  void remove(T &&value) noexcept { remove(value); }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // From Functions

  inline void from_array(T *node_list, int size, bool is_reverse = false) {
    release();

    for (int i = 0; i < size; i++) {
      add(node_list[i], !is_reverse);
    }
  }

  inline void from_array(Array<T> &node_list, bool is_reverse = false) {
    release();

    const int size = node_list.get_length();

    for (int i = 0; i < size; i++) {
      add(node_list[i], !is_reverse);
    }
  }

  inline void from_array(DynamicArray<T> &node_list, bool is_reverse = false) {
    release();

    const int size = node_list.get_element_count();

    for (int i = 0; i < size; i++) {
      add(node_list[i], !is_reverse);
    }
  }

  inline void from_double_linked_list(DoubleLinkedList<T> &double_linked_list,
                                      bool is_reverse = false) {
    release();

    const int size = double_linked_list.get_length();

    for (int i = 0; i < size; i++) {
      add(double_linked_list[i], !is_reverse);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // To Functions

  void to_array(T *array) {
    if (is_empty()) {
      return;
    }

    fill_array(array);
  }

  void to_array(Array<T> &array) {
    if (is_empty()) {
      return;
    }

    array.reallocate(length);

    fill_array(array);
  }

  void to_array(DynamicArray<T> &array) {
    if (is_empty()) {
      return;
    }

    array.reallocate(length);

    fill_array(array);
  }

  void to_double_linked_list(DoubleLinkedList<T> &dll) noexcept {
    if (is_empty()) {
      return;
    }

    dll.release();

    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      dll.add(current_node->value);
      current_node = current_node->next;
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Inline Member Funcs

  constexpr inline size_t get_byte_size() const noexcept {
    return length * sizeof(T);
  }
  constexpr inline size_t get_mbyte_size() const noexcept {
    return (length * sizeof(T)) / 1024.f;
  }
  constexpr inline size_t get_gbyte_size() const noexcept {
    return (length * sizeof(T)) / (1024.f * 1024.f);
  }
  constexpr inline size_t get_data_size() const noexcept { return sizeof(T); }

  constexpr inline int get_length() const noexcept { return length; }
  constexpr inline bool is_empty() const noexcept {
    return head == nullptr && length == 0 ? true : false;
  }
  constexpr inline Node *get_head_node() const noexcept { return head; }
  constexpr inline Node *get_end_node() const noexcept { return end; }
  constexpr inline void exec_move() noexcept {
    head = nullptr;
    end = nullptr;
    length = 0;
  }

private:
  constexpr void from_linked_list(const LinkedList<T> &other) {
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

  constexpr inline int get_calc_index(int index) {
    PXD_ASSERT(index < length);

    int calc_index = index;

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      calc_index = length + index;
    }

    return calc_index;
  }

  inline void fill_array(T *array) const noexcept {
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  inline void fill_array(Array<T> &array) noexcept {
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  inline void fill_array(DynamicArray<T> &array) noexcept {
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  inline void remove_node(Node *prev_node, Node *current_node) noexcept {
    if (current_node == nullptr) {
      return;
    }

    if (current_node == head) {
      head = current_node->next;
    } else if (current_node == end) {
      end = prev_node;
    }

    if (prev_node != nullptr) {
      prev_node->next = current_node->next;
    }

    delete current_node;

    if (head == nullptr) {
      end = nullptr;
    }

    length--;
  }

private:
  Node *head = nullptr;
  Node *end = nullptr;
  int length = 0;
};
} // namespace pxd
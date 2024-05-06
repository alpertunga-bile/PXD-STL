#pragma once

#include "checks.hpp"

namespace pxd {

template <typename T> class Array;

template <typename T> class LinkedList {
private:
  struct Node {
    T value;
    Node *next = nullptr;
  };

public:
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors & Deconstructor

  constexpr LinkedList() noexcept {
    head = nullptr;
    end = nullptr;
    length = 0;
  } // default constructor
  constexpr LinkedList(T *node_list, int size, bool is_reverse = false) {
    from_array(node_list, size, is_reverse);
  }
  constexpr LinkedList(Array<T> &node_list, bool is_reverse = false) {
    from_array(node_list.get_ptr(), node_list.get_length(), is_reverse);
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

  constexpr T &operator[](int index) noexcept {
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
  // Class Functionalities

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

  constexpr void reverse() noexcept {
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
    int current_index = 0;

    do {
      if (current_node->value == value) {
        return current_index;
      }
      current_node = current_node->next;
      current_index++;
    } while (current_node != nullptr);

    return length;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Element Functions

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
  constexpr void add(T &&new_value, bool add_back = true) {
    add(new_value, add_back);
  }

  constexpr void remove_at(int index) {
    int calc_index = get_calc_index(index);

    if (head == nullptr) {
      return;
    }

    if (calc_index == 0) {
      Node *new_head = head->next;
      delete head;
      head = new_head;
    } else if (calc_index == length - 1) {
      Node *new_end = get_node_at(-2);
      delete end;
      end = new_end;
      end->next = nullptr;
    } else {
      Node *current_node = head;
      const int to_reach = index - 1;

      for (int i = 1; i < to_reach; i++) {
        current_node = current_node->next;
      }

      Node *prev_node = current_node;
      Node *node_to_delete = prev_node->next;
      Node *new_neighbor = node_to_delete->next;

      prev_node->next = new_neighbor;
      delete node_to_delete;
    }

    if (head == nullptr) {
      end = nullptr;
    }

    length--;
  }

  void remove(T &value) {
    int remove_index = where(value);

    PXD_ASSERT(remove_index < length);

    remove_at(remove_index);
  }

  constexpr void remove(T &&value) { remove(value); }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Conversions

  void to_array(T *array) const {
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

  Array<T> to_array() {
    if (is_empty()) {
      return;
    }

    Array<T> array(length);

    fill_array(array);

    return array;
  }

  void from_array(T *node_list, int size, bool is_reverse = false) {
    head = nullptr;
    end = nullptr;
    length = 0;

    for (int i = 0; i < size; i++) {
      add(node_list[i], !is_reverse);
    }
  }

  void from_array(Array<T> &node_list, bool is_reverse = false) {
    head = nullptr;
    end = nullptr;
    length = 0;

    const int size = node_list.get_length();

    for (int i = 0; i < size; i++) {
      add(node_list[i], !is_reverse);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Variable Inline Functions

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
  constexpr Node *get_node_at(int index) {
    int calc_index = get_calc_index(index);

    Node *current_node = head;

    for (int i = 0; i < calc_index; i++) {
      current_node = current_node->next;
    }

    return current_node;
  }

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

  constexpr void from_linked_list(LinkedList<T> &&other) {
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

  void fill_array(T *array) const noexcept {
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  void fill_array(Array<T> &array) noexcept {
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

private:
  Node *head = nullptr;
  Node *end = nullptr;
  int length = 0;
};
} // namespace pxd
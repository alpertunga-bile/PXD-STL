#pragma once

#include "checks.hpp"

namespace pxd {

template <typename T> class LinkedList;
template <typename T> class Array;
template <typename T> class DynamicArray;

template <typename T> class DoubleLinkedList {
private:
  struct Node {
    T value;
    Node *next = nullptr;
    Node *prev = nullptr;
  };

public:
  constexpr DoubleLinkedList() noexcept {
    head = nullptr;
    end = nullptr;
    length = 0;
  }
  DoubleLinkedList(T *array, int size, bool add_back = true) {
    from_array(array, size, add_back);
  }
  DoubleLinkedList(Array<T> &array, bool add_back = true) {
    from_array(array, add_back);
  }
  DoubleLinkedList(DynamicArray<T> &array, bool add_back = true) {
    from_array(array, add_back);
  }
  DoubleLinkedList(LinkedList<T> &linked_list, bool add_back = true) {
    from_linked_list(linked_list, add_back);
  }
  DoubleLinkedList(const DoubleLinkedList<T> &other) {
    from_double_linked_list(other);
  }
  DoubleLinkedList &operator=(const DoubleLinkedList<T> &other) {
    from_double_linked_list(other);
    return *this;
  }
  constexpr DoubleLinkedList(DoubleLinkedList<T> &&other) {
    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();
    other.exec_move();
  }
  constexpr DoubleLinkedList &operator=(DoubleLinkedList<T> &&other) {
    release();

    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();

    other.exec_move();

    return *this;
  }
  inline ~DoubleLinkedList() noexcept { release(); }

  T &operator[](int index) noexcept {
    bool is_negative = false;
    int calc_index = get_calc_min_index(index, is_negative);
    Node *current_node = nullptr;

    if (!is_negative) {
      current_node = head;

      for (int i = 0; i < calc_index; i++) {
        current_node = current_node->next;
      }
    } else {
      current_node = end;

      for (int i = 1; i < calc_index; i++) {
        current_node = current_node->prev;
      }
    }

    return current_node->value;
  }

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

  void add(T &value, bool add_back = true) noexcept {
    Node *new_node = new Node();
    new_node->value = value;

    if (head == nullptr) {
      head = new_node;
      end = new_node;
      length++;
      return;
    }

    if (add_back) {
      end->next = new_node;
      new_node->prev = end;
      end = new_node;
    } else {
      new_node->next = head;
      head = new_node;
    }

    length++;
  }

  void add(T &&value, bool add_back = true) noexcept { add(value, add_back); }

  void remove_at(int index) noexcept {
    if (head == nullptr || index < 0 || index >= length) {
      return;
    }

    Node *current_node = get_node_at(index);

    remove_node(current_node);
  }

  void remove(T &value) noexcept {
    if (head == nullptr) {
      return;
    }

    Node *current_node = nullptr;

    Node *from_head = head;
    Node *from_end = end;

    int head_index = 0;
    int end_index = length - 1;

    do {
      if (from_head->value == value) {
        current_node = from_head;
        break;
      }

      if (from_end->value == value) {
        current_node = from_end;
        break;
      }

      from_head = from_head->next;
      from_end = from_end->prev;
      head_index++;
      end_index--;
    } while (head_index <= end_index);

    remove_node(current_node);
  }

  void remove(T &&value) noexcept { remove(value); }

  void reverse() noexcept {
    if (head == nullptr || length == 1) {
      return;
    }

    Node *current_node = head;
    Node *temp_node = nullptr;

    end = current_node;

    while (current_node->next != nullptr) {
      temp_node = current_node->next;
      current_node->next = current_node->prev;
      current_node->prev = temp_node;

      current_node = temp_node;
    }

    temp_node = current_node->next;
    current_node->next = current_node->prev;
    current_node->prev = temp_node;

    head = current_node;
  }

  inline void from_array(T *array, int size, bool add_back = true) noexcept {
    release();

    for (int i = 0; i < size; i++) {
      add(array[i], add_back);
    }
  }

  inline void from_array(Array<T> &array, bool add_back = true) noexcept {
    release();
    const int size = array.get_length();

    for (int i = 0; i < size; i++) {
      add(array[i], add_back);
    }
  }

  inline void from_array(DynamicArray<T> &array,
                         bool add_back = true) noexcept {
    release();
    const int size = array.get_element_count();

    for (int i = 0; i < size; i++) {
      add(array[i], add_back);
    }
  }

  inline void from_linked_list(LinkedList<T> &linked_list,
                               bool add_back = true) noexcept {
    release();
    const int size = linked_list.get_length();

    for (int i = 0; i < size; i++) {
      add(linked_list[i], add_back);
    }
  }

  inline void to_array(T *array) noexcept {
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  inline void to_array(Array<T> &array) noexcept {
    array.reallocate(length);
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  inline void to_array(DynamicArray<T> &array) noexcept {
    array.reallocate(length);
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  inline void to_linked_list(LinkedList<T> &linked_list) noexcept {
    linked_list.release();

    Node *current_node = head;

    for (int i = 0; i < length; i++) {
      linked_list.add(current_node->value);
      current_node = current_node->next;
    }
  }

  int where(T &value) noexcept {
    Node *from_head = head;
    Node *from_end = end;

    int head_index = 0;
    int end_index = length - 1;
    int index = length;

    do {
      if (from_head->value == value) {
        index = head_index;
        break;
      }

      if (from_end->value == value) {
        index = end_index;
        break;
      }

      from_head = from_head->next;
      from_end = from_end->prev;
      head_index++;
      end_index--;
    } while (head_index <= end_index);

    return index;
  }

  int where(T &&value) noexcept { return where(value); }

  inline Node *get_head_node() const noexcept { return head; }
  inline Node *get_end_node() const noexcept { return end; }
  inline int get_length() const noexcept { return length; }

  constexpr void exec_move() noexcept {
    head = nullptr;
    end = nullptr;
    length = 0;
  }

private:
  inline int get_calc_min_index(int &given_index,
                                bool &is_negative) const noexcept {
    int positive_index = given_index >= 0 ? given_index : length + given_index;
    int negative_index =
        given_index < 0 ? given_index * -1 : length - given_index;

    is_negative = positive_index <= negative_index ? false : true;

    return is_negative ? negative_index : positive_index;
  }

  inline Node *get_node_at(int &index) noexcept {
    bool is_negative = false;
    int calc_index = get_calc_min_index(index, is_negative);
    Node *current_node = nullptr;

    if (!is_negative) {
      current_node = head;

      for (int i = 0; i < calc_index; i++) {
        current_node = current_node->next;
      }
    } else {
      current_node = end;

      for (int i = 1; i < calc_index; i++) {
        current_node = current_node->prev;
      }
    }

    return current_node;
  }

  inline void remove_node(Node *current_node) noexcept {
    if (current_node == nullptr) {
      return;
    }

    if (current_node == head) {
      head = current_node->next;
    } else if (current_node == end) {
      end = current_node->prev;
    }

    Node *prev_node = current_node->prev;
    Node *forw_node = current_node->next;

    if (prev_node != nullptr) {
      prev_node->next = forw_node;
    }

    if (forw_node != nullptr) {
      forw_node->prev = prev_node;
    }

    delete current_node;

    length--;

    if (head == nullptr) {
      end = nullptr;
    }
  }

  inline void from_double_linked_list(const DoubleLinkedList<T> &other) {
    release();

    const int other_length = other.get_length();

    Node *current_node = other.get_head_node();

    for (int i = 0; i < other_length; i++) {
      add(current_node->value);
      current_node = current_node->next;
    }
  }

private:
  Node *head = nullptr;
  Node *end = nullptr;
  int length = 0;
};
} // namespace pxd
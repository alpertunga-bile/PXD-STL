#pragma once

#include "checks.hpp"

#include <cstdint>

namespace pxd {
template <typename T> class XORDoubleLinkedList {
private:
  struct Node {
    T value;
    Node *dir = nullptr;
  };

public:
  XORDoubleLinkedList() = default;
  XORDoubleLinkedList(T *array, int size) { from_array(array, size); }
  XORDoubleLinkedList(const XORDoubleLinkedList<T> &other) {
    from_xor_dll(other);
  }
  XORDoubleLinkedList &operator=(const XORDoubleLinkedList<T> &other) {
    from_xor_dll(other);

    return *this;
  }
  XORDoubleLinkedList(XORDoubleLinkedList<T> &&other) {
    release();

    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();

    other.exec_move();
  }
  XORDoubleLinkedList &operator=(XORDoubleLinkedList<T> &&other) {
    release();

    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();

    other.exec_move();

    return *this;
  }
  inline ~XORDoubleLinkedList() noexcept { release(); }

  inline void release() noexcept {
    if (head == nullptr) {
      return;
    }

    Node *current_node = head;
    Node *prev_node = nullptr;
    Node *next_node = nullptr;

    for (int i = 0; i < length; i++) {
      next_node = XOR(prev_node, current_node->dir);
      prev_node = current_node;

      delete current_node;
      current_node = nullptr;

      current_node = next_node;
    }

    head = nullptr;
    end = nullptr;
    length = 0;
  }

  T &operator[](int index) noexcept {
    bool is_negative = false;
    int calc_index = get_calc_min_index(index, is_negative);

    Node *current_node = nullptr;
    Node *next_node = nullptr;
    Node *prev_node = nullptr;

    if (!is_negative) {
      current_node = head;

      for (int i = 0; i < calc_index; i++) {
        next_node = XOR(prev_node, current_node->dir);
        prev_node = current_node;
        current_node = next_node;
      }
    } else {
      current_node = end;

      for (int i = 1; i < calc_index; i++) {
        prev_node = XOR(next_node, current_node->dir);
        next_node = current_node;
        current_node = prev_node;
      }
    }

    return current_node->value;
  }

  void add(T &value, bool add_back = true) noexcept {
    Node *new_node = new Node();
    new_node->value = value;

    if (add_back) {
      new_node->dir = XOR(end, nullptr);
    } else {
      new_node->dir = XOR(nullptr, head);
    }

    length++;

    if (head == nullptr) {
      head = new_node;
      end = new_node;
      return;
    }

    if (add_back) {
      end->dir = XOR(XOR(end->dir, nullptr), new_node);

      end = new_node;
    } else {
      head->dir = XOR(new_node, XOR(head->dir, nullptr));

      head = new_node;
    }
  }

  inline void add(T &&value, bool add_back = true) noexcept {
    add(value, add_back);
  }

  inline void remove(T &value) {
    Node *current_node = head;
    Node *prev_node = nullptr;
    Node *next_node = nullptr;

    for (int i = 0; i < length; i++) {
      if (current_node->value == value) {
        break;
      }

      next_node = XOR(prev_node, current_node->dir);
      prev_node = current_node;
      current_node = next_node;
    }

    remove_node(current_node, prev_node);
  }

  void remove(T &&value) { remove(value); }

  inline void from_array(T *array, int size) {
    release();

    for (int i = 0; i < size; i++) {
      add(array[i]);
    }
  }

  void to_array(T *array) {
    if (head == nullptr) {
      return;
    }

    Node *current_node = head;
    Node *prev_node = nullptr;
    Node *next_node = nullptr;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;

      next_node = XOR(prev_node, current_node->dir);
      prev_node = current_node;
      current_node = next_node;
    }
  }

  inline Node *get_head_node() const noexcept { return head; }
  inline Node *get_end_node() const noexcept { return end; }
  inline int get_length() const noexcept { return length; }
  constexpr void exec_move() noexcept {
    head = nullptr;
    end = nullptr;
    length = 0;
  }

private:
  inline Node *XOR(Node *prev, Node *next) {
    return (Node *)((uintptr_t)(prev) ^ (uintptr_t)(next));
  }

  inline int get_calc_min_index(int &given_index, bool &is_negative) noexcept {
    int positive_index = given_index >= 0 ? given_index : length + given_index;
    int negative_index =
        given_index < 0 ? given_index * -1 : length - given_index;

    is_negative = positive_index <= negative_index ? false : true;

    return is_negative ? negative_index : positive_index;
  }

  inline void from_xor_dll(const XORDoubleLinkedList<T> &other) {
    release();

    const int other_length = other.get_length();

    Node *current_node = other.get_head_node();
    Node *prev_node = nullptr;
    Node *next_node = nullptr;

    for (int i = 0; i < other_length; i++) {
      add(current_node->value);
      next_node = XOR(prev_node, current_node->dir);
      prev_node = current_node;
      current_node = next_node;
    }
  }

  inline void remove_node(Node *current_node, Node *prev_node) noexcept {
    if (current_node == nullptr) {
      return;
    }

    if (current_node == head) {
      Node *next_node = XOR(nullptr, current_node->dir);
      Node *next_next_node = XOR(current_node, next_node->dir);

      next_node->dir = XOR(nullptr, next_next_node);

      head = next_node;
    } else if (current_node == end) {
      Node *prev_node = XOR(nullptr, current_node->dir);
      Node *prev_prev_node = XOR(current_node, prev_node->dir);

      prev_node->dir = XOR(prev_prev_node, nullptr);

      end = prev_node;
    } else {
      Node *next_node = XOR(prev_node, current_node->dir);
      Node *prev_prev_node = XOR(current_node, prev_node->dir);

      prev_node->dir = XOR(prev_prev_node, next_node);

      Node *next_next_node = XOR(current_node, next_node->dir);
      next_node->dir = XOR(prev_node, next_next_node);
    }

    delete current_node;
    current_node = nullptr;

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
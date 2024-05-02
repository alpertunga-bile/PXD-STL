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

  LinkedList() {
    head = nullptr;
    end = nullptr;
    length = 0;
  } // default constructor
  LinkedList(T *node_list, int size, bool is_reverse = false) {
    from_array(node_list, size, is_reverse);
  }
  LinkedList(Array<T> &node_list, bool is_reverse = false) {
    from_array(node_list.get_ptr(), node_list.get_length(), is_reverse);
  }
  LinkedList(const LinkedList<T> &other) { from_linked_list(other); }
  LinkedList(LinkedList<T> &&other) {
    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();
    other.exec_move();
  }
  LinkedList &operator=(LinkedList<T> &&other) {
    release();

    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();

    other.exec_move();

    return *this;
  }
  LinkedList &operator=(const LinkedList<T> &other) {
    from_linked_list(other);

    return *this;
  }
  ~LinkedList() { release(); }

  T &operator[](int index) {
    const int calc_index = get_calc_index(index);

    Node *current_node = head;

    for (int i = 0; i < calc_index; i++) {
      current_node = current_node->next;
    }

    return current_node->value;
  }

  // actually; head, end and length will be same and inner nodes may be
  // different if the user tries hard enough but whatever
  bool operator==(LinkedList<T> &other) {
    return head == other.get_head_node() && end == other.get_end_node() &&
                   length == other.get_length()
               ? true
               : false;
  }

  bool operator!=(LinkedList<T> &other) {
    return head != other.get_head_node() && end != other.get_end_node() &&
                   length != other.get_length()
               ? true
               : false;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Class Functionalities

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
    length = 0;
  }

  void reverse() {
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
  int where(T &value) {
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
  void add(T &&new_value, bool add_back = true) { add(new_value, add_back); }

  void remove_at(int index) {
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

  void remove(T &&value) { remove(value); }

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
      Node *new_node = new Node();
      new_node->value = node_list[i];

      if (is_reverse) {
        add_to_front(new_node);
      } else {
        add_to_back(new_node);
      }
    }
  }

  void from_array(Array<T> &node_list, bool is_reverse = false) {
    head = nullptr;
    end = nullptr;
    length = 0;

    const int size = node_list.get_length();

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

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Variable Inline Functions

  inline size_t get_byte_size() const { return length * sizeof(T); }
  inline size_t get_mbyte_size() const { return (length * sizeof(T)) / 1024.f; }
  inline size_t get_gbyte_size() const {
    return (length * sizeof(T)) / (1024.f * 1024.f);
  }
  inline size_t get_data_size() const { return sizeof(T); }

  inline int get_length() const { return length; }
  inline bool is_empty() const {
    return head == nullptr && length == 0 ? true : false;
  }
  inline Node *get_head_node() const { return head; }
  inline Node *get_end_node() const { return end; }
  inline void exec_move() {
    head = nullptr;
    end = nullptr;
    length = 0;
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
    }

    length++;
  }

  Node *get_node_at(int index) {
    int calc_index = get_calc_index(index);

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

  void from_linked_list(LinkedList<T> &&other) {
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

  inline int get_calc_index(int index) {
    PXD_ASSERT(index < length);

    int calc_index = index;

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      calc_index = length + index;
    }

    return calc_index;
  }

  void fill_array(T *array) const {
    Node *current_node = head;
    int index = 0;

    do {
      array[index] = current_node->value;
      current_node = current_node->next;
      index++;
    } while (current_node != nullptr);
  }

  void fill_array(Array<T> &array) {
    Node *current_node = head;
    int index = 0;

    do {
      array[index] = current_node->value;
      current_node = current_node->next;
      index++;
    } while (current_node != nullptr);
  }

private:
  Node *head = nullptr;
  Node *end = nullptr;
  int length = 0;
};
} // namespace pxd
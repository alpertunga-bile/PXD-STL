#pragma once

#include "checks.hpp"

#include <cstddef>

namespace pxd {

template<typename T>
class LinkedList
{
public:
  struct Node
  {
    T value;
    Node* next = nullptr;
  };

public:
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors

  constexpr LinkedList() noexcept
  {
    head = nullptr;
    end = nullptr;
    length = 0;
  } // default constructor
  LinkedList(T* node_list, int size, bool is_reverse = false)
  {
    from_array(node_list, size, is_reverse);
  }
  constexpr LinkedList(const LinkedList<T>& other) { from_linked_list(other); }
  constexpr LinkedList(LinkedList<T>&& other) noexcept
  {
    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();
    other.exec_move();
  }
  constexpr LinkedList& operator=(LinkedList<T>&& other)
  {
    release();

    head = other.get_head_node();
    end = other.get_end_node();
    length = other.get_length();

    other.exec_move();

    return *this;
  }
  constexpr LinkedList& operator=(const LinkedList<T>& other)
  {
    from_linked_list(other);

    return *this;
  }
  inline ~LinkedList() noexcept { release(); }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Operator Overloads

  T& operator[](int index) noexcept
  {
    const int calc_index = get_calc_index(index);

    Node* current_node = head;

    for (int i = 0; i < calc_index; i++) {
      current_node = current_node->next;
    }

    return current_node->value;
  }

  // actually; head, end and length will be same and inner nodes may be
  // different if the user tries hard enough but whatever
  constexpr bool operator==(LinkedList<T>& other) noexcept
  {
    return head == other.get_head_node() && end == other.get_end_node() &&
               length == other.get_length()
             ? true
             : false;
  }

  // actually; head, end and length will be same and inner nodes may be
  // different if the user tries hard enough but whatever
  constexpr bool operator!=(LinkedList<T>& other) noexcept
  {
    return head != other.get_head_node() && end != other.get_end_node() &&
               length != other.get_length()
             ? true
             : false;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // DS Functionalities

  /// @brief delete all the nodes in the linked list
  inline void release() noexcept
  {
    if (head == nullptr) {
      return;
    }

    Node* current_node = head;

    do {
      Node* prev_node = current_node;
      current_node = current_node->next;
      delete prev_node;
    } while (current_node != nullptr);

    head = nullptr;
    end = nullptr;
    length = 0;
  }

  /// @brief reverse the linked list's direction
  void reverse() noexcept
  {
    if (is_empty() || length == 1) {
      return;
    }

    Node* current_node = head->next;

    Node* prev_node = head;
    prev_node->next = nullptr;
    end = prev_node;

    Node* forw_node = nullptr;

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
  constexpr int where(T& value) noexcept
  {
    Node* current_node = head;

    for (int i = 0; i < length; i++) {
      if (current_node->value == value) {
        return i;
      }

      current_node = current_node->next;
    }

    return length;
  }

  /// @brief add a value to the linked list
  /// @param new_value value to be added
  /// @param add_back true add the value to the end, false add the value to the
  /// head
  inline void add(T& new_value, bool add_back = true) noexcept
  {
    Node* new_node = new Node();
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

  /// @brief add a value to the linked list
  /// @param new_value value to be added
  /// @param add_back true add the value to the end, false add the value to the
  /// head
  inline void add(T&& new_value, bool add_back = true)
  {
    add(new_value, add_back);
  }

  /// @brief remove a value from the linked list with an index
  /// @param index the index of the value
  void remove_at(int index)
  {
    if (head == nullptr) {
      return;
    }

    int calc_index = get_calc_index(index);

    Node* prev_node = nullptr;
    Node* current_node = head;

    for (int i = 0; i < calc_index; i++) {
      prev_node = current_node;
      current_node = current_node->next;
    }

    remove_node(prev_node, current_node);
  }

  /// @brief remove a value from the linked list
  /// @param value the value wants to be removed
  void remove(T& value) noexcept
  {
    if (head == nullptr) {
      return;
    }

    Node* prev_node = nullptr;
    Node* current_node = head;

    for (int i = 0; i < length; i++) {
      if (current_node->value == value) {
        break;
      }

      prev_node = current_node;
      current_node = current_node->next;
    }

    remove_node(prev_node, current_node);
  }

  /// @brief remove a value from the linked list
  /// @param value the value wants to be removed
  void remove(T&& value) noexcept { remove(value); }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // From Functions

  inline void from_array(T* node_list, int size, bool is_reverse = false)
  {
    release();

    for (int i = 0; i < size; i++) {
      add(node_list[i], !is_reverse);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // To Functions

  void to_array(T* array)
  {
    if (is_empty()) {
      return;
    }

    fill_array(array);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Inline Member Funcs

  constexpr inline std::size_t get_byte_size() const noexcept
  {
    return length * sizeof(T);
  }
  constexpr inline std::size_t get_data_size() const noexcept
  {
    return sizeof(T);
  }
  constexpr inline int get_length() const noexcept { return length; }
  constexpr inline bool is_empty() const noexcept
  {
    return head == nullptr && length == 0 ? true : false;
  }
  constexpr inline Node* get_head_node() const noexcept { return head; }
  constexpr inline Node* get_end_node() const noexcept { return end; }

  /// @brief the function has to be executed in the move constructors. releasing
  /// class without deleting
  constexpr inline void exec_move() noexcept
  {
    head = nullptr;
    end = nullptr;
    length = 0;
  }

private:
  constexpr void from_linked_list(const LinkedList<T>& other)
  {
    release();

    Node* this_current_node = new Node();
    Node* other_current_node = other.get_head_node();

    this_current_node->value = other_current_node->value;
    head = this_current_node;
    other_current_node = other_current_node->next;

    length = other.get_length();

    for (int i = 1; i < length; i++) {
      Node* new_node = new Node();
      new_node->value = other_current_node->value;

      this_current_node->next = new_node;

      this_current_node = new_node;
      other_current_node = other_current_node->next;
    }

    end = this_current_node;
  }

  /// @brief calculate index for negative and positive indices
  /// @param index the given index
  /// @return the calculated valid index
  constexpr inline int get_calc_index(int index)
  {
    PXD_ASSERT(index < length);

    int calc_index = index;

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      calc_index = length + index;
    }

    return calc_index;
  }

  inline void fill_array(T* array) const noexcept
  {
    Node* current_node = head;

    for (int i = 0; i < length; i++) {
      array[i] = current_node->value;
      current_node = current_node->next;
    }
  }

  inline void remove_node(Node* prev_node, Node* current_node) noexcept
  {
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
    current_node = nullptr;

    if (head == nullptr) {
      end = nullptr;
    }

    length--;
  }

private:
  Node* head = nullptr;
  Node* end = nullptr;
  int length = 0;
};
} // namespace pxd
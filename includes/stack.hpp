#pragma once

#include "linked_list.hpp"

namespace pxd {
template <typename T> class Stack {
public:
  Stack() : stack(){};
  Stack(T *array, int size) : stack(array, size, true){};
  Stack(const Stack<T> &other) : stack(other.get_stack()){};
  Stack(Stack<T> &&other) { stack = std::move(other.get_stack()); };
  Stack &operator=(Stack<T> &&other) {
    stack = std::move(other.get_stack());
    return *this;
  }
  Stack &operator=(const Stack<T> &other) {
    LinkedList<T> temp_ll(other);
    stack = temp_ll;
    return temp_ll;
  };
  ~Stack() { stack.release(); };

  inline void push(T &value) { stack.add(value, false); }
  inline void push(T &&value) { stack.add(value, false); }

  inline T pop() {
    PXD_ASSERT(!is_empty());

    T top_value = stack[0];
    stack.remove_at(0);

    return top_value;
  }

  inline T peek() {
    PXD_ASSERT(!is_empty());

    return stack[0];
  }

  inline void release() { stack.release(); }

  inline void to_array(T *array) { stack.to_array(array); }

  inline void reverse() { stack.reverse(); }

  inline bool is_empty() const { return stack.is_empty(); }
  inline LinkedList<T> get_stack() const { return stack; }
  inline int get_length() const { return stack.get_length(); }

private:
  LinkedList<T> stack;
};
} // namespace pxd
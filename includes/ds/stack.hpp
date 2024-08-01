#pragma once

#include "linked_list.hpp"

namespace pxd {
template <typename T> class Stack {
public:
  Stack() : stack() {};
  Stack(T *array, int size) : stack(array, size, true) {};
  Stack(const Stack<T> &other) : stack(other.get_stack()) {};
  Stack(Stack<T> &&other) noexcept { stack = std::move(other.get_stack()); };
  auto operator=(Stack<T> &&other) noexcept -> Stack & {
    stack = std::move(other.get_stack());
    return *this;
  }
  auto operator=(const Stack<T> &other) -> Stack & {
    if (other.get_stack().get_head_node() == stack.get_head_node()) {
      return *this;
    }

    LinkedList<T> temp_ll(other);
    stack = temp_ll;
    return temp_ll;
  };
  ~Stack() { stack.release(); };

  void push(const T &value) { stack.add(value, false); }
  void push(T &&value) { stack.add(value, false); }

  auto pop() -> T {
    PXD_ASSERT(!is_empty());

    T top_value = stack[0];
    stack.remove_at(0);

    return top_value;
  }

  auto peek() -> T {
    PXD_ASSERT(!is_empty());

    return stack[0];
  }

  void release() { stack.release(); }

  void to_array(T *array) { stack.to_array(array); }

  void reverse() { stack.reverse(); }

  auto is_empty() const -> bool { return stack.is_empty(); }
  auto get_stack() const -> LinkedList<T> { return stack; }
  auto get_length() const -> int { return stack.get_length(); }

private:
  LinkedList<T> stack;
};
} // namespace pxd
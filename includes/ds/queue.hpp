#pragma once

#include "linked_list.hpp"

namespace pxd {
template <typename T> class Queue {
public:
  Queue() : queue() {};
  Queue(T *array, int size) : queue(array, size) {};
  Queue(const Queue<T> &other) : queue(other.get_queue()) {}
  Queue(Queue<T> &&other) noexcept { queue = std::move(other.get_queue()); }
  auto operator=(Queue<T> &&other) noexcept -> Queue & {
    queue = std::move(other.get_queue());
    return *this;
  }
  auto operator=(const Queue<T> &other) -> Queue & {
    if (other.get_queue().get_head_node() == queue.get_head_node()) {
      return *this;
    }

    LinkedList<T> q(other.get_queue);
    queue = q;
    return q;
  }
  ~Queue() { queue.release(); }

  void push(const T &value) { queue.add(value); }
  void push(T &&value) { queue.add(value); }

  auto pop() -> T {
    PXD_ASSERT(!is_empty());

    T top_value = queue[0];
    queue.remove_at(0);

    return top_value;
  }

  auto peek() -> T {
    PXD_ASSERT(!is_empty());

    return queue[0];
  }

  void to_array(int *array) { queue.to_array(array); }

  void release() { queue.release(); }

  void reverse() { queue.reverse(); }

  auto is_empty() const -> bool { return queue.is_empty(); }
  auto get_queue() const -> LinkedList<T> { return queue; }
  auto get_length() const -> int { return queue.get_length(); }

private:
  LinkedList<T> queue;
};
} // namespace pxd
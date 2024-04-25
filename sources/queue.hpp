#pragma once

#include "linked_list.hpp"

namespace pxd {
template <typename T> class Queue {
public:
  Queue() : queue(){};
  Queue(const Queue<T> &other) : queue(other.get_queue()) {}
  Queue(Queue<T> &&other) : queue(other.get_queue()) {}
  Queue &operator=(const Queue<T> &other) {
    LinkedList<T> q(other.get_queue);
    queue = q;
    return q;
  }
  ~Queue() { queue.release(); }

  inline void push(T &value) { queue.add(value); }
  inline void push(T &&value) { queue.add(value); }

  inline T pop() {
    PXD_ASSERT(!is_empty());

    T top_value = queue[0];
    queue.remove_at(0);

    return top_value;
  }

  inline T peek() {
    PXD_ASSERT(!is_empty());

    return queue[0];
  }

  inline void reverse() { queue.reverse(); }

  inline bool is_empty() const { return queue.is_empty(); }
  inline LinkedList<T> get_queue() const { return queue; }
  inline int get_length() const { return queue.get_length(); }

private:
  LinkedList<T> queue;
};
} // namespace pxd
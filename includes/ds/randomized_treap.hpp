#pragma once

#include "../checks.hpp"
#include "treap.hpp"

#include "../random_gen.hpp"

namespace pxd {
/// @brief Randomized treap to use as (most possibly) balanced BST
/// @tparam T value type
template <typename T> class RandomizedTreap {
public:
  RandomizedTreap() = default;
  RandomizedTreap(const RandomizedTreap<T> &other) = default;
  RandomizedTreap &operator=(const RandomizedTreap<T> &other) = default;
  RandomizedTreap(RandomizedTreap &&other) = default;
  RandomizedTreap &operator=(RandomizedTreap &&other) = default;
  ~RandomizedTreap() = default;

  void insert(T &key) { treap.insert(key, get_random_value(0.0, 1.0)); }

  void constains(const T &key) { return treap.contains(key); }

  void remove(const T &key) { return treap.remove(key); }

  T min() { return treap.min(); }
  T max() { return treap.max(); }

private:
  Treap<T> treap;
};
} // namespace pxd
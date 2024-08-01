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
  auto
  operator=(const RandomizedTreap<T> &other) -> RandomizedTreap & = default;
  RandomizedTreap(RandomizedTreap &&other) = default;
  auto operator=(RandomizedTreap &&other) -> RandomizedTreap & = default;
  ~RandomizedTreap() = default;

  void insert(const T &key) {
    treap.insert(key, pxd::random::random_value<double>(0.0, 1.0));
  }

  void constains(const T &key) { return treap.contains(key); }

  void remove(const T &key) { return treap.remove(key); }

  auto min() -> T { return treap.min(); }
  auto max() -> T { return treap.max(); }

private:
  Treap<T> treap;
};
} // namespace pxd
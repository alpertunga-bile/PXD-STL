#pragma once

#include "checks.hpp"
#include "treap.hpp"

#include "random_gen.hpp"

namespace pxd {
template <typename T> class RandomizedTreap {

private:
  Treap<T> treap;
};
} // namespace pxd
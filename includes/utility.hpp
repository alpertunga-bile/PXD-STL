#pragma once

namespace pxd {
template <typename T> void swap(T &left, T &right) {
  T temp = left;
  left = right;
  right = temp;
}
} // namespace pxd
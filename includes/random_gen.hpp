#pragma once

#include "absl/random.hpp"
#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace pxd {

namespace random {

// do not use this !!!
// use the functions instead
static absl::BitGen bitgen;

enum class ClosureTypes : uint8_t {
  CLOSED,
  OPEN,
  OPEN_CLOSED,
  OPEN_OPEN,
  CLOSED_OPEN,
  CLOSED_CLOSED
};

template <typename T>
auto random_value(T min, T max,
                  ClosureTypes closure_type = ClosureTypes::CLOSED_OPEN) -> T {
  if (!std::numeric_limits<T>::is_integer &&
      !std::is_floating_point<T>::value) {
    return {};
  }

  switch (closure_type) {
  case ClosureTypes::CLOSED:
    return absl::Uniform<T>(absl::IntervalClosed, bitgen, min, max);
    break;
  case ClosureTypes::OPEN:
    return absl::Uniform<T>(absl::IntervalOpen, bitgen, min, max);
    break;
  case ClosureTypes::OPEN_CLOSED:
    return absl::Uniform<T>(absl::IntervalOpenClosed, bitgen, min, max);
    break;
  case ClosureTypes::OPEN_OPEN:
    return absl::Uniform<T>(absl::IntervalOpenOpen, bitgen, min, max);
    break;
  case ClosureTypes::CLOSED_OPEN:
    return absl::Uniform<T>(absl::IntervalClosedOpen, bitgen, min, max);
    break;
  case ClosureTypes::CLOSED_CLOSED:
    return absl::Uniform<T>(absl::IntervalClosedClosed, bitgen, min, max);
    break;
  default:
    return {};
    break;
  }
}

/*
 * ------------------------------------------------------------------------------
 * -- Shuffle Array
 */

template <typename T> inline void shuffle(T *ptr, size_t size) {
  if (nullptr == ptr) {
    return;
  }

  const int loop_time = size - 1;

  for (int i = 0; i < loop_time; ++i) {
    int j = random_value<int>(i, loop_time, ClosureTypes::CLOSED_CLOSED);

    auto temp = ptr[i];
    ptr[i] = ptr[j];
    ptr[j] = temp;
  }
}

template <typename T, size_t N> inline void shuffle(std::array<T, N> &array) {
  const int loop_time = N - 1;

  for (int i = 0; i < loop_time; ++i) {
    int j = random_value<int>(i, loop_time, ClosureTypes::CLOSED_CLOSED);

    auto temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

template <typename T> inline void shuffle(std::vector<T> &vec) {
  const int loop_time = vec.size() - 1;

  for (int i = 0; i < loop_time; ++i) {
    int j = random_value<int>(i, loop_time, ClosureTypes::CLOSED_CLOSED);

    auto temp = vec[i];
    vec[i] = vec[j];
    vec[j] = temp;
  }
}

} // namespace random

} // namespace pxd
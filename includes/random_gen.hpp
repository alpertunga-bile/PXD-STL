#pragma once

#include "absl/random.hpp"
#include <cstdint>
#include <limits>
#include <type_traits>

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

} // namespace random

} // namespace pxd
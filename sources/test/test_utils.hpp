#pragma once

#include "checks.hpp"
#include "i_test.hpp"

namespace pxd {
template <typename T>
inline bool check_arrays(T *first_array, T *second_array, int size) {
  for (int i = 0; i < size; i++) {
    if (first_array[i] != second_array[i]) {
      std::cout << "first_array : " << first_array[i]
                << " second_array : " << second_array[i] << "\n";

      return false;
    }
  }

  return true;
}

template <typename T>
inline bool check_arrays(T *first_array, int first_start, T *second_array,
                         int second_start, int size) {
  int second_index = second_start;
  int end_index = first_start + size;

  for (int first_index = first_start; first_index < end_index; first_index++) {
    if (first_array[first_index] != second_array[second_index]) {
      return false;
    }

    second_index++;
  }

  return true;
}

template <typename T>
inline bool check_reverse_arrays(T *normal_array, T *reverse_array, int size) {
  int reverse_index = size - 1;

  for (int i = 0; i < size; i++) {
    if (normal_array[i] != reverse_array[reverse_index]) {
      return false;
    }

    reverse_index--;
  }

  return true;
}
} // namespace pxd
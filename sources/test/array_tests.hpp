#pragma once

#include "../array.hpp"
#include "checks.hpp"
#include "i_test.hpp"

namespace pxd {
class ArrayTests : public ITest
{
public:
  void start_test() override { start_init_with_array_test(); }

private:
  template<typename T>
  bool check_arrays(T* first_array, T* second_array, int size)
  {
    for (int i = 0; i < size; i++) {
      if (first_array[i] != second_array[i]) {
        return false;
      }
    }

    return true;
  }

  template<typename T>
  bool check_arrays(T* first_array,
                    int first_start,
                    T* second_array,
                    int second_start,
                    int size)
  {
    int second_index = second_start;
    int end_index = first_start + size;

    for (int first_index = first_start; first_index < end_index;
         first_index++) {
      if (first_array[first_index] != second_array[second_index]) {
        return false;
      }

      second_index++;
    }

    return true;
  }

  void start_init_with_array_test()
  {
    int temp[N];
    for (int i = 0; i < N; i++) {
      temp[i] = i;
    }

    Array<int> arr(temp, N);

    test_results["init with array test"] =
      check_arrays<int>(temp, arr.get_ptr(), N);
  }

private:
  int N = 10;
};
}
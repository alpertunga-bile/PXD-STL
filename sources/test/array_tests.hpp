#pragma once

#include "../array.hpp"
#include "checks.hpp"
#include "i_test.hpp"

/*
void
print_array(const char* name, pxd::Array<int>& arr)
{
  printf("---------------------------------------------------------------------"
         "-------\n");
  printf("%s array :\n", name);
  int i = 0;
  int size = arr.get_length();

  for (i = 0; i < size; i++) {
    printf("%6d -> %6d\n", i + 1, arr[i]);
  }
  printf("---------------------------------------------------------------------"
         "-------\n");
}
*/

namespace pxd {
class ArrayTests : public ITest {
public:
  void start_test() override {
    int *temp = new int[N];

    for (int i = 0; i < N; i++) {
      temp[i] = i;
    }

    start_init_with_array_test<int>(temp);
    start_copy_to_test<int>(temp);
    start_bool_compare_test<int>();
    start_expand_test<int>(temp);
    start_index_test<int>(temp);
    start_resize_test<int>(temp);

    delete[] temp;
  }

private:
  template <typename T>
  bool check_arrays(T *first_array, T *second_array, int size) {
    for (int i = 0; i < size; i++) {
      if (first_array[i] != second_array[i]) {
        return false;
      }
    }

    return true;
  }

  template <typename T>
  bool check_arrays(T *first_array, int first_start, T *second_array,
                    int second_start, int size) {
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

  template <typename T> void start_init_with_array_test(T *temp) {
    Array<T> arr(temp, N);

    test_results["init with array test"] =
        check_arrays<T>(temp, arr.get_ptr(), N);
  }

  template <typename T> void start_copy_to_test(T *temp) {
    Array<T> arr(temp, N);

    Array<T> t(N);
    arr.copy_to(t);

    test_results["copy_to test"] =
        check_arrays<T>(t.get_ptr(), arr.get_ptr(), N);
  }

  template <typename T> void start_bool_compare_test() {
    Array<T> arr(10);
    Array<T> temp(10);

    test_results["bool compare test"] = arr != temp && arr == arr;
  }

  template <typename T> void start_expand_test(T *temp) {
    Array<T> arr(temp, N);
    Array<T> t(10);

    arr.copy_to(t);

    t.expand(arr);

    test_results["expand test"] =
        check_arrays(t.get_ptr(), arr.get_ptr(), N) &&
        check_arrays(t.get_ptr(), N, arr.get_ptr(), 0, N);
  }

  template <typename T> void start_index_test(T *temp) {
    Array<T> arr(temp, N);

    test_results["index test"] = arr[-1] == arr[N - 1] && arr[2] == 2;
  }

  template <typename T> void start_resize_test(T *temp) {
    int new_size = 5;
    Array<T> arr(temp, N);

    arr.resize(new_size);

    test_results["resize test"] =
        check_arrays<T>(temp, arr.get_ptr(), new_size);
  }

private:
  int N = 10;
};
} // namespace pxd
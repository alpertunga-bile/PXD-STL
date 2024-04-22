#pragma once

#include "array.hpp"
#include "test_utils.hpp"
namespace pxd {
class ArrayTests : public ITest
{
public:
  void start_test() override
  {
    int* temp = new int[N];

    for (int i = 0; i < N; i++) {
      temp[i] = i;
    }

    start_init_with_array_test<int>(temp);
    start_copy_to_test<int>(temp);
    start_bool_compare_test<int>();
    start_expand_test<int>(temp);
    start_index_test<int>(temp);
    start_resize_test<int>(temp);
    start_copy_ctor_test<int>(temp);
    start_move_ctor_test<int>(temp);
    start_assign_ctor_test<int>(temp);

    delete[] temp;
  }

private:
  template<typename T>
  void start_init_with_array_test(T* temp)
  {
    Array<T> arr(temp, N);

    test_results["init with array"] = check_arrays<T>(temp, arr.get_ptr(), N);
  }

  template<typename T>
  void start_copy_to_test(T* temp)
  {
    Array<T> arr(temp, N);

    Array<T> t(N);
    arr.copy_to(t);

    test_results["copy_to"] = check_arrays<T>(t.get_ptr(), arr.get_ptr(), N);
  }

  template<typename T>
  void start_bool_compare_test()
  {
    Array<T> arr(10);
    Array<T> temp(10);

    test_results["bool compare"] = arr != temp && arr == arr;
  }

  template<typename T>
  void start_expand_test(T* temp)
  {
    Array<T> arr(temp, N);
    Array<T> t(10);

    arr.copy_to(t);

    t.expand(arr);

    test_results["expand"] = check_arrays(t.get_ptr(), arr.get_ptr(), N) &&
                             check_arrays(t.get_ptr(), N, arr.get_ptr(), 0, N);
  }

  template<typename T>
  void start_index_test(T* temp)
  {
    Array<T> arr(temp, N);

    test_results["negative index"] = arr[-1] == arr[N - 1];
    test_results["index"] = arr[2] == 2;
  }

  template<typename T>
  void start_resize_test(T* temp)
  {
    int new_size = 5;
    Array<T> arr(temp, N);

    arr.resize(new_size);

    test_results["resize test"] =
      check_arrays<T>(temp, arr.get_ptr(), new_size);
  }

  template<typename T>
  void start_copy_ctor_test(T* temp)
  {
    Array<T> arr(temp, N);

    Array<T> t(arr);

    test_results["copy constructor"] =
      check_arrays(arr.get_ptr(), t.get_ptr(), N);
  }

  template<typename T>
  void start_move_ctor_test(T* temp)
  {
    Array<T> arr(temp, N);

    Array<T> t(std::move(arr));

    test_results["move constructor"] =
      check_arrays(arr.get_ptr(), t.get_ptr(), N);
  }

  template<typename T>
  void start_assign_ctor_test(T* temp)
  {
    Array<T> arr(temp, N);

    Array<T> t = arr;

    test_results["assign constructor"] =
      check_arrays(arr.get_ptr(), t.get_ptr(), N);
  }

private:
  int N = 10;
};
} // namespace pxd
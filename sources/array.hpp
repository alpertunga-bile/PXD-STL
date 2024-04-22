#pragma once

#include "checks.hpp"

namespace pxd {
template <typename T> class Array {
public:
  Array() = delete; // default constructor
  Array(int size) { allocate(size); }
  Array(T *given_array, int size) {
    allocate(size);
    copy(given_array, arr_ptr, 0, size);
  }

  Array(const Array<T> &other) = delete;            // copy constructor
  Array(Array<T> &&other) = delete;                 // move constructor
  Array &operator=(const Array<T> &other) = delete; // copy assignment
  ~Array()                                          // deconstructor
  {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;
  }

  bool operator==(Array<T> &other) { return other.arr_ptr == arr_ptr; }
  bool operator!=(Array<T> &other) { return other.arr_ptr != arr_ptr; }

  decltype(auto) operator[](int index) {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT((index * -1) <= length);
      return arr_ptr[length - (index * -1)];
    }

    return arr_ptr[index];
  }

  void resize(int new_size) {
    T *temp_array = new T[length];
    copy(arr_ptr, temp_array, 0, length);

    release();
    allocate(temp_array, new_size);

    delete[] temp_array;
  }

  void reallocate(T *given_array, int size) {
    release();
    allocate(given_array, size);
  }

  void release() {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;

    arr_ptr = nullptr;
    length = 0;
    byte_size = 0;
  }

  void copy_to(Array<T> &to) {
    PXD_ASSERT(to.get_length() == length);

    for (int i = 0; i < length; i++) {
      to[i] = arr_ptr[i];
    }
  }

  void expand(Array<T> &given_array) {
    if (given_array.get_length() == 0) {
      return;
    }

    int old_length = length;
    int new_size = length + given_array.get_length();

    resize(new_size);

    for (int i = old_length; i < new_size; i++) {
      arr_ptr[i] = given_array[i - old_length];
    }
  }

  void expand(T *given_array, int size) {
    int old_length = length;
    int new_size = length + size;

    resize(new_size);

    for (int i = old_length; i < new_size; i++) {
      arr_ptr[i] = given_array[i - old_length];
    }
  }

  inline T *get_ptr() { return arr_ptr; }
  inline int get_length() { return length; }
  inline size_t get_byte_size() { return byte_size; }
  inline float get_mbyte_size() { return ((float)byte_size) / 1024.f; }
  inline float get_gbyte_size() {
    return ((float)byte_size) / (1024.f * 1024.f);
  }
  inline size_t get_data_size() { return sizeof(T); }

private:
  void allocate(int size) {
    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);
  }

  void allocate(T *given_array, int size) {
    PXD_ASSERT(given_array != nullptr);

    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);
    copy(given_array, arr_ptr, 0, size);
  }

  void copy(T *from, T *to, int start, int end) {
    PXD_ASSERT(from != nullptr);
    PXD_ASSERT(to != nullptr);
    PXD_ASSERT(end > start);

    for (int i = start; i < end; i++) {
      to[i] = from[i];
    }
  }

private:
  T *arr_ptr = nullptr;
  int length = 0;
  size_t byte_size = 0;
};
} // namespace pxd
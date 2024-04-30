#pragma once

#include "checks.hpp"

namespace pxd {

template <typename T> class LinkedList;

template <typename T> class Array {
public:
  Array() {
    arr_ptr = nullptr;
    length = 0;
    byte_size = 0;
  } // default constructor
  Array(int size) { allocate(size); }
  Array(T *given_array, int size) { allocate(given_array, size); }

  Array(const Array<T> &other) { from(other); } // copy constructor

  Array(Array<T> &&other) { from(other); } // move constructor

  Array &operator=(const Array<T> &other) {
    from(other);

    *this;
  } // copy assignment

  ~Array() // deconstructor
  {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;
  }

  bool operator==(Array<T> &other) { return compare(other); }
  bool operator!=(Array<T> &other) { return compare(other); }

  decltype(auto) operator[](int index) {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      return arr_ptr[length + index];
    }

    return arr_ptr[index];
  }

  decltype(auto) operator[](int index) const {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      return arr_ptr[length + index];
    }

    return arr_ptr[index];
  }

  inline int where(T &value) {
    int index = 0;
    return find(value, 0, length, index);
  }
  inline int where(T &&value) { return where(value); }

  void resize(int new_size) {
    if (length == 0) {
      allocate(new_size);
      return;
    }

    T *temp_array = new T[length];
    copy_full(arr_ptr, temp_array);

    reallocate(temp_array, new_size);

    delete[] temp_array;
  }

  void reallocate(T *given_array, int size) {
    release();
    allocate(given_array, size);
  }

  void reallocate(int size) {
    release();
    allocate(size);
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
    if (to.get_length() != length) {
      to.resize(length);
    }

    copy_full(arr_ptr, to.get_ptr());
  }

  void expand(Array<T> &given_array) {
    if (given_array.get_length() == 0) {
      return;
    }

    if (length == 0) {
      allocate(given_array.get_ptr(), given_array.get_length());
      return;
    }

    int old_length = length;
    int new_size = length + given_array.get_length();

    resize(new_size);

    size_t copy_size = given_array.get_length() * sizeof(T);
    copy(given_array.get_ptr(), arr_ptr + old_length, copy_size);
  }

  void expand(T *given_array, int size) {
    if (length == 0) {
      allocate(given_array, size);
      return;
    }

    int old_length = length;
    int new_size = length + size;

    resize(new_size);

    copy(given_array, arr_ptr + old_length, size * sizeof(T));
  }

  void to_linked_list(LinkedList<T> &linked_list) {
    linked_list.release();
    linked_list.from_array(arr_ptr, length);
  }

  LinkedList<T> to_linked_list() {
    LinkedList<T> ll(arr_ptr, length);
    return ll;
  }

  inline T *get_ptr() { return arr_ptr; }
  inline T *get_ptr() const { return arr_ptr; }
  inline int get_length() const { return length; }
  inline size_t get_byte_size() const { return byte_size; }
  inline float get_mbyte_size() const { return ((float)byte_size) / 1024.f; }
  inline float get_gbyte_size() const {
    return ((float)byte_size) / (1024.f * 1024.f);
  }
  inline size_t get_data_size() const { return sizeof(T); }

private:
  void allocate(int size) {
    PXD_ASSERT(arr_ptr == nullptr);

    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);
  }

  void allocate(T *given_array, int size) {
    PXD_ASSERT(arr_ptr == nullptr);
    PXD_ASSERT(given_array != nullptr);

    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);

    copy_full(given_array, arr_ptr);
  }

  void copy(T *from, T *to, size_t size) {
    PXD_ASSERT(from != nullptr);
    PXD_ASSERT(to != nullptr);

    memcpy(to, from, size);
  }

  void copy_full(T *from, T *to) { copy(from, to, byte_size); }

  void from(const Array<T> &given_array) {
    if (length != given_array.get_length()) {
      reallocate(given_array.get_length());
    }

    copy_full(arr_ptr, given_array.get_ptr());
  }

  void from(Array<T> &&given_array) { from(given_array); }

  bool compare(Array<T> &other) {
    if (length != other.get_length() || byte_size != other.get_byte_size() ||
        !IS_VALID(other.get_ptr())) {
      return false;
    }

    bool is_same = true;
    compare_arrays(arr_ptr, other.get_ptr(), 0, length, is_same);

    return is_same;
  }

  void compare_arrays(T *first_array, T *second_array, int start, int end,
                      bool &result) {
    if (start > end) {
      return;
    }

    int mid = (start + end) / 2;
    result = result && (first_array[mid] == second_array[mid] ? true : false);

    compare_arrays(first_array, second_array, start, mid - 1, result);
    compare_arrays(first_array, second_array, mid + 1, end, result);
  }

  void find(T &value, int start, int end, int &index) {
    if (start > end) {
      return;
    }

    int mid = (start + end) / 2;
    if (arr_ptr[mid] == value) {
      index = mid;
    }

    find(value, start, mid - 1, index);
    find(value, mid + 1, end, index);
  }

private:
  T *arr_ptr = nullptr;
  int length = 0;
  size_t byte_size = 0;
};
} // namespace pxd
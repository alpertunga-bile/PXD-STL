#pragma once

#include "checks.hpp"

namespace pxd {
template <typename T> class Array {
public:
  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors

  constexpr Array() {
    arr_ptr = nullptr;
    length = 0;
    byte_size = 0;
  } // default constructor
  constexpr Array(int size) { allocate(size); }
  Array(T *given_array, int size) { allocate(given_array, size); }
  constexpr Array(const Array<T> &other) { from(other); } // copy constructor
  constexpr Array(Array<T> &&other) noexcept {
    arr_ptr = other.get_ptr();
    length = other.get_length();
    byte_size = other.get_byte_size();

    other.exec_move();
  } // move constructor
  constexpr Array &operator=(Array<T> &&other) noexcept {
    release();

    arr_ptr = other.get_ptr();
    length = other.get_length();
    byte_size = other.get_byte_size();

    other.exec_move();

    return *this;
  }
  constexpr Array &operator=(const Array<T> &other) {
    from(other);

    return *this;
  }                        // copy assignment
  inline ~Array() noexcept // deconstructor
  {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Operator Overloads

  constexpr bool operator==(Array<T> &other) noexcept { return compare(other); }
  constexpr bool operator!=(Array<T> &other) noexcept { return compare(other); }

  constexpr decltype(auto) operator[](int index) {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      return arr_ptr[length + index];
    }

    return arr_ptr[index];
  }

  constexpr decltype(auto) operator[](int index) const {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      return arr_ptr[length + index];
    }

    return arr_ptr[index];
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // DS Functionalities

  constexpr inline int where(T &value) noexcept {
    int index = 0;
    return find(value, 0, length, index);
  }

  constexpr inline int where(T &&value) noexcept { return where(value); }

  constexpr void resize(int new_size) {
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

  constexpr void reallocate(int size) {
    release();
    allocate(size);
  }

  inline void release() noexcept {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;

    arr_ptr = nullptr;
    length = 0;
    byte_size = 0;
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

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // To Functions

  void copy_to(Array<T> &to) {
    if (to.get_length() != length) {
      to.resize(length);
    }

    copy_full(arr_ptr, to.get_ptr());
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Inline Member Funcs

  inline T *get_ptr() noexcept { return arr_ptr; }
  inline T *get_ptr() const noexcept { return arr_ptr; }
  inline int get_length() const noexcept { return length; }
  inline size_t get_byte_size() const noexcept { return byte_size; }
  inline float get_mbyte_size() const noexcept {
    return ((float)byte_size) / 1024.f;
  }
  inline float get_gbyte_size() const noexcept {
    return ((float)byte_size) / (1024.f * 1024.f);
  }
  inline size_t get_data_size() const noexcept { return sizeof(T); }
  constexpr void exec_move() noexcept {
    arr_ptr = nullptr;
    length = 0;
    byte_size = 0;
  }

private:
  constexpr void allocate(int size) {
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

  void from(const Array<T> &given_array) noexcept {
    if (length != given_array.get_length()) {
      reallocate(given_array.get_length());
    }

    copy_full(given_array.get_ptr(), arr_ptr);
  }

  bool compare(Array<T> &other) noexcept {
    if (length != other.get_length() || byte_size != other.get_byte_size() ||
        !IS_VALID(other.get_ptr())) {
      return false;
    }

    bool is_same = true;
    compare_arrays(arr_ptr, other.get_ptr(), 0, length - 1, is_same);

    return is_same;
  }

  void compare_arrays(T *first_array, T *second_array, int start, int end,
                      bool &result) noexcept {
    if (start > end) {
      return;
    }

    int mid = (start + end) / 2;
    result = result && (first_array[mid] == second_array[mid] ? true : false);

    compare_arrays(first_array, second_array, start, mid - 1, result);
    compare_arrays(first_array, second_array, mid + 1, end, result);
  }

  void find(T &value, int start, int end, int &index) noexcept {
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
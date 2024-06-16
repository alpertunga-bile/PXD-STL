#pragma once

#include "checks.hpp"
#include "utility.hpp"
#include <cstring>

namespace pxd {
template <typename T> class Array {
public:
  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors

  Array() {
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
  } // copy assignment
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

  decltype(auto) operator[](int index) {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      return arr_ptr[length + index];
    }

    return arr_ptr[index];
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // DS Functionalities

  /// @brief get index of the value
  /// @param value wanted value to be found
  /// @return index value if successful, if not INDEX_NONE
  constexpr inline int where(T &value) noexcept { return find(*this, value); }

  /// @brief get index of the value
  /// @param value wanted value to be found
  /// @return index value if successful, if not INDEX_NONE
  constexpr inline int where(T &&value) noexcept { return where(value); }

  /// @brief resize array with contained values
  /// @param new_size wanted new size
  constexpr void resize(int new_size) {
    if (length == 0) {
      allocate(new_size);
      return;
    }

    if (new_size == 0) {
      release();
      return;
    }

    T *temp_array = new T[length];
    copy_full(arr_ptr, temp_array);

    reallocate(temp_array, new_size);

    delete[] temp_array;
  }

  /// @brief recreate the array from given raw array values
  /// @param given_array array that contains values
  /// @param size total value count of the given array
  void reallocate(T *given_array, int size) {
    release();
    allocate(given_array, size);
  }

  /// @brief recreate the array with the given size
  /// @param size wanted new size
  constexpr void reallocate(int size) {
    release();
    allocate(size);
  }

  /// @brief release the array
  inline void release() noexcept {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;

    arr_ptr = nullptr;
    length = 0;
    byte_size = 0;
  }

  /// @brief expand the array with given array's values
  /// @param given_array the array that contains values
  void expand(Array<T> &given_array) {
    if (given_array.get_length() == 0) {
      return;
    }

    // if the array is empty
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

  /// @brief expand the array with given raw array's values
  /// @param given_array the array that contains values
  /// @param size total value count of the given array
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

  /// @brief copy the array's values to another array
  /// @param to recreated array
  void copy_to(Array<T> &to) {
    if (to.get_length() != length) {
      to.reallocate(length);
    }

    copy_full(arr_ptr, to.get_ptr());
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Inline Member Funcs

  /// @brief get array pointer
  /// @return raw array pointer
  inline T *get_ptr() noexcept { return arr_ptr; }
  /// @brief const get array pointer
  /// @return raw array pointer
  inline T *get_ptr() const noexcept { return arr_ptr; }
  /// @brief get total value count
  /// @return value count
  inline int get_length() const noexcept { return length; }
  /// @brief get array's byte size
  /// @return array's byte size
  inline size_t get_byte_size() const noexcept { return byte_size; }
  /// @brief get template class's byte size
  /// @return typename's byte size
  inline size_t get_data_size() const noexcept { return sizeof(T); }

  /// @brief the function has to be executed in the move constructors. releasing
  /// class without deleting
  constexpr void exec_move() noexcept {
    arr_ptr = nullptr;
    length = 0;
    byte_size = 0;
  }

private:
  /// @brief create the array and fill the class variables
  /// @param size wanted size
  constexpr void allocate(int size) {
    PXD_ASSERT(arr_ptr == nullptr);

    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);
  }

  /// @brief create the array and copy the values from a raw array and fill the
  /// class variables
  /// @param given_array the array that contains values
  /// @param size total value count of the given array
  void allocate(T *given_array, int size) {
    PXD_ASSERT(arr_ptr == nullptr);
    PXD_ASSERT(given_array != nullptr);

    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);

    copy_full(given_array, arr_ptr);
  }

  /// @brief copy from array to to array with given size
  /// @param from source array
  /// @param to destination array
  /// @param size wanted copy size
  void copy(T *from, T *to, size_t size) {
    PXD_ASSERT(from != nullptr);
    PXD_ASSERT(to != nullptr);

    std::memcpy(to, from, size);
  }

  /// @brief copy all the values of from array to to array
  /// @param from source array
  /// @param to destination array
  inline void copy_full(T *from, T *to) { copy(from, to, byte_size); }

  void from(const Array<T> &given_array) noexcept {
    if (length != given_array.get_length()) {
      reallocate(given_array.get_length());
    }

    copy_full(given_array.get_ptr(), arr_ptr);
  }

  /// @brief compare the values of the array with the given array
  /// @param other given array
  /// @return true if all values are equal, false if any values are not equal
  bool compare(const Array<T> &other) noexcept {
    if (length != other.get_length() || byte_size != other.get_byte_size() ||
        !IS_VALID(other.get_ptr())) {
      return false;
    }

    bool is_same = true;
    compare_arrays(arr_ptr, other.get_ptr(), 0, length - 1, is_same);

    return is_same;
  }

  /// @brief compare arrays with divide & conquer
  /// @param first_array the array
  /// @param second_array the given array
  /// @param start start point, 0
  /// @param end end point, length - 1
  /// @param result true or false based on the equality of the values
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

private:
  T *arr_ptr = nullptr;
  int length = 0;
  size_t byte_size = 0;
};
} // namespace pxd
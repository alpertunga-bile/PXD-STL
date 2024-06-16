#pragma once

#include "checks.hpp"
#include <utility>

namespace pxd {

template<typename T>
class Array;

template<typename T>
class DynamicArray
{
public:
  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors

  constexpr DynamicArray() noexcept = default;
  constexpr DynamicArray(int size, int inc_size = 5)
    : inc_size_count(inc_size)
  {
    resize(size, true);
  }
  constexpr DynamicArray(T* values, int size) { expand(values, size); }
  constexpr DynamicArray(Array<T>& values) { expand(values); }
  constexpr DynamicArray(const DynamicArray& other)
  {
    expand(other.get_data(), other.get_array().get_length());
  }
  constexpr DynamicArray(DynamicArray&& other) noexcept
  {
    array = std::move(other.get_array());
    element_count = other.get_element_count();
    total_capacity = other.get_total_capacity();
    total_byte_size = other.get_total_size();
    inc_size_count = other.get_increment_count();

    other.exec_move();
  }
  constexpr DynamicArray& operator=(DynamicArray&& other)
  {
    release();

    array = std::move(other.get_array());
    element_count = other.get_element_count();
    total_capacity = other.get_total_capacity();
    total_byte_size = other.get_total_size();
    inc_size_count = other.get_increment_count();

    other.exec_move();

    return *this;
  }
  constexpr DynamicArray& operator=(const DynamicArray& other)
  {
    expand(other.get_data(), other.get_array().get_length());
  }
  inline ~DynamicArray() noexcept { release(); }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Operator Overloads

  constexpr decltype(auto) operator[](int index) { return array[index]; }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // DS Functionalities

  /// @brief append the given value to the back of the array
  /// @param value the given value
  void add(T& value)
  {
    if (total_capacity == 0) {
      resize(inc_size_count);
    }

    if (element_count == total_capacity) {
      add_capacity();
    }

    array[element_count] = value;

    element_count++;
  }
  /// @brief append the given value to the back of the array
  /// @param value the given value
  constexpr void add(T&& value) { add(value); }

  /// @brief get index of the value
  /// @param value wanted value to be found
  /// @return index value if successful, if not INDEX_NONE
  constexpr int where(T& value) noexcept { return array.where(value); }
  /// @brief get index of the value
  /// @param value wanted value to be found
  /// @return index value if successful, if not INDEX_NONE
  constexpr int where(T&& value) noexcept { return array.where(value); }

  /// @brief release the array
  inline void release() noexcept
  {
    array.release();
    element_count = 0;
    total_capacity = 0;
    total_byte_size = 0;
  }

  /// @brief recreate the array with the new size
  /// @param size wanted new size
  /// @param need_more need to add more size for the total capacity
  void reallocate(int size, bool need_more = false)
  {
    int new_size = need_more ? size + inc_size_count : size;

    array.reallocate(new_size);

    element_count = 0;
    total_capacity = new_size;
    total_byte_size = new_size * sizeof(T);
  }

  /// @brief truncate the non-used parts of the dynamic array and make it like a
  /// normal array
  void shrink()
  {
    if (total_capacity == element_count) {
      return;
    }

    array.resize(element_count);

    total_capacity = element_count;
    total_byte_size = element_count * sizeof(T);
  }

  /// @brief remove and return the last value of the array
  /// @return the last value
  T remove_last()
  {
    if (element_count == 0) {
      return T();
    }

    element_count--;
    T last_node = array[element_count];

    shrink();

    return last_node;
  }

  /// @brief expand the array with given raw array's values
  /// @param given_array the array that contains values
  /// @param size total value count of the given array
  void expand(T* new_values, int size)
  {
    array.expand(new_values, size);

    element_count += size;
    total_capacity += size;
    total_byte_size = total_capacity * sizeof(T);
  }

  /// @brief expand the array with given array's values
  /// @param given_array the array that contains values
  void expand(Array<T>& new_values)
  {
    int size = new_values.get_length();
    array.expand(new_values);

    element_count += size;
    total_capacity += size;
    total_byte_size = total_capacity * sizeof(T);
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Inline Member Funcs

  /// @brief get array value
  /// @return array
  inline Array<T> get_array() const noexcept { return array; }
  /// @brief get raw array pointer
  /// @return raw array pointer
  inline T* get_data() const noexcept { return array.get_ptr(); }
  inline int get_element_count() const noexcept { return element_count; }
  inline int get_total_capacity() const noexcept { return total_capacity; }
  inline std::size_t get_total_size() const noexcept { return total_byte_size; }
  inline std::size_t get_element_size() const noexcept
  {
    return (element_count * sizeof(T));
  }
  inline int get_increment_count() const noexcept { return inc_size_count; }

  /// @brief the function has to be executed in the move constructors. releasing
  /// class without deleting
  constexpr inline void exec_move() noexcept
  {
    array.exec_move();
    element_count = 0;
    total_capacity = 0;
    total_byte_size = 0;
    inc_size_count = 5;
  }

  /// @brief set increment size of the capacity
  /// @param inc_size wanted increment size
  constexpr inline void set_inc_size(int inc_size) noexcept
  {
    if (inc_size < 1) {
      return;
    }

    inc_size_count = inc_size;
  }

  /// @brief resize array with contained values
  /// @param new_size wanted new size
  void resize(int size, bool need_more = false)
  {
    int new_size = need_more ? size + inc_size_count : size;

    array.resize(new_size);

    element_count = element_count < size ? element_count : size;
    total_capacity = new_size;
    total_byte_size = new_size * sizeof(T);
  }

private:
  /// @brief resize to new increased size which is greater than the
  /// element_count with inc_size_count difference
  void add_capacity()
  {
    int new_size = total_capacity + inc_size_count;

    resize(new_size);
  }

private:
  Array<T> array;
  int element_count = 0;
  int total_capacity = 0;
  std::size_t total_byte_size = 0;
  int inc_size_count = 5;
};
} // namespace pxd
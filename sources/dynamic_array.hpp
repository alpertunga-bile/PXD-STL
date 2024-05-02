#pragma once

#include "checks.hpp"

namespace pxd {

template <typename T> class Array;

template <typename T> class DynamicArray {
public:
  DynamicArray() = default;
  DynamicArray(int size, int inc_size = 5) : inc_size_count(inc_size) {
    resize(size, true);
  }
  DynamicArray(T *values, int size) { expand(values, size); }
  DynamicArray(Array<T> &values) { expand(values); }
  DynamicArray(const DynamicArray &other) {
    expand(other.get_data(), other.get_array().get_length());
  }
  DynamicArray(DynamicArray &&other) {
    array = std::move(other.get_array());
    element_count = other.get_element_count();
    total_capacity = other.get_total_capacity();
    total_byte_size = other.get_total_size();
    inc_size_count = other.get_increment_count();

    other.exec_move();
  }
  DynamicArray &operator=(DynamicArray &&other) {
    release();

    array = std::move(other.get_array());
    element_count = other.get_element_count();
    total_capacity = other.get_total_capacity();
    total_byte_size = other.get_total_size();
    inc_size_count = other.get_increment_count();

    other.exec_move();

    return *this;
  }
  DynamicArray &operator=(const DynamicArray &other) {
    expand(other.get_data(), other.get_array().get_length());
  }
  ~DynamicArray() { release(); }

  decltype(auto) operator[](int index) { return array[index]; }

  void add(T &value) {
    if (total_capacity == 0) {
      resize(inc_size_count);
    }

    if (element_count == total_capacity) {
      add_capacity();
    }

    array[element_count] = value;

    element_count++;
  }

  int where(T &value) { return array.where(value); }
  int where(T &&value) { return array.where(value); }

  void add(T &&value) { add(value); }

  void release() {
    array.release();
    element_count = 0;
    total_capacity = 0;
    total_byte_size = 0;
  }

  void reallocate(int size, bool need_more = false) {
    int new_size = need_more ? size + inc_size_count : size;

    array.reallocate(new_size);

    element_count = 0;
    total_capacity = new_size;
    total_byte_size = new_size * sizeof(T);
  }

  void shrink() {
    if (total_capacity == element_count) {
      return;
    }

    array.resize(element_count);

    total_capacity = element_count;
    total_byte_size = element_count * sizeof(T);
  }

  void expand(T *new_values, int size) {
    array.expand(new_values, size);

    element_count += size;
    total_capacity += size;
    total_byte_size = total_capacity * sizeof(T);
  }

  void expand(Array<T> &new_values) {
    int size = new_values.get_length();
    array.expand(new_values);

    element_count += size;
    total_capacity += size;
    total_byte_size = total_capacity * sizeof(T);
  }

  inline Array<T> get_array() const { return array; }
  inline T *get_data() const { return array.get_ptr(); }
  inline int get_element_count() const { return element_count; }
  inline int get_total_capacity() const { return total_capacity; }
  inline size_t get_total_size() const { return total_byte_size; }
  inline size_t get_element_size() const { return (element_count * sizeof(T)); }
  inline int get_increment_count() const { return inc_size_count; }

  inline void exec_move() {
    array.exec_move();
    element_count = 0;
    total_capacity = 0;
    total_byte_size = 0;
    inc_size_count = 5;
  }

  inline void set_inc_size(int inc_size) {
    if (inc_size < 1) {
      return;
    }

    inc_size_count = inc_size;
  }

private:
  void add_capacity() {
    int new_size = total_capacity + inc_size_count;

    resize(new_size);
  }

  void resize(int size, bool need_more = false) {
    int new_size = need_more ? size + inc_size_count : size;

    array.resize(new_size);
    total_capacity = new_size;
    total_byte_size = new_size * sizeof(T);
  }

private:
  Array<T> array;
  int element_count = 0;
  int total_capacity = 0;
  size_t total_byte_size = 0;
  int inc_size_count = 5;
};
} // namespace pxd
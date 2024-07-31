#pragma once

#include "checks.hpp"

#include <cstddef>
#include <utility>

namespace pxd {
template <typename T> class Handle {
public:
  Handle() { ptr = new T(); };
  explicit Handle(T &value) {
    ptr = new T();
    *ptr = value;
  };
  explicit Handle(T value) {
    ptr = new T();
    *ptr = std::move(value);
  };
  Handle(const Handle<T> &other) = delete;
  Handle(Handle<T> &&other) = delete;

  auto operator=(const Handle<T> &other) -> Handle & = delete;
  auto operator=(Handle<T> &&other) -> Handle & = delete;
  ~Handle() { release(); };

  auto operator->() -> T & { return *ptr; }

  void recreate(T &value) {
    release();
    ptr = new T();
    *ptr = value;
  }

  void recreate(T value) { recreate(value); }

  void release() {
    if (ptr == nullptr) {
      return;
    }
    delete ptr;
    ptr = nullptr;
  }

  auto get() -> T * { return ptr; }
  auto get_value() const -> T { return *ptr; }

private:
  T *ptr = nullptr;
};

template <typename T> class Array;
template <typename T> class LinkedList;

template <typename T> class HandleArray {
public:
  HandleArray() = delete;
  explicit HandleArray(int size) { ptr = new T[size]; }
  HandleArray(T *array, int size) { from_array(array, size); };
  explicit HandleArray(Array<T> &array) {
    from_array(array.get_ptr(), array.get_length());
  }
  explicit HandleArray(LinkedList<T> &linked_list) {
    T *temp_array = new T[linked_list.get_length()];
    linked_list.to_array(temp_array);

    from_array(temp_array, linked_list.get_length());

    delete[] temp_array;
  }
  HandleArray(const HandleArray<T> &other) = delete;
  HandleArray(HandleArray<T> &&other) = delete;

  auto operator=(const HandleArray<T> &other) -> HandleArray & = delete;
  auto operator=(HandleArray<T> &&other) -> HandleArray & = delete;

  ~HandleArray() { release(); };

  auto operator[](int index) -> decltype(auto) {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      return ptr[length + index];
    }

    return ptr[index];
  }

  void release() {
    if (ptr == nullptr) {
      return;
    }

    delete[] ptr;
    ptr = nullptr;
    length = 0;
  }

  void from_array(T *array, int size) {
    release();

    ptr = new T[size];

    memcpy(ptr, array, size * sizeof(T));

    length = size;
    byte_size = size * sizeof(T);
  }

  void to_array(Array<T> &array) { array.reallocate(ptr, length); }

  void to_linked_list(LinkedList<T> &linked_list) {
    linked_list.from_array(ptr, length);
  }

  auto get_array() const -> T * { return ptr; }
  auto get_length() const -> int { return length; }
  auto get_byte_size() const -> int { return byte_size; }

private:
  T *ptr;
  int length;
  std::size_t byte_size = 0;
};
} // namespace pxd
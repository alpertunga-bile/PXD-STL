#pragma once

#include "checks.hpp"

#include <cstddef>

namespace pxd {
template<typename T>
class Handle
{
public:
  Handle() { ptr = new T(); };
  Handle(T& value)
  {
    ptr = new T();
    *ptr = value;
  };
  Handle(T&& value)
  {
    ptr = new T();
    *ptr = value;
  };
  Handle(const Handle<T>& other) = delete;
  Handle(Handle<T>&& other) = delete;
  Handle& operator=(const Handle<T>& other) = delete;
  ~Handle() { release(); };

  T& operator->() { return *ptr; }

  void recreate(T& value)
  {
    release();
    ptr = new T();
    *ptr = value;
  }

  void recreate(T&& value) { recreate(value); }

  void release()
  {
    if (ptr == nullptr) {
      return;
    }
    delete ptr;
    ptr = nullptr;
  }

  inline T* get() { return ptr; }
  inline T get_value() const { return *ptr; }

private:
  T* ptr = nullptr;
};

template<typename T>
class Array;
template<typename T>
class LinkedList;

template<typename T>
class HandleArray
{
public:
  HandleArray() = delete;
  HandleArray(int size) { ptr = new T[size]; }
  HandleArray(T* array, int size) { from_array(array, size); };
  HandleArray(Array<T>& array)
  {
    from_array(array.get_ptr(), array.get_length());
  }
  HandleArray(LinkedList<T>& linked_list)
  {
    T* temp_array = new T[linked_list.get_length()];
    linked_list.to_array(temp_array);

    from_array(temp_array, linked_list.get_length());

    delete[] temp_array;
  }
  HandleArray(const HandleArray<T>& other) = delete;
  HandleArray(HandleArray<T>&& other) = delete;
  HandleArray& operator=(const HandleArray<T>& other) = delete;
  ~HandleArray() { release(); };

  decltype(auto) operator[](int index)
  {
    PXD_ASSERT(index < length);

    if (index < 0) {
      PXD_ASSERT(length + index >= 0);
      return ptr[length + index];
    }

    return ptr[index];
  }

  void release()
  {
    if (ptr == nullptr) {
      return;
    }

    delete[] ptr;
    ptr = nullptr;
    length = 0;
  }

  void from_array(T* array, int size)
  {
    release();

    ptr = new T[size];

    memcpy(ptr, array, size * sizeof(T));

    length = size;
    byte_size = size * sizeof(T);
  }

  void to_array(Array<T>& array) { array.reallocate(ptr, length); }

  void to_linked_list(LinkedList<T>& linked_list)
  {
    linked_list.from_array(ptr, length);
  }

  inline T* get_array() const { return ptr; }
  inline int get_length() const { return length; }
  inline int get_byte_size() const { return byte_size; }

private:
  T* ptr;
  int length;
  std::size_t byte_size = 0;
};
} // namespace pxd
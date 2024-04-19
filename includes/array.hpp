#pragma once

#define ARRAY_SIZE(arr) get_array_size(arr)

template <typename T, size_t N> inline int get_array_size(T (&)[N] array) {
  return N;
}

namespace pxd {
template <typename T> class Array {
public:
  Array() = delete; // default constructor
  Array(int size) { allocate(size); }
  Array(T *given_array, int size) {
    allocate(size);
    copy(given_array, arr_ptr, size);
  }

  Array(const Array<T> &other) = delete;            // copy constructor
  Array(Array<T> &&other) = delete;                 // move constructor
  Array &operator=(const Array<T> &other) = delete; // copy assignment

  ~Array() // deconstructor
  {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;
  }

  decltype(auto) operator==(Array<T> &other) {
    return other.arr_ptr == arr_ptr;
  }

  decltype(auto) operator[](int index) {
    if (index < 0) {
      if ((index * -1) > length) {
        throw "Negative Index Out of Bound";
      }

      return arr_ptr[length - (index * -1)];
    } else if (index >= length) {
      throw "Index Out of Bound";
    }

    return arr_ptr[index];
  }

  void resize(int new_size) {
    T *temp_array = new T[length];
    copy(arr_ptr, temp_array, length);

    release();
    allocate(temp_array, new_size);

    delete[] temp_array;
  }

  void recreate(T *given_array, int size) {
    release();
    allocate(given_array, size);
  }

  void release() {
    if (arr_ptr == nullptr) {
      return;
    }

    delete[] arr_ptr;
    length = 0;
    byte_size = 0;
  }

  void copy_to(Array<T> &to) {
    int i = 0;
    for (i = 0; i < length; i++) {
      to[i] = arr_ptr[i];
    }
  }

  inline int get_length() { return length; }
  inline T *get_ptr() { return arr_ptr; }
  inline size_t get_byte_size() { return byte_size; }
  inline float get_mbyte_size() { return ((float)byte_size) / 1024.f; }
  inline float get_gbyte_size() {
    return ((float)byte_size) / (1024.f * 1024.f);
  }

private:
  void allocate(int size) {
    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);
  }

  void allocate(T *given_array, int size) {
    arr_ptr = new T[size];
    length = size;
    byte_size = size * sizeof(T);
    copy(given_array, arr_ptr, size);
  }

  void copy(T *from, T *to, int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
      to[i] = from[i];
    }
  }

private:
  T *arr_ptr = nullptr;
  int length = 0;
  size_t byte_size = 0;
};
} // namespace pxd
#pragma once

#include "checks.hpp"

namespace pxd {

template <typename T> class Array;

template <typename T> class Matrix {
public:
  Matrix() = default;
  Matrix(int row, int column) { allocate(row, column); }
  Matrix(T *values, int row, int column) {
    allocate(row, column);
    memcpy(matrix, values, get_byte_size());
  }
  Matrix(Array<T> &values, int row, int column) {
    allocate(row, column);
    memcpy(matrix, values.get_ptr(), get_byte_size());
  }
  Matrix(const Matrix<T> &other) {
    allocate(other.get_row(), other.get_column());
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }
  Matrix(Matrix<T> &&other) {
    allocate(other.get_row(), other.get_column());
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }
  Matrix &operator=(const Matrix<T> &other) {
    allocate(other.get_row(), other.get_column());
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
    return *this;
  }
  ~Matrix() { release(); }

  decltype(auto) operator[](int index) {
    PXD_ASSERT(index < row);

    int start_stride = index * column;

    Array<T> row_array(matrix + start_stride, column);

    return row_array;
  }

  decltype(auto) operator()(int row, int column) {
    PXD_ASSERT(row < this->row && column < this->column);

    return matrix[row * this->column + column];
  }

  void release() {
    if (matrix == nullptr) {
      return;
    }

    delete[] matrix;
    row = 0;
    column = 0;
    element_count = 0;
    b_row_order = true;
  }

  void transpose() {
    int current_row = 0;
    int current_col = 0;
    int normal_index = 0;
    int transpose_index = 0;
    T temp;

    for (int i = 0; i < element_count; i++) {
      current_row = i / row;
      current_col = i % column;

      normal_index = current_row * column + current_col;
      transpose_index = current_col * column + current_row;

      if (current_col == current_row) {
        i += (column - current_col - 1);
        continue;
      }

      temp = matrix[normal_index];
      matrix[normal_index] = matrix[transpose_index];
      matrix[transpose_index] = temp;
    }

    b_row_order = !b_row_order;
  }

  inline void change_order() { transpose(); }

  inline T *get_matrix() { return matrix; }
  inline T *get_matrix() const { return matrix; }
  inline int get_row() const { return row; }
  inline int get_column() const { return column; }
  inline int get_element_count() const { return element_count; }
  inline size_t get_byte_size() const { return element_count * sizeof(T); }
  inline bool is_row_order() const { return b_row_order; }

private:
  void allocate(int row, int column) {
    element_count = row * column;

    matrix = new T[element_count];

    this->row = row;
    this->column = column;
    b_row_order = true;
  }

  inline void from_array(T *values) {
    size_t byte_size = element_count * sizeof(T);
    memcpy(matrix, values, byte_size);
  }

  inline void from_matrix(const Matrix<T> &other) {
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }

  inline void from_matrix(Matrix<T> &&other) {
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }

private:
  T *matrix = nullptr;
  int row = 0;
  int column = 0;
  int element_count = 0;
  bool b_row_order = true;
};
} // namespace pxd
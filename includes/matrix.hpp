#pragma once

#include "checks.hpp"

#include <cstddef>
#include <memory>

namespace pxd {

template <typename T> class Array;

template <typename T> class Matrix {
public:
  constexpr Matrix() noexcept = default;
  Matrix(int row, int column) { allocate(row, column); }
  Matrix(T *values, int row, int column) {
    allocate(row, column);
    memcpy(matrix.get(), values, get_byte_size());
  }
  Matrix(const Matrix<T> &other) {
    allocate(other.get_row(), other.get_column());
    memcpy(matrix.get(), other.get_matrix(), other.get_byte_size());
  }
  constexpr Matrix(Matrix<T> &&other) noexcept
      : matrix(other.get_matrix()), row(other.get_row()),
        column(other.get_column()), element_count(other.get_element_count()),
        b_row_order(other.is_row_order()) {
    other.exec_move();
  }
  constexpr auto operator=(Matrix<T> &&other) noexcept -> Matrix & {
    release();

    matrix = other.get_matrix();
    row = other.get_row();
    column = other.get_column();
    element_count = other.get_element_count();
    b_row_order = other.is_row_order();

    other.exec_move();

    return *this;
  }
  Matrix &operator=(const Matrix<T> &other) {
    allocate(other.get_row(), other.get_column());
    memcpy(matrix, other.get_matrix(), other.get_byte_size());

    row = other.get_row();
    column = other.get_column();
    element_count = other.get_element_count();
    b_row_order = other.is_row_order();

    return *this;
  }

  ~Matrix() noexcept { release(); }

  auto operator[](int index) -> decltype(auto) {
    PXD_ASSERT(index < row);

    int start_stride = index * column;

    Array<T> row_array(matrix.get() + start_stride, column);

    return row_array;
  }

  auto operator()(int row, int column) -> decltype(auto) {
    PXD_ASSERT(row < this->row && column < this->column);

    return matrix.get()[row * this->column + column];
  }

  void release() noexcept {
    if (matrix == nullptr) {
      return;
    }

    matrix.reset();

    row = 0;
    column = 0;
    element_count = 0;
    b_row_order = true;
  }

  void transpose() noexcept {
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

      temp = matrix.get()[normal_index];
      matrix.get()[normal_index] = matrix.get()[transpose_index];
      matrix.get()[transpose_index] = temp;
    }

    b_row_order = !b_row_order;
  }

  void change_order() noexcept { transpose(); }

  auto get_matrix() noexcept -> T * { return matrix.get(); }
  auto get_matrix() const noexcept -> T * { return matrix.get(); }
  auto get_row() const noexcept -> int { return row; }
  auto get_column() const noexcept -> int { return column; }
  auto get_element_count() const noexcept -> int { return element_count; }
  auto get_byte_size() const noexcept -> std::size_t {
    return element_count * sizeof(T);
  }

  auto is_row_order() const noexcept -> bool { return b_row_order; }

  constexpr void exec_move() noexcept {
    matrix = nullptr;
    row = 0;
    column = 0;
    element_count = 0;
    b_row_order = true;
  }

private:
  constexpr void allocate(int row, int column) noexcept {
    element_count = row * column;

    matrix = std::make_shared<T>(new T[element_count]);

    this->row = row;
    this->column = column;
    b_row_order = true;
  }

  void from_array(T *values) {
    std::size_t byte_size = element_count * sizeof(T);
    memcpy(matrix, values, byte_size);
  }

  void from_matrix(const Matrix<T> &other) {
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }

  void from_matrix(Matrix<T> other) {
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }

private:
  std::shared_ptr<T> matrix;
  int row = 0;
  int column = 0;
  int element_count = 0;
  bool b_row_order = true;
};
} // namespace pxd
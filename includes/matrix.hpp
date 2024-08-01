#pragma once

#include "checks.hpp"

#include <array>
#include <cstddef>

namespace pxd {

template <typename T> class Array;

template <typename T, size_t TotalRow, size_t TotalCol> class Matrix {
public:
  constexpr Matrix() noexcept { init(); }
  explicit Matrix(T *values) {
    init();
    memcpy(matrix.data(), values, get_byte_size());
  }
  Matrix(const Matrix<T, TotalRow, TotalCol> &other) {
    init();
    memcpy(matrix.data(), other.get_ptr(), other.get_byte_size());
  }
  constexpr Matrix(Matrix<T, TotalRow, TotalCol> &&other) noexcept
      : matrix(other.get_matrix()), row(other.get_row()),
        column(other.get_column()), element_count(other.get_element_count()),
        b_row_order(other.is_row_order()) {
    other.exec_move();
  }
  constexpr auto
  operator=(Matrix<T, TotalRow, TotalCol> &&other) noexcept -> Matrix & {
    release();

    matrix = other.get_matrix();
    row = other.get_row();
    column = other.get_column();
    element_count = other.get_element_count();
    b_row_order = other.is_row_order();

    other.exec_move();

    return *this;
  }
  auto operator=(const Matrix<T, TotalRow, TotalCol> &other) -> Matrix & {
    if (matrix == other.get_matrix()) {
      return *this;
    }

    init();
    memcpy(matrix.data(), other.get_matrix(), other.get_byte_size());

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

    Array<T> row_array(matrix.data() + start_stride, column);

    return row_array;
  }

  auto operator()(int row, int column) -> decltype(auto) {
    PXD_ASSERT(row < this->row && column < this->column);

    return matrix[row * this->column + column];
  }

  void release() noexcept {
    matrix.empty();

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

      temp = matrix[normal_index];
      matrix[normal_index] = matrix[transpose_index];
      matrix[transpose_index] = temp;
    }

    b_row_order = !b_row_order;
  }

  void change_order() noexcept { transpose(); }

  auto get_matrix() noexcept -> std::array<T, TotalRow * TotalCol> {
    return matrix;
  }
  auto get_matrix() const noexcept -> std::array<T, TotalRow * TotalCol> {
    return matrix;
  }
  auto get_ptr() noexcept -> const T * { return matrix.data(); }
  auto get_ptr() const noexcept -> const T * { return matrix.data(); }
  auto get_row() const noexcept -> int { return row; }
  auto get_column() const noexcept -> int { return column; }
  auto get_element_count() const noexcept -> int { return element_count; }
  auto get_byte_size() const noexcept -> std::size_t {
    return element_count * sizeof(T);
  }

  auto is_row_order() const noexcept -> bool { return b_row_order; }

  constexpr void exec_move() noexcept {
    row = 0;
    column = 0;
    element_count = 0;
    b_row_order = true;
  }

private:
  constexpr void init() noexcept {
    element_count = TotalRow * TotalCol;

    this->row = TotalRow;
    this->column = TotalCol;
    b_row_order = true;
  }

  void from_array(T *values) {
    std::size_t byte_size = element_count * sizeof(T);
    memcpy(matrix, values, byte_size);
  }

  void from_matrix(const Matrix<T, TotalRow, TotalCol> &other) {
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }

  void from_matrix(Matrix<T, TotalRow, TotalCol> other) {
    memcpy(matrix, other.get_matrix(), other.get_byte_size());
  }

private:
  std::array<T, TotalRow * TotalCol> matrix;
  int row = 0;
  int column = 0;
  int element_count = 0;
  bool b_row_order = true;
}; // namespace pxd
} // namespace pxd
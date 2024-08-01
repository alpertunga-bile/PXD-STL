#pragma once

#include "matrix.hpp"
#include "test_utils.hpp"

namespace pxd {
class MatrixTests : public ITest {
public:
  void start_test() override {
    int *temp_arr = new int[N];

    for (int i = 0; i < N; i++) {
      temp_arr[i] = i + 1;
    }

    start_array_ctor_tests(temp_arr);
    start_copy_ctor_tests(temp_arr);
    start_move_ctor_tests(temp_arr);
    start_assign_ctor_tests(temp_arr);
    start_row_index_tests(temp_arr);
    start_double_indexing_tests(temp_arr);
    start_parant_double_indexing_tests(temp_arr);
    start_transpose_tests(temp_arr);

    delete[] temp_arr;
  }

private:
  void start_array_ctor_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);

    test_results["array ctor"] =
        check_arrays<int>(temp_arr, matrix.get_matrix().data(), N);
  }

  void start_copy_ctor_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);
    Matrix<int, 4, 4> temp(matrix);

    test_results["copy ctor"] =
        check_arrays<int>(temp_arr, temp.get_matrix().data(), N);
  }

  void start_move_ctor_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);
    Matrix<int, 4, 4> temp(std::move(matrix));

    test_results["move ctor"] =
        check_arrays<int>(temp_arr, temp.get_matrix().data(), N);

    Matrix<int, 4, 4> temp_2 = std::move(temp);

    test_results["assign move ctor"] =
        check_arrays<int>(temp_arr, temp_2.get_matrix().data(), N);
  }

  void start_assign_ctor_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);
    Matrix<int, 4, 4> temp = matrix;

    test_results["assign ctor"] =
        check_arrays<int>(temp_arr, temp.get_ptr(), N);
  }

  void start_row_index_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);

    auto temp_row = matrix[0];

    test_results["row indexing"] =
        check_arrays<int>(temp_row.get_ptr(), temp_arr, 4);

    temp_row = matrix[1];

    test_results["row indexing 2"] =
        check_arrays<int>(temp_row.get_ptr(), 0, temp_arr, 1 * 4, 4);

    temp_row = matrix[3];

    test_results["row indexing 3"] =
        check_arrays<int>(temp_row.get_ptr(), 0, temp_arr, 3 * 4, 4);
  }

  void start_double_indexing_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);

    test_results["double indexing"] = matrix[0][2] == temp_arr[2] &&
                                      matrix[1][2] == temp_arr[6] &&
                                      matrix[2][3] == temp_arr[11];
  }

  void start_parant_double_indexing_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);

    test_results["parant double indexing"] = matrix(0, 2) == temp_arr[2] &&
                                             matrix(1, 2) == temp_arr[6] &&
                                             matrix(2, 3) == temp_arr[11];

    matrix(0, 2) = 123;

    test_results["assign parant double indexing"] = matrix[0][2] != temp_arr[2];
  }

  void start_transpose_tests(int *temp_arr) {
    Matrix<int, 4, 4> matrix(temp_arr);

    matrix.transpose();

    int transposed[16] = {1, 5, 9,  13, 2, 6, 10, 14,
                          3, 7, 11, 15, 4, 8, 12, 16};

    test_results["transpose"] =
        check_arrays<int>(matrix.get_matrix().data(), transposed, 16);
  }

private:
  int N = 16;
};
} // namespace pxd
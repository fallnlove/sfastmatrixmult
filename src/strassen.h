#pragma once

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include "matrix.h"
#include "simple_multiplication.h"

namespace s_fast {

namespace detail {

size_t GetNearestPowerOfTwo(size_t number) {
    size_t power_of_two = 1;
    while (power_of_two < number) {
        power_of_two *= 2;
    }
    return power_of_two;
}

template <class T>
std::vector<Matrix<T>> GetSubMatrixes(const Matrix<T>& matrix) {
    size_t rows = GetNearestPowerOfTwo(matrix.Rows());
    size_t columns = GetNearestPowerOfTwo(matrix.Columns());

    assert(rows > 1 && columns > 1);

    return {matrix.GetSubMatrix({0, 0}, {rows / 2 - 1, columns / 2 - 1}),
            matrix.GetSubMatrix({0, columns / 2}, {rows / 2 - 1, columns - 1}),
            matrix.GetSubMatrix({rows / 2, 0}, {rows - 1, columns / 2 - 1}),
            matrix.GetSubMatrix({rows / 2, columns / 2}, {rows - 1, columns - 1})};
}

template <class T>
void SetSubMatrix(Matrix<T>& to, const Matrix<T>& from, std::pair<size_t, size_t> begin) {
    for (size_t row = begin.first; row < std::min(to.Rows(), from.Rows() + begin.first); ++row) {
        for (size_t column = begin.second;
             column < std::min(to.Columns(), from.Columns() + begin.second); ++column) {
            to(row, column) = from(row - begin.first, column - begin.second);
        }
    }
}

}  // namespace detail

template <class T>
Matrix<T> Strassen(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    assert(lhs.Columns() == rhs.Rows());

    if (std::min({lhs.Rows(), lhs.Columns(), rhs.Columns()}) <= 8) {
        return SimpleMultiplication(lhs, rhs);
    }

    std::vector<Matrix<T>> lhs_sub_mat = std::move(detail::GetSubMatrixes(lhs));
    std::vector<Matrix<T>> rhs_sub_mat = std::move(detail::GetSubMatrixes(rhs));

    std::vector<Matrix<T>> tmp_matrixes;
    tmp_matrixes.reserve(7);

    tmp_matrixes.push_back(
        Strassen(lhs_sub_mat[0] + lhs_sub_mat[3], rhs_sub_mat[0] + rhs_sub_mat[3]));
    tmp_matrixes.push_back(Strassen(lhs_sub_mat[2] + lhs_sub_mat[3], rhs_sub_mat[0]));
    tmp_matrixes.push_back(Strassen(lhs_sub_mat[0], rhs_sub_mat[1] - rhs_sub_mat[3]));
    tmp_matrixes.push_back(Strassen(lhs_sub_mat[3], rhs_sub_mat[2] - rhs_sub_mat[0]));
    tmp_matrixes.push_back(Strassen(lhs_sub_mat[0] + lhs_sub_mat[1], rhs_sub_mat[3]));
    tmp_matrixes.push_back(
        Strassen(lhs_sub_mat[2] - lhs_sub_mat[0], rhs_sub_mat[0] + rhs_sub_mat[1]));
    tmp_matrixes.push_back(
        Strassen(lhs_sub_mat[1] - lhs_sub_mat[3], rhs_sub_mat[2] + rhs_sub_mat[3]));

    tmp_matrixes[6] += tmp_matrixes[0];
    tmp_matrixes[6] += tmp_matrixes[3];
    tmp_matrixes[6] -= tmp_matrixes[4];

    tmp_matrixes[4] += tmp_matrixes[2];

    tmp_matrixes[3] += tmp_matrixes[1];

    tmp_matrixes[0] -= tmp_matrixes[1];
    tmp_matrixes[0] += tmp_matrixes[2];
    tmp_matrixes[0] += tmp_matrixes[5];

    Matrix<T> res(lhs.Rows(), rhs.Columns());

    detail::SetSubMatrix<T>(res, tmp_matrixes[6], {0, 0});
    detail::SetSubMatrix<T>(res, tmp_matrixes[4], {0, tmp_matrixes[6].Columns()});
    detail::SetSubMatrix<T>(res, tmp_matrixes[3], {tmp_matrixes[6].Rows(), 0});
    detail::SetSubMatrix<T>(res, tmp_matrixes[0],
                            {tmp_matrixes[6].Rows(), tmp_matrixes[6].Columns()});

    return res;
}

}  // namespace s_fast
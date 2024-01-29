#pragma once

#include <cstddef>
#include "matrix.h"

namespace s_fast {

template <class T>
Matrix<T> SimpleMultiplication(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> result(lhs.Rows(), rhs.Columns());

    for (size_t row = 0; row < lhs.Rows(); ++row) {
        for (size_t column = 0; column < rhs.Columns(); ++column) {
            for (size_t i = 0; i < lhs.Columns(); ++i) {
                result(row, column) += lhs(row, i) * rhs(i, column);
            }
        }
    }

    return result;
}

template <class T>
Matrix<T> SimpleMultiplicationWithTranspose(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    assert(lhs.Columns() == rhs.Rows());

    Matrix<T> result(lhs.Rows(), rhs.Columns());
    Matrix<T> rhs_t = Transpose(rhs);

    for (size_t row = 0; row < lhs.Rows(); ++row) {
        for (size_t column = 0; column < rhs.Columns(); ++column) {
            for (size_t i = 0; i < lhs.Columns(); ++i) {
                result(row, column) += lhs(row, i) * rhs(i, column);
            }
        }
    }

    return result;
}

}  // namespace s_fast

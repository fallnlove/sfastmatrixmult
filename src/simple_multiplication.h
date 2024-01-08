#pragma once

#include <cstddef>
#include "matrix.h"

namespace s_fast {

template <class T>
Matrix<T> SimpleMultiplication(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> result(lhs.Rows(), rhs.Columns());

    for (size_t row = 0; row < lhs.Rows(); ++row) {
        for (size_t column = 0; column < rhs.Columns(); ++column) {
            T tmp = 0;
            for (size_t i = 0; i < lhs.Columns(); ++i) {
                tmp += lhs(row, i) * rhs(i, column);
            }
            result(row, column) = tmp;
        }
    }

    return result;
}

template <class T>
Matrix<T> SimpleMultiplicationWithTranspose(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> result(lhs.Rows(), rhs.Columns());
    Matrix<T> rhs_t = rhs.Transpose();

    for (size_t row = 0; row < lhs.Rows(); ++row) {
        for (size_t column = 0; column < rhs.Columns(); ++column) {
            T tmp = 0;
            for (size_t i = 0; i < lhs.Columns(); ++i) {
                tmp += lhs(row, i) * rhs_t(column, i);
            }
            result(row, column) = tmp;
        }
    }

    return result;
}

}  // namespace s_fast
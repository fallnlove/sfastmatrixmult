#pragma once

#include <cstddef>
#include "matrix.h"

namespace s_fast {

template <class T>
Matrix<T> SimpleMultiplication(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    using Index = typename Matrix<T>::Index;
    Matrix<T> result(lhs.Rows(), rhs.Columns());

    for (Index row = 0; row < lhs.Rows(); ++row) {
        for (Index column = 0; column < rhs.Columns(); ++column) {
            for (Index i = 0; i < lhs.Columns(); ++i) {
                result(row, column) += lhs(row, i) * rhs(i, column);
            }
        }
    }

    return result;
}

template <class T>
Matrix<T> SimpleMultiplicationWithTranspose(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    using Index = typename Matrix<T>::Index;
    assert(lhs.Columns() == rhs.Rows());

    Matrix<T> result(lhs.Rows(), rhs.Columns());
    Matrix<T> rhs_t = Transpose(rhs);

    for (Index row = 0; row < lhs.Rows(); ++row) {
        for (Index column = 0; column < rhs.Columns(); ++column) {
            for (Index i = 0; i < lhs.Columns(); ++i) {
                result(row, column) += lhs(row, i) * rhs_t(column, i);
            }
        }
    }

    return result;
}

}  // namespace s_fast

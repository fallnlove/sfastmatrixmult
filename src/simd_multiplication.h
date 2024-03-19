#pragma once

#include <cstddef>
#include "matrix.h"
#include "xsimd/xsimd.hpp"

namespace s_fast {

template <class T>
Matrix<T> SimdMultiplication(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    using Index = typename Matrix<T>::Index;
    using SIMDtype = xsimd::batch<T>;

    assert(lhs.Columns() == rhs.Rows());

    Matrix<T> result(lhs.Rows(), rhs.Columns());
    Matrix<T> rhs_t = Transpose(rhs);

    for (Index row = 0; row < lhs.Rows(); ++row) {
        for (Index column = 0; column < rhs.Columns(); ++column) {
            Index register_size = SIMDtype::size;
            Index vec_size = lhs.Columns() - lhs.Columns() % register_size;

            for (Index i = 0; i < vec_size; i += register_size) {
                SIMDtype lhs_vec = SIMDtype::load_unaligned(&lhs.data_[row * lhs.Columns() + i]);
                SIMDtype rhs_vec =
                    SIMDtype::load_unaligned(&rhs_t.data_[column * rhs_t.Columns() + i]);

                result(row, column) += xsimd::reduce_add(lhs_vec * rhs_vec);
            }

            for (Index i = vec_size; i < lhs.Columns(); ++i) {
                result(row, column) += lhs(row, i) * rhs_t(column, i);
            }
        }
    }

    return result;
}

}  // namespace s_fast

#pragma once

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

#include "matrix.h"
#include "simple_multiplication.h"
#include "view_matrix.h"
#include "utils.h"

namespace s_fast {

namespace detail_strassen {

template <class T>
Matrix<T> Strassen(const ConstViewMatrix<T>& lhs, const ConstViewMatrix<T>& rhs) {
    using utils::BlockMatrix;
    using utils::GetSubMatrixes;
    using utils::SetSubMatrix;

    assert(lhs.Columns() == rhs.Rows());

    if (std::min({lhs.Rows(), lhs.Columns(), rhs.Columns()}) <= 8) {
        return SimpleMultiplication(GetMatrix(lhs), GetMatrix(rhs));
    }

    auto lhs_sub = GetSubMatrixes<const ConstViewMatrix<T>, ConstViewMatrix<T>>(lhs);
    auto rhs_sub = GetSubMatrixes<const ConstViewMatrix<T>, ConstViewMatrix<T>>(rhs);

    Matrix<T> m1 =
        Strassen(lhs_sub.left_top + lhs_sub.right_bottom, rhs_sub.left_top + rhs_sub.right_bottom);
    Matrix<T> m2 = Strassen(lhs_sub.left_bottom + lhs_sub.right_bottom, rhs_sub.left_top);
    Matrix<T> m3 = Strassen(lhs_sub.left_top, rhs_sub.right_top - rhs_sub.right_bottom);
    Matrix<T> m4 = Strassen(lhs_sub.right_bottom, rhs_sub.left_bottom - rhs_sub.left_top);
    Matrix<T> m5 = Strassen(lhs_sub.left_top + lhs_sub.right_top, rhs_sub.right_bottom);
    Matrix<T> m6 =
        Strassen(lhs_sub.left_bottom - lhs_sub.left_top, rhs_sub.left_top + rhs_sub.right_top);
    Matrix<T> m7 = Strassen(lhs_sub.right_top - lhs_sub.right_bottom,
                            rhs_sub.left_bottom + rhs_sub.right_bottom);

    m7 += m1;
    m7 += m4;
    m7 -= m5;
    m5 += m3;
    m4 += m2;
    m1 -= m2;
    m1 += m3;
    m1 += m6;

    Matrix<T> result(lhs.Rows(), rhs.Columns());

    SetSubMatrix<T>(m7, {0, 0}, &result);
    SetSubMatrix<T>(m5, {0, m7.Columns()}, &result);
    SetSubMatrix<T>(m4, {m7.Rows(), 0}, &result);
    SetSubMatrix<T>(m1, {m7.Rows(), m7.Columns()}, &result);

    return result;
}

}  // namespace detail_strassen

template <class T>
Matrix<T> Strassen(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return detail_strassen::Strassen(ConstViewMatrix<T>(lhs), ConstViewMatrix<T>(rhs));
}

template <class T>
Matrix<T> Strassen(const ConstViewMatrix<T>& lhs, const Matrix<T>& rhs) {
    return detail_strassen::Strassen(lhs, ConstViewMatrix<T>(rhs));
}

template <class T>
Matrix<T> Strassen(const Matrix<T>& lhs, const ConstViewMatrix<T>& rhs) {
    return detail_strassen::Strassen(ConstViewMatrix<T>(lhs), rhs);
}

}  // namespace s_fast

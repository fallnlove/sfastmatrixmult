#pragma once

#include "matrix.h"
#include "view_matrix.h"
#include "simple_multiplication.h"
#include "utils.h"

namespace s_fast {

namespace detail_cache_oblivious {

template <class T>
void CacheObliviousMult(const ConstViewMatrix<T>& lhs, const ConstViewMatrix<T>& rhs,
                        ViewMatrix<T>& result) {
    using utils::GetSubMatrixesCacheOblivious;

    if (std::min({lhs.Rows(), lhs.Columns(), rhs.Columns()}) <= 8) {
        result += SimpleMultiplication(GetMatrix(lhs), GetMatrix(rhs));
        return;
    }

    auto lhs_sub = GetSubMatrixesCacheOblivious<const ConstViewMatrix<T>, ConstViewMatrix<T>>(lhs);
    auto rhs_sub = GetSubMatrixesCacheOblivious<const ConstViewMatrix<T>, ConstViewMatrix<T>>(rhs);
    auto result_sub = GetSubMatrixesCacheOblivious<ViewMatrix<T>, ViewMatrix<T>>(result);

    CacheObliviousMult(lhs_sub.left_top, rhs_sub.left_top, result_sub.left_top);
    CacheObliviousMult(lhs_sub.right_top, rhs_sub.left_bottom, result_sub.left_top);

    CacheObliviousMult(lhs_sub.left_top, rhs_sub.right_top, result_sub.right_top);
    CacheObliviousMult(lhs_sub.right_top, rhs_sub.right_bottom, result_sub.right_top);

    CacheObliviousMult(lhs_sub.left_bottom, rhs_sub.left_top, result_sub.left_bottom);
    CacheObliviousMult(lhs_sub.right_bottom, rhs_sub.left_bottom, result_sub.left_bottom);

    CacheObliviousMult(lhs_sub.left_bottom, rhs_sub.right_top, result_sub.right_bottom);
    CacheObliviousMult(lhs_sub.right_bottom, rhs_sub.right_bottom, result_sub.right_bottom);
}

}  // namespace detail_cache_oblivious

template <class T>
Matrix<T> CacheObliviousMult(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    using Index = typename Matrix<T>::Index;
    using detail_cache_oblivious::CacheObliviousMult;

    Matrix<T> result(lhs.Rows(), rhs.Columns());
    ViewMatrix<T> result_view(result);

    CacheObliviousMult(ConstViewMatrix<T>(lhs), ConstViewMatrix<T>(rhs), result_view);

    return result;
}

}  // namespace s_fast

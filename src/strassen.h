#pragma once

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include "matrix.h"
#include "simple_multiplication.h"
#include "view_matrix.h"

namespace s_fast {

namespace detail_strassen {

using Index = typename Matrix<int>::Index;
using Position = typename Matrix<int>::Position;

inline Index GetNearestPowerOfTwo(Index number) {
    Index power_of_two = 1;
    while (power_of_two < number) {
        power_of_two *= 2;
    }
    return power_of_two;
}

template <class ContainerType>
struct BlockMatrix {
    ContainerType left_top;
    ContainerType right_top;
    ContainerType left_bottom;
    ContainerType right_bottom;
};

template <class InputContainer, class OuputContainer>
BlockMatrix<OuputContainer> GetSubMatrixes(InputContainer& matrix) {
    Index rows = GetNearestPowerOfTwo(matrix.Rows());
    Index columns = GetNearestPowerOfTwo(matrix.Columns());

    assert(rows > 1 && columns > 1);

    return {.left_top = OuputContainer(matrix, {0, 0}, {rows / 2, columns / 2}),
            .right_top = OuputContainer(matrix, {0, columns / 2}, {rows / 2, columns}),
            .left_bottom = OuputContainer(matrix, {rows / 2, 0}, {rows, columns / 2}),
            .right_bottom = OuputContainer(matrix, {rows / 2, columns / 2}, {rows, columns})};
}

template <class T>
void SetSubMatrix(const Matrix<T>& from, Position begin, Matrix<T>* to) {
    for (Index row = begin.row; row < std::min(to->Rows(), from.Rows() + begin.row); ++row) {
        for (Index column = begin.column;
             column < std::min(to->Columns(), from.Columns() + begin.column); ++column) {
            (*to)(row, column) = from(row - begin.row, column - begin.column);
        }
    }
}

}  // namespace detail_strassen

template <class T>
Matrix<T> Strassen(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    using detail_strassen::BlockMatrix;
    using detail_strassen::GetSubMatrixes;
    using detail_strassen::SetSubMatrix;

    assert(lhs.Columns() == rhs.Rows());

    if (std::min({lhs.Rows(), lhs.Columns(), rhs.Columns()}) <= 8) {
        return SimpleMultiplication(lhs, rhs);
    }

    auto lhs_sub = GetSubMatrixes<const Matrix<T>, ConstViewMatrix<T>>(lhs);
    auto rhs_sub = GetSubMatrixes<const Matrix<T>, ConstViewMatrix<T>>(rhs);

    Matrix<T> m1 =
        Strassen(lhs_sub.left_top + lhs_sub.right_bottom, rhs_sub.left_top + rhs_sub.right_bottom);
    Matrix<T> m2 =
        Strassen(lhs_sub.left_bottom + lhs_sub.right_bottom, GetMatrix(rhs_sub.left_top));
    Matrix<T> m3 = Strassen(GetMatrix(lhs_sub.left_top), rhs_sub.right_top - rhs_sub.right_bottom);
    Matrix<T> m4 =
        Strassen(GetMatrix(lhs_sub.right_bottom), rhs_sub.left_bottom - rhs_sub.left_top);
    Matrix<T> m5 = Strassen(lhs_sub.left_top + lhs_sub.right_top, GetMatrix(rhs_sub.right_bottom));
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

}  // namespace s_fast

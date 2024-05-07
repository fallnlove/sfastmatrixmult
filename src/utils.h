#pragma once

#include "matrix.h"

namespace s_fast {

namespace utils {

using Index = typename Matrix<int>::Index;
using Position = typename Matrix<int>::Position;

constexpr Index kStopStrassenConstant = 16;
constexpr Index kStopCacheObliviousConstant = 16;

template <class ContainerType>
struct BlockMatrix {
    ContainerType left_top;
    ContainerType right_top;
    ContainerType left_bottom;
    ContainerType right_bottom;
};

template <class InputContainer, class OuputContainer>
BlockMatrix<OuputContainer> GetSubMatrixesStrassen(InputContainer& matrix) {
    Index rows = matrix.Rows() + (matrix.Rows() % 2 == 0 ? 0 : 1);
    Index columns = matrix.Columns() + (matrix.Columns() % 2 == 0 ? 0 : 1);

    assert(rows > 1 && columns > 1);

    return {.left_top = OuputContainer(matrix, {0, 0}, {rows / 2, columns / 2}),
            .right_top = OuputContainer(matrix, {0, columns / 2}, {rows / 2, columns}),
            .left_bottom = OuputContainer(matrix, {rows / 2, 0}, {rows, columns / 2}),
            .right_bottom = OuputContainer(matrix, {rows / 2, columns / 2}, {rows, columns})};
}

template <class InputContainer, class OuputContainer>
BlockMatrix<OuputContainer> GetSubMatrixesCacheOblivious(InputContainer& matrix) {
    Index rows = matrix.Rows();
    Index columns = matrix.Columns();

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

}  // namespace utils

}  // namespace s_fast

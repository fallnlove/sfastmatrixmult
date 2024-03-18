#pragma once

#include "matrix.h"

namespace s_fast {

namespace utils {

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

}  // namespace utils

}  // namespace s_fast

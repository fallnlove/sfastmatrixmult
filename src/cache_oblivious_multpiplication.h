#pragma once

#include "matrix.h"

namespace s_fast {

template <class T>
Matrix<T> CacheObliviousMult(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    using Index = typename Matrix<T>::Index;
    Matrix<T> result(lhs.Rows(), rhs.Columns());
}

}  // namespace s_fast

#pragma once

#include "../src/matrix.h"

namespace s_fast {

namespace utils {

template <class T>
bool IsEqualMatrix(const s_fast::Matrix<T>& lhs, const s_fast::Matrix<T>& rhs) {
    if (lhs.Rows() != rhs.Rows() || lhs.Columns() != rhs.Columns()) {
        return false;
    }

    for (size_t i = 0; i < lhs.Rows(); ++i) {
        for (size_t j = 0; j < lhs.Columns(); ++j) {
            if (lhs(i, j) != rhs(i, j)) {
                return false;
            }
        }
    }

    return true;
}

}  // namespace utils

}  // namespace s_fast
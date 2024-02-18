#pragma once

#include "matrix.h"

namespace s_fast {

template <class T>
class ViewMatrix {
public:
    using Position = typename Matrix<T>::Position;
    using Index = typename Matrix<T>::Index;
    using ReturnElementType = typename Matrix<T>::ReturnElementType;

    ViewMatrix(const Matrix<T>& matrix, Position begin, Position end)
        : matrix_(matrix), begin_(begin), end_(end) {
    }

    ViewMatrix(ViewMatrix<T>& matrix, Position begin, Position end)
        : matrix_(matrix.matrix_),
          begin_({matrix.begin_.row + begin.row, matrix.begin_.column + begin.column}),
          end_({matrix.begin_.row + end.row, matrix.begin_.column + end.column}) {
    }

    ViewMatrix() = delete;

    Index Rows() const {
        return end_.row - begin_.row;
    }

    Index Columns() const {
        return end_.column - begin_.column;
    }

    ReturnElementType operator()(Index row, Index column) const {
        assert(row < Rows() && column < Columns());

        if (begin_.row + row >= matrix_.Rows() || begin_.column + column >= matrix_.Columns()) {
            return 0;
        }

        return matrix_(begin_.row + row, begin_.column + column);
    }

    ViewMatrix<T>& operator+=(const ViewMatrix<T>& other) {
        assert(Rows() == other.Rows() && Columns() == other.Columns());

        for (Index i = 0; i < Rows(); ++i) {
            for (Index j = 0; j < Columns(); ++j) {
                matrix_(begin_.row + i, begin_.column + j) += other(i, j);
            }
        }

        return *this;
    }

    ViewMatrix<T>& operator-=(const ViewMatrix<T>& other) {
        assert(Rows() == other.Rows() && Columns() == other.Columns());

        for (Index i = 0; i < Rows(); ++i) {
            for (Index j = 0; j < Columns(); ++j) {
                matrix_(begin_.row + i, begin_.column + j) -= other(i, j);
            }
        }

        return *this;
    }

private:
    Matrix<T> matrix_;

    Position begin_;
    Position end_;

    inline friend Matrix<T> operator+(const ViewMatrix<T>& lhs, const ViewMatrix<T>& rhs) {
        assert(lhs.Rows() == rhs.Rows() && lhs.Columns() == rhs.Columns());

        Matrix<T> result(lhs.Rows(), lhs.Columns());

        for (Index i = 0; i < lhs.Rows(); ++i) {
            for (Index j = 0; j < lhs.Columns(); ++j) {
                result(i, j) = lhs(i, j) + rhs(i, j);
            }
        }

        return result;
    }
    inline friend Matrix<T> operator-(const ViewMatrix<T>& lhs, const ViewMatrix<T>& rhs) {
        assert(lhs.Rows() == rhs.Rows() && lhs.Columns() == rhs.Columns());

        Matrix<T> result(lhs.Rows(), lhs.Columns());

        for (Index i = 0; i < lhs.Rows(); ++i) {
            for (Index j = 0; j < lhs.Columns(); ++j) {
                result(i, j) = lhs(i, j) - rhs(i, j);
            }
        }

        return result;
    }

    inline friend bool operator==(const ViewMatrix<T>& lhs, const ViewMatrix<T>& rhs) {
        if (lhs.Rows() != rhs.Rows() && lhs.Columns() != rhs.Columns()) {
            return false;
        }

        for (Index i = 0; i < lhs.Rows(); ++i) {
            for (Index j = 0; j < lhs.Columns(); ++j) {
                if (lhs(i, j) != rhs(i, j)) {
                    return false;
                }
            }
        }

        return true;
    }
};

template <class T>
bool operator!=(const ViewMatrix<T>& lhs, const ViewMatrix<T>& rhs) {
    return !(lhs == rhs);
}

template <class T>
Matrix<T> GetMatrix(const ViewMatrix<T>& view_matrix) {
    using Index = typename Matrix<T>::Index;

    Matrix<T> matrix(view_matrix.Rows(), view_matrix.Columns());

    for (Index i = 0; i < view_matrix.Rows(); ++i) {
        for (Index j = 0; j < view_matrix.Columns(); ++j) {
            matrix(i, j) = view_matrix(i, j);
        }
    }

    return matrix;
}

}  // namespace s_fast

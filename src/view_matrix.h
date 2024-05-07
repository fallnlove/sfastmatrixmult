#pragma once

#include <functional>
#include "matrix.h"
#include "view_matrix_helper.h"

namespace s_fast {

namespace view_matrix_detail {

template <class T, bool IsConst>
class RawViewMatrix {
public:
    using Position = typename Matrix<T>::Position;
    using Index = typename Matrix<T>::Index;

    using ReturnElementType = view_matrix_helper::ReturnIfConst<T, IsConst>;
    using ConstReturnElementType = helper::ReturnAs<T>;

    using MatrixType = view_matrix_helper::ContainerType<Matrix<T>, IsConst>;
    using ViewMatrixType = view_matrix_helper::ContainerType<RawViewMatrix<T, IsConst>, IsConst>;

    RawViewMatrix(MatrixType& matrix, Position begin = {0, 0}, Position end = {-1, -1})
        : matrix_(matrix),
          begin_(begin),
          end_({end.row == -1 ? matrix.Rows() : end.row,
                end.column == -1 ? matrix.Columns() : end.column}) {
    }

    RawViewMatrix(ViewMatrixType& matrix, Position begin = {0, 0}, Position end = {-1, -1})
        : matrix_(matrix.matrix_),
          begin_({matrix.begin_.row + begin.row, matrix.begin_.column + begin.column}),
          end_({end.row == -1 ? matrix.end_.row : matrix.begin_.row + end.row,
                end.column == -1 ? matrix.end_.column : matrix.begin_.column + end.column}) {
    }

    RawViewMatrix() = delete;

    Index Rows() const {
        return end_.row - begin_.row;
    }

    Index ExistedRows() const {
        return std::min(Rows(), matrix_.Rows() - begin_.row);
    }

    Index Columns() const {
        return end_.column - begin_.column;
    }

    Index ExistedColumns() const {
        return std::min(Columns(), matrix_.Columns() - begin_.column);
    }

    ReturnElementType operator()(Index row, Index column) {
        assert(0 <= row && begin_.row + row < matrix_.Rows() && 0 <= column &&
               begin_.column + column < matrix_.Columns());

        return matrix_(begin_.row + row, begin_.column + column);
    }

    ConstReturnElementType operator()(Index row, Index column) const {
        assert(0 <= row && row < Rows() && 0 <= column && column < Columns());

        if (begin_.row + row >= matrix_.Rows() || begin_.column + column >= matrix_.Columns()) {
            return 0;
        }

        return matrix_(begin_.row + row, begin_.column + column);
    }

    template <class TMatrix>
    RawViewMatrix& operator+=(const TMatrix& other) {
        assert(Rows() == other.Rows() && Columns() == other.Columns());

        for (Index i = 0; i < ExistedRows(); ++i) {
            for (Index j = 0; j < ExistedColumns(); ++j) {
                matrix_(i + begin_.row, j + begin_.column) += other(i, j);
            }
        }

        return *this;
    }

    template <class TMatrix>
    RawViewMatrix& operator-=(const TMatrix& other) {
        assert(Rows() == other.Rows() && Columns() == other.Columns());

        for (Index i = 0; i < ExistedRows(); ++i) {
            for (Index j = 0; j < ExistedColumns(); ++j) {
                matrix_(i + begin_.row, j + begin_.column) -= other(i, j);
            }
        }

        return *this;
    }

    friend Matrix<T> operator+(const RawViewMatrix& lhs, const RawViewMatrix& rhs) {
        assert(lhs.Rows() == rhs.Rows() && lhs.Columns() == rhs.Columns());

        Matrix<T> result(lhs.Rows(), lhs.Columns());
        RawViewMatrix<T, false> result_view(result);
        result_view += lhs;
        result_view += rhs;
        return result;
    }
    friend Matrix<T> operator-(const RawViewMatrix& lhs, const RawViewMatrix& rhs) {
        assert(lhs.Rows() == rhs.Rows() && lhs.Columns() == rhs.Columns());

        Matrix<T> result(lhs.Rows(), lhs.Columns());
        RawViewMatrix<T, false> result_view(result);
        result_view += lhs;
        result_view -= rhs;
        return result;
    }

    friend bool operator==(const RawViewMatrix& lhs, const RawViewMatrix& rhs) {
        if (lhs.Rows() != rhs.Rows() || lhs.Columns() != rhs.Columns()) {
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

private:
    MatrixType& matrix_;

    Position begin_;
    Position end_;
};

template <class T, bool IsConst>
bool operator!=(const RawViewMatrix<T, IsConst>& lhs, const RawViewMatrix<T, IsConst>& rhs) {
    return !(lhs == rhs);
}

}  // namespace view_matrix_detail

template <class T>
using ViewMatrix = view_matrix_detail::RawViewMatrix<T, false>;

template <class T>
using ConstViewMatrix = view_matrix_detail::RawViewMatrix<T, true>;

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

template <class T>
Matrix<T> GetMatrix(const ConstViewMatrix<T>& view_matrix) {
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

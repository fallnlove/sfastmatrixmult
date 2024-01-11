#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

namespace s_fast {

template <class T>
class Matrix {
public:
    Matrix() : rows_(0), columns_(0) {
    }

    explicit Matrix(size_t n) : data_(n * n, 0), rows_(n), columns_(n) {
    }

    Matrix(size_t rows, size_t columns) : data_(rows * columns, 0), rows_(rows), columns_(columns) {
    }

    Matrix(const std::vector<std::vector<T>>& matrix)
        : data_(matrix.empty() ? 0 : matrix.size() * matrix.back().size()),
          rows_(matrix.size()),
          columns_(matrix.empty() ? 0 : matrix.back().size()) {

        for (size_t row = 0; row < rows_; ++row) {
            for (size_t column = 0; column < columns_; ++column) {
                (*this)(row, column) = matrix[row][column];
            }
        }
    }

    size_t Rows() const {
        return rows_;
    }

    size_t Columns() const {
        return columns_;
    }

    T& operator()(size_t row, size_t column) {
        assert(row < rows_ && column < columns_);

        return data_[row * columns_ + column];
    }

    const T& operator()(size_t row, size_t column) const {
        assert(row < rows_ && column < columns_);

        return data_[row * columns_ + column];
    }

    Matrix<T>& operator+=(const Matrix<T>& other) {
        assert(rows_ == other.Rows() && columns_ == other.Columns());

        for (size_t row = 0; row < rows_; ++row) {
            for (size_t column = 0; column < columns_; ++column) {
                (*this)(row, column) += other(row, column);
            }
        }

        return *this;
    }

    Matrix<T>& operator-=(const Matrix<T>& other) {
        assert(rows_ == other.Rows() && columns_ == other.Columns());

        for (size_t row = 0; row < rows_; ++row) {
            for (size_t column = 0; column < columns_; ++column) {
                (*this)(row, column) -= other(row, column);
            }
        }

        return *this;
    }

    Matrix<T> Transpose() const {
        Matrix<T> transpose(columns_, rows_);
        for (size_t row = 0; row < rows_; ++row) {
            for (size_t column = 0; column < columns_; ++column) {
                transpose(column, row) = (*this)(row, column);
            }
        }
        return transpose;
    }

    // This function can be made more faster
    Matrix<T> GetSubMatrix(std::pair<size_t, size_t> begin, std::pair<size_t, size_t> end) const {
        assert(end.first >= begin.first && end.second >= begin.second);
        Matrix<T> sub_matrix(end.first - begin.first + 1, end.second - begin.second + 1);

        for (size_t row = begin.first; row < std::min(rows_, end.first + 1); ++row) {
            for (size_t column = begin.second; column < std::min(columns_, end.second + 1);
                 ++column) {
                sub_matrix(row - begin.first, column - begin.second) = (*this)(row, column);
            }
        }

        return sub_matrix;
    }

private:
    std::vector<T> data_;
    size_t rows_;
    size_t columns_;
};

template <class T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> tmp(lhs);
    tmp += rhs;
    return tmp;
}

template <class T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> tmp(lhs);
    tmp -= rhs;
    return tmp;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    for (size_t row = 0; row < matrix.Rows(); ++row) {
        for (size_t column = 0; column < matrix.Columns(); ++column) {
            os << matrix(row, column) << (column == matrix.Columns() - 1 ? "\n" : " ");
        }
    }
    return os;
}

}  // namespace s_fast

#pragma once

#include <cstddef>
#include <vector>

namespace s_fast {

template <class T>
class Matrix {
public:
    Matrix() : rows_(0), columns_(0) {
    }

    Matrix(size_t n) : data_(n * n, 0), rows_(n), columns_(n) {
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
        return data_[row * columns_ + column];
    }

    const T& operator()(size_t row, size_t column) const {
        return data_[row * columns_ + column];
    }

    Matrix<T>& operator+=(Matrix<T> other) {
        for (size_t i = 0; i < rows_ * columns_; ++i) {
            data_[i] += other[i];
        }
        return *this;
    }

    Matrix<T>& operator-=(Matrix<T> other) {
        for (size_t i = 0; i < rows_ * columns_; ++i) {
            data_[i] -= other[i];
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

}  // namespace s_fast

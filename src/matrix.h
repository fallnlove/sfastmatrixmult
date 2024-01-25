#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

#include "helper.h"

namespace s_fast {

template <class T>
class Matrix {
public:
    using Index = int64_t;
    using ReturnElementType = helper::ReturnAs<T>;

    Matrix() = default;

    Matrix(const Matrix& other) = default;

    Matrix(Matrix&& other) noexcept : data_(std::move(other.data_)), columns_(other.columns_) {
        other.columns_ = 0;
    }

    Matrix& operator=(const Matrix& other) = default;

    Matrix& operator=(Matrix&& other) noexcept {
        data_ = std::move(other.data_);
        columns_ = other.columns_;

        other.columns_ = 0;

        return *this;
    }

    explicit Matrix(Index n) : data_(n * n, 0), columns_(n) {
    }

    Matrix(Index rows, Index columns) : data_(rows * columns, 0), columns_(columns) {
    }

    Matrix(std::initializer_list<std::initializer_list<T>> data)
        : data_(std::empty(data) ? 0 : data.size() * data.begin()->size()),
          columns_(std::empty(data) ? 0 : data.begin()->size()) {

        Index i = 0;

        for (const auto& row : data) {
            assert(row.size() == Columns() && "All rows must have the same size!");
            for (const auto& element : row) {
                data_[i] = element;
                ++i;
            }
        }
    }

    Index Rows() const {
        return columns_ == 0 ? 0 : data_.size() / columns_;
    }

    Index Columns() const {
        return columns_;
    }

    T& operator()(Index row, Index column) {
        assert(row < Rows() && column < Columns());

        return data_[row * Columns() + column];
    }

    ReturnElementType operator()(Index row, Index column) const {
        assert(row < Rows() && column < Columns());

        return data_[row * Columns() + column];
    }

    Matrix<T>& operator+=(const Matrix<T>& other) {
        assert(Rows() == other.Rows() && Columns() == other.Columns());

        for (size_t i = 0; i < data_.size(); ++i) {
            data_[i] += other.data_[i];
        }

        return *this;
    }

    Matrix<T>& operator-=(const Matrix<T>& other) {
        assert(Rows() == other.Rows() && Columns() == other.Columns());

        for (size_t i = 0; i < data_.size(); ++i) {
            data_[i] -= other.data_[i];
        }

        return *this;
    }

    // This function can be made much faster
    Matrix<T> GetSubMatrix(std::pair<size_t, size_t> begin, std::pair<size_t, size_t> end) const {
        assert(end.first >= begin.first && end.second >= begin.second);
        Matrix<T> sub_matrix(end.first - begin.first + 1, end.second - begin.second + 1);

        for (size_t row = begin.first; row < std::min(Rows(), static_cast<int64_t>(end.first) + 1);
             ++row) {
            for (size_t column = begin.second;
                 column < std::min(Columns(), static_cast<int64_t>(end.second) + 1); ++column) {
                sub_matrix(row - begin.first, column - begin.second) = (*this)(row, column);
            }
        }

        return sub_matrix;
    }

private:
    std::vector<T> data_;
    Index columns_ = 0;
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
Matrix<T> Transpose(const Matrix<T>& other) {
    Matrix<T> transpose(other.Columns(), other.Rows());

    for (size_t row = 0; row < other.Rows(); ++row) {
        for (size_t column = 0; column < other.Columns(); ++column) {
            transpose(column, row) = other(row, column);
        }
    }

    return transpose;
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

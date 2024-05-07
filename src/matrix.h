#pragma once

#include <cassert>
#include <cstddef>
#include <random>
#include <vector>

#include "helper.h"

namespace s_fast {

template <class T>
class Matrix;

template <class T>
Matrix<T> SimdMultiplication(const Matrix<T>& lhs, const Matrix<T>& rhs);

template <class T>
class Matrix {
public:
    using Index = int64_t;
    using ReturnElementType = helper::ReturnAs<T>;

    struct Position {
        Index row;
        Index column;
    };

    Matrix() = default;

    Matrix(const Matrix& other) = default;

    Matrix(Matrix&& other) noexcept
        : data_(std::move(other.data_)), columns_(std::exchange(other.columns_, 0)) {
    }

    Matrix& operator=(const Matrix& other) = default;

    Matrix& operator=(Matrix&& other) noexcept {
        data_ = std::move(other.data_);
        columns_ = std::exchange(other.columns_, 0);

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
        assert(0 <= row && row < Rows() && 0 <= column && column < Columns());

        return data_[row * Columns() + column];
    }

    ReturnElementType operator()(Index row, Index column) const {
        assert(0 <= row && row < Rows() && 0 <= column && column < Columns());

        return data_[row * Columns() + column];
    }

    Matrix& operator+=(const Matrix& other) {
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

    Matrix<T>& operator*=(const T& element) {
        for (size_t i = 0; i < data_.size(); ++i) {
            data_[i] *= element;
        }

        return *this;
    }

private:
    inline friend Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
        Matrix<T> tmp(lhs);
        tmp += rhs;
        return tmp;
    }
    inline friend Matrix<T> operator+(Matrix<T>&& lhs, const Matrix<T>& rhs) {
        Matrix tmp = std::move(lhs);
        tmp += rhs;
        return tmp;
    }
    inline friend Matrix<T> operator+(const Matrix<T>& lhs, Matrix<T>&& rhs) {
        return std::move(rhs) + lhs;
    }
    inline friend Matrix<T> operator+(Matrix<T>&& lhs, Matrix<T>&& rhs) {
        Matrix tmp = std::move(lhs);
        tmp += rhs;
        return tmp;
    }

    inline friend Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) {
        Matrix<T> tmp(lhs);
        tmp -= rhs;
        return tmp;
    }
    inline friend Matrix<T> operator-(Matrix<T>&& lhs, const Matrix<T>& rhs) {
        Matrix tmp = std::move(lhs);
        tmp -= rhs;
        return tmp;
    }
    inline friend Matrix<T> operator-(const Matrix<T>& lhs, Matrix<T>&& rhs) {
        rhs *= -1;
        rhs += lhs;
        return rhs;
    }
    inline friend Matrix<T> operator-(Matrix<T>&& lhs, Matrix<T>&& rhs) {
        Matrix tmp = std::move(lhs);
        tmp -= rhs;
        return tmp;
    }

    inline friend bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs) {
        return lhs.Rows() == rhs.Rows() && lhs.Columns() == rhs.Columns() && lhs.data_ == rhs.data_;
    }

    friend Matrix SimdMultiplication<T>(const Matrix&, const Matrix&);

private:
    std::vector<T> data_;
    Index columns_ = 0;
};

template <class T>
bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return !(lhs == rhs);
}

//  TODO: cache-oblivious transpose
template <class T>
Matrix<T> Transpose(const Matrix<T>& other) {
    using Index = typename Matrix<T>::Index;
    Matrix<T> transpose(other.Columns(), other.Rows());

    for (Index row = 0; row < other.Rows(); ++row) {
        for (Index column = 0; column < other.Columns(); ++column) {
            transpose(column, row) = other(row, column);
        }
    }

    return transpose;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    using Index = typename Matrix<T>::Index;

    for (Index row = 0; row < matrix.Rows(); ++row) {
        for (Index column = 0; column < matrix.Columns(); ++column) {
            os << matrix(row, column) << (column == matrix.Columns() - 1 ? "\n" : " ");
        }
    }
    return os;
}

template <class T, class Distribution>
Matrix<T> Random(typename Matrix<T>::Index rows, typename Matrix<T>::Index columns,
                 Distribution distribution, uint64_t random_seed = 42) {
    using Index = typename Matrix<T>::Index;

    Matrix<T> random_matrix(rows, columns);
    std::mt19937 random(random_seed);

    for (Index i = 0; i < rows; ++i) {
        for (Index j = 0; j < columns; ++j) {
            random_matrix(i, j) = distribution(random);
        }
    }

    return random_matrix;
}

}  // namespace s_fast

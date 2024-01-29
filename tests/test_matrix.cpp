#include <gtest/gtest.h>
#include <cstddef>
#include <vector>

#include "../src/matrix.h"

TEST(MatrixCorrection, Constructors) {
    s_fast::Matrix<int> a;
    EXPECT_EQ(a.Rows(), 0);
    EXPECT_EQ(a.Columns(), 0);

    s_fast::Matrix<int> b(10);
    EXPECT_EQ(b.Rows(), 10);
    EXPECT_EQ(b.Columns(), 10);

    s_fast::Matrix<int> c(13, 14);
    EXPECT_EQ(c.Rows(), 13);
    EXPECT_EQ(c.Columns(), 14);

    std::vector<std::vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    s_fast::Matrix<int> d({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); ++j) {
            EXPECT_EQ(d(i, j), mat[i][j]);
        }
    }
}

TEST(MatrixCorrection, Plus) {
    s_fast::Matrix<int> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    s_fast::Matrix<int> b({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}});
    s_fast::Matrix<int> c({{4, 4, 4}, {10, 10, 10}, {16, 16, 16}});

    EXPECT_TRUE(a + b == c);
}

TEST(MatrixCorrection, Minus) {
    s_fast::Matrix<int> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    s_fast::Matrix<int> b({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}});
    s_fast::Matrix<int> c({{4, 4, 4}, {10, 10, 10}, {16, 16, 16}});

    EXPECT_TRUE(c - b == a);
}

TEST(MatrixCorrection, Transpose) {
    s_fast::Matrix<int> a({{1, 0, 0}, {0, 5, 0}, {0, 0, 9}});
    EXPECT_TRUE(Transpose(a) == a);

    s_fast::Matrix<int> b({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}});
    s_fast::Matrix<int> c({{3, 6, 9}, {2, 5, 8}, {1, 4, 7}});
    EXPECT_TRUE(Transpose(b) == c);
    EXPECT_TRUE(Transpose(c) == b);
}

TEST(MatrixCorrection, Random) {
    using Index = typename s_fast::Matrix<int>::Index;
    s_fast::Matrix<int> a =
        s_fast::Random<int>(5, 6, std::uniform_int_distribution<int>(-100, 100));
    EXPECT_TRUE(a.Rows() == 5);
    EXPECT_TRUE(a.Columns() == 6);

    for (Index i = 0; i < a.Rows(); ++i) {
        for (Index j = 0; j < a.Columns(); ++j) {
            EXPECT_TRUE(-100 <= a(i, j));
            EXPECT_TRUE(a(i, j) <= 100);
        }
    }
}

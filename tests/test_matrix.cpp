#include <gtest/gtest.h>
#include <cstddef>
#include <vector>

#include "../src/matrix.h"
#include "utils.h"

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

    EXPECT_TRUE(s_fast::utils::IsEqualMatrix(a + b, c));
}

TEST(MatrixCorrection, Minus) {
    s_fast::Matrix<int> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    s_fast::Matrix<int> b({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}});
    s_fast::Matrix<int> c({{4, 4, 4}, {10, 10, 10}, {16, 16, 16}});

    EXPECT_TRUE(s_fast::utils::IsEqualMatrix(c - b, a));
}

TEST(MatrixCorrection, Transpose) {
    s_fast::Matrix<int> a({{1, 0, 0}, {0, 5, 0}, {0, 0, 9}});
    EXPECT_TRUE(s_fast::utils::IsEqualMatrix(Transpose(a), a));

    s_fast::Matrix<int> b({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}});
    s_fast::Matrix<int> c({{3, 6, 9}, {2, 5, 8}, {1, 4, 7}});
    EXPECT_TRUE(s_fast::utils::IsEqualMatrix(Transpose(b), c));
    EXPECT_TRUE(s_fast::utils::IsEqualMatrix(Transpose(c), b));
}

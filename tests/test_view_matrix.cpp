#include <gtest/gtest.h>
#include <cstddef>
#include <vector>

#include "../src/matrix.h"
#include "../src/view_matrix.h"

TEST(ViewMatrixCorrection, Constructors) {
    using s_fast::Matrix;
    using s_fast::ViewMatrix;
    using Index = Matrix<int>::Index;

    Matrix<int> matrix({});
    ViewMatrix<int> view_a(matrix, {0, 0}, {0, 0});
    EXPECT_EQ(view_a.Rows(), 0);
    EXPECT_EQ(view_a.Columns(), 0);

    ViewMatrix<int> view_b(matrix, {0, 0}, {10, 10});
    EXPECT_EQ(view_b.Rows(), 10);
    EXPECT_EQ(view_b.Columns(), 10);

    ViewMatrix<int> view_c(matrix, {1, 3}, {14, 17});
    EXPECT_EQ(view_c.Rows(), 13);
    EXPECT_EQ(view_c.Columns(), 14);

    std::vector<std::vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix<int> d({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    ViewMatrix<int> d_view(d);
    for (Index i = 0; i < mat.size(); ++i) {
        for (Index j = 0; j < mat[i].size(); ++j) {
            EXPECT_EQ(d_view(i, j), mat[i][j]);
        }
    }
}

TEST(ViewMatrixCorrection, ConstructorsFromView) {
    using s_fast::Matrix;
    using s_fast::ViewMatrix;
    using Index = Matrix<int>::Index;

    Matrix<int> matrix({});
    ViewMatrix<int> view_a(matrix, {0, 0}, {0, 0});
    ViewMatrix<int> view_view_a(view_a, {0, 0}, {0, 0});
    EXPECT_EQ(view_view_a.Rows(), 0);
    EXPECT_EQ(view_view_a.Columns(), 0);

    ViewMatrix<int> view_view_b(view_a, {1, 2}, {12, 5});
    EXPECT_EQ(view_view_b.Rows(), 11);
    EXPECT_EQ(view_view_b.Columns(), 3);

    std::vector<std::vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix<int> d({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    ViewMatrix<int> d_view(d);
    ViewMatrix<int> d_view_view(d_view);

    EXPECT_EQ(d_view_view.Rows(), 3);
    EXPECT_EQ(d_view_view.Columns(), 3);

    for (Index i = 0; i < mat.size(); ++i) {
        for (Index j = 0; j < mat[i].size(); ++j) {
            EXPECT_EQ(d_view_view(i, j), mat[i][j]);
        }
    }
}

TEST(ViewMatrixCorrection, GetByIndex) {
    using s_fast::Matrix;
    using s_fast::ViewMatrix;
    using Index = Matrix<int>::Index;

    Matrix<int> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    ViewMatrix<int> view_a(a, {1, 1}, {3, 3});

    for (Index i = 0; i < 2; ++i) {
        for (Index j = 0; j < 2; ++j) {
            EXPECT_EQ(a(i + 1, j + 1), view_a(i, j));
        }
    }
}

TEST(ViewMatrixCorrection, Plus) {
    using s_fast::Matrix;
    using s_fast::ViewMatrix;

    Matrix<int> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix<int> b({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}});
    Matrix<int> c({{4, 4, 4}, {10, 10, 10}, {16, 16, 16}});

    EXPECT_TRUE(ViewMatrix<int>(a, {0, 0}, {3, 3}) + ViewMatrix<int>(b, {0, 0}, {3, 3}) == c);

    auto a_view = ViewMatrix<int>(a, {0, 0}, {3, 3});
    a_view += ViewMatrix<int>(b, {0, 0}, {3, 3});
    EXPECT_TRUE(a_view == c);
}

TEST(ViewMatrixCorrection, Minus) {
    using s_fast::Matrix;
    using s_fast::ViewMatrix;

    Matrix<int> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix<int> b({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}});
    Matrix<int> c({{4, 4, 4}, {10, 10, 10}, {16, 16, 16}});

    EXPECT_TRUE(ViewMatrix<int>(c, {0, 0}, {3, 3}) - ViewMatrix<int>(b, {0, 0}, {3, 3}) == a);

    auto c_view = ViewMatrix<int>(a, {0, 0}, {3, 3});
    c_view -= ViewMatrix<int>(b, {0, 0}, {3, 3});
    EXPECT_TRUE(c_view == a);
}

TEST(ViewMatrixCorrection, GetMatrix) {
    using s_fast::GetMatrix;
    using s_fast::Matrix;
    using s_fast::ViewMatrix;

    Matrix<int> a({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

    ViewMatrix<int> view_a = ViewMatrix<int>(a);
    Matrix<int> a_copy = GetMatrix(view_a);

    EXPECT_TRUE(a_copy == a);
}

#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>

#include "../src/simple_multiplication.h"

TEST(SimpleMultTest, Correctness3x3) {
    using s_fast::Matrix;

    Matrix<int> a({{1, 6, 3}, {2, -4, 2}, {0, 8, 3}});
    Matrix<int> b({{-3, 4, 0}, {1, -5, 4}, {2, 0, 0}});
    Matrix<int> res({{9, -26, 24}, {-6, 28, -16}, {14, -40, 32}});

    Matrix<int> return_val = s_fast::SimpleMultiplication(a, b);

    EXPECT_TRUE(res == return_val);
}

TEST(SimpleMultWithTransposeTest, Correctness3x3) {
    using s_fast::Matrix;

    Matrix<int> a({{1, 6, 3}, {2, -4, 2}, {0, 8, 3}});
    Matrix<int> b({{-3, 4, 0}, {1, -5, 4}, {2, 0, 0}});
    Matrix<int> res({{9, -26, 24}, {-6, 28, -16}, {14, -40, 32}});

    Matrix<int> return_val = s_fast::SimpleMultiplicationWithTranspose(a, b);

    EXPECT_TRUE(res == return_val);
}

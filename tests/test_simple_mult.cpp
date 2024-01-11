#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>

#include "../src/simple_multiplication.h"
#include "utils.h"

TEST(SimpleMultTest, Correctness3x3) {
    s_fast::Matrix<int> a({{1, 6, 3}, {2, -4, 2}, {0, 8, 3}});
    s_fast::Matrix<int> b({{-3, 4, 0}, {1, -5, 4}, {2, 0, 0}});
    s_fast::Matrix<int> res({{9, -26, 24}, {-6, 28, -16}, {14, -40, 32}});

    s_fast::Matrix<int> return_val = s_fast::SimpleMultiplication(a, b);

    EXPECT_TRUE(s_fast::utils::IsEqualMatrix<int>(res, return_val));
}

TEST(SimpleMultWithTransposeTest, Correctness3x3) {
    s_fast::Matrix<int> a({{1, 6, 3}, {2, -4, 2}, {0, 8, 3}});
    s_fast::Matrix<int> b({{-3, 4, 0}, {1, -5, 4}, {2, 0, 0}});
    s_fast::Matrix<int> res({{9, -26, 24}, {-6, 28, -16}, {14, -40, 32}});

    s_fast::Matrix<int> return_val = s_fast::SimpleMultiplicationWithTranspose(a, b);

    EXPECT_TRUE(s_fast::utils::IsEqualMatrix<int>(res, return_val));
}

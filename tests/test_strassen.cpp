#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <random>

#include "../src/strassen.h"
#include "../src/simple_multiplication.h"
#include "utils.h"

TEST(StrassenTest, Correctness3x3) {
    s_fast::Matrix<int> a({{1, 6, 3}, {2, -4, 2}, {0, 8, 3}});
    s_fast::Matrix<int> b({{-3, 4, 0}, {1, -5, 4}, {2, 0, 0}});
    s_fast::Matrix<int> res({{9, -26, 24}, {-6, 28, -16}, {14, -40, 32}});

    s_fast::Matrix<int> return_val = s_fast::Strassen(a, b);

    EXPECT_TRUE(s_fast::utils::IsEqualMatrix<int>(res, return_val));
}

constexpr size_t kIters = 10;

TEST(StrassenTest, StressTest) {
    std::mt19937 random(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<int> range(500, 1000);
    size_t n = range(random);
    size_t m = range(random);
    size_t k = range(random);

    s_fast::Matrix<int> a(n, m);
    s_fast::Matrix<int> b(m, k);

    for (size_t _ = 0; _ < kIters; ++_) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                a(i, j) = random() % 2;
            }
        }
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < k; ++j) {
                b(i, j) = random() % 2;
            }
        }

        EXPECT_TRUE(s_fast::utils::IsEqualMatrix<int>(s_fast::SimpleMultiplication(a, b),
                                                      s_fast::Strassen(a, b)));
    }
}

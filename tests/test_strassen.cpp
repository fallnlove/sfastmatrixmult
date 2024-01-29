#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <random>

#include "../src/strassen.h"
#include "../src/simple_multiplication.h"

namespace {

class StrassenTest : public ::testing::Test {
protected:
    static size_t ItersCount() {
        return 10;
    }
};

}  // namespace

TEST_F(StrassenTest, Correctness3x3) {
    s_fast::Matrix<int> a({{1, 6, 3}, {2, -4, 2}, {0, 8, 3}});
    s_fast::Matrix<int> b({{-3, 4, 0}, {1, -5, 4}, {2, 0, 0}});
    s_fast::Matrix<int> res({{9, -26, 24}, {-6, 28, -16}, {14, -40, 32}});

    s_fast::Matrix<int> return_val = s_fast::Strassen(a, b);

    EXPECT_TRUE(res == return_val);
}

TEST_F(StrassenTest, StressTest) {
    using s_fast::Matrix;
    using s_fast::Random;

    size_t n = 30;
    size_t m = 30;
    size_t k = 30;

    for (size_t _ = 0; _ < ItersCount(); ++_) {
        Matrix<int> a = Random<int>(n, m, std::uniform_int_distribution<int>(0, 1));
        Matrix<int> b = Random<int>(m, k, std::uniform_int_distribution<int>(0, 1));

        EXPECT_TRUE(s_fast::SimpleMultiplication(a, b) == s_fast::Strassen(a, b));
    }
}

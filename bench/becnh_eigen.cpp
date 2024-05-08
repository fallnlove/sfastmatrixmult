#include <benchmark/benchmark.h>
#include <cstddef>
#include <random>
#include <Eigen/Dense>

#include "bench_constants.h"
#include "../src/matrix.h"

namespace {

void BenchEigen(benchmark::State& state) {
    using bench_utils::BenchmarkConstants;
    using Eigen::MatrixXd;
    using s_fast::Matrix;
    using s_fast::Random;
    using Index = s_fast::Matrix<int>::Index;

    size_t n = state.range(0);
    size_t m = state.range(1);
    size_t k = state.range(2);

    Matrix<double> aa = Random<double>(
        n, m,
        std::uniform_real_distribution<double>(BenchmarkConstants::kMinElementValue,
                                               BenchmarkConstants::kMaxElementValue));
    Matrix<double> bb = Random<double>(
        m, k,
        std::uniform_real_distribution<double>(BenchmarkConstants::kMinElementValue,
                                               BenchmarkConstants::kMaxElementValue));

    MatrixXd a(n, m);
    MatrixXd b(m, k);

    for (Index i = 0; i < n; ++i) {
        for (Index j = 0; j < m; ++j) {
            a(i, j) = aa(i, j);
        }
    }
    for (Index i = 0; i < m; ++i) {
        for (Index j = 0; j < k; ++j) {
            b(i, j) = bb(i, j);
        }
    }

    for (auto _ : state) {
        MatrixXd result = a * b;
        benchmark::DoNotOptimize(result);
    }
}

}  // namespace

BENCHMARK(BenchEigen)
    ->Iterations(bench_utils::BenchmarkConstants::kIterationCount)
    ->Unit(benchmark::kSecond)
    ->Args({bench_utils::BenchmarkConstants::kRowsLeftMatrix,
            bench_utils::BenchmarkConstants::kColumnsLeftMatrix,
            bench_utils::BenchmarkConstants::kColumnsRightMatrix});

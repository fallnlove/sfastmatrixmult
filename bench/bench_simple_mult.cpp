#include <benchmark/benchmark.h>
#include <cstddef>
#include <random>

#include "../src/simple_multiplication.h"
#include "bench_constants.h"

namespace {

void BenchSimpleMult(benchmark::State& state) {
    using bench_utils::BenchmarkConstants;
    using s_fast::Matrix;
    using s_fast::Random;
    using s_fast::SimpleMultiplication;

    size_t n = state.range(0);
    size_t m = state.range(1);
    size_t k = state.range(2);

    Matrix<double> a = Random<double>(
        n, m,
        std::uniform_real_distribution<double>(BenchmarkConstants::kMinElementValue,
                                               BenchmarkConstants::kMaxElementValue));
    Matrix<double> b = Random<double>(
        m, k,
        std::uniform_real_distribution<double>(BenchmarkConstants::kMinElementValue,
                                               BenchmarkConstants::kMaxElementValue));

    for (auto _ : state) {
        Matrix<double> result = SimpleMultiplication(a, b);
        benchmark::DoNotOptimize(result);
    }
}

void BenchSimpleMultWithTranspose(benchmark::State& state) {
    using bench_utils::BenchmarkConstants;
    using s_fast::Matrix;
    using s_fast::Random;
    using s_fast::SimpleMultiplicationWithTranspose;

    size_t n = state.range(0);
    size_t m = state.range(1);
    size_t k = state.range(2);

    Matrix<double> a = Random<double>(
        n, m,
        std::uniform_real_distribution<double>(BenchmarkConstants::kMinElementValue,
                                               BenchmarkConstants::kMaxElementValue));
    Matrix<double> b = Random<double>(
        m, k,
        std::uniform_real_distribution<double>(BenchmarkConstants::kMinElementValue,
                                               BenchmarkConstants::kMaxElementValue));

    for (auto _ : state) {
        Matrix<double> result = SimpleMultiplicationWithTranspose(a, b);
        benchmark::DoNotOptimize(result);
    }
}

}  // namespace

BENCHMARK(BenchSimpleMult)
    ->Iterations(bench_utils::BenchmarkConstants::kIterationCount)
    ->Unit(benchmark::kSecond)
    ->Args({bench_utils::BenchmarkConstants::kRowsLeftMatrix,
            bench_utils::BenchmarkConstants::kColumnsLeftMatrix,
            bench_utils::BenchmarkConstants::kColumnsRightMatrix});

BENCHMARK(BenchSimpleMultWithTranspose)
    ->Iterations(bench_utils::BenchmarkConstants::kIterationCount)
    ->Unit(benchmark::kSecond)
    ->Args({bench_utils::BenchmarkConstants::kRowsLeftMatrix,
            bench_utils::BenchmarkConstants::kColumnsLeftMatrix,
            bench_utils::BenchmarkConstants::kColumnsRightMatrix});

BENCHMARK_MAIN();

#include <benchmark/benchmark.h>
#include <cstddef>
#include <random>

#include "../src/simple_multiplication.h"

static void BenchSimpleMult(benchmark::State& state) {
    std::random_device random_device;
    std::mt19937 random_gen(random_device());

    size_t n = state.range(0);
    size_t m = state.range(1);
    size_t k = state.range(2);

    s_fast::Matrix<double> a(n, m);
    s_fast::Matrix<double> b(m, k);

    std::uniform_real_distribution<double> range_double(-1000., 1000.);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            a(i, j) = range_double(random_gen);
        }
    }

    for (auto _ : state) {
        s_fast::SimpleMultiplication(a, b);
    }
}

static void BenchSimpleMultWithTranspose(benchmark::State& state) {
    std::random_device random_device;
    std::mt19937 random_gen(random_device());

    size_t n = state.range(0);
    size_t m = state.range(1);
    size_t k = state.range(2);

    s_fast::Matrix<double> a(n, m);
    s_fast::Matrix<double> b(m, k);

    std::uniform_real_distribution<double> range_double(-1000., 1000.);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            a(i, j) = range_double(random_gen);
        }
    }

    for (auto _ : state) {
        s_fast::SimpleMultiplicationWithTranspose(a, b);
    }
}

// BENCHMARK(BmSomeFunction)->Iterations(10);

BENCHMARK(BenchSimpleMult)->Iterations(10)->Unit(benchmark::kSecond)->Args({1000, 500, 1000});

BENCHMARK(BenchSimpleMultWithTranspose)
    ->Iterations(10)
    ->Unit(benchmark::kSecond)
    ->Args({1000, 500, 1000});

BENCHMARK_MAIN();

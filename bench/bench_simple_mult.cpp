#include <benchmark/benchmark.h>
#include <cstddef>
#include <random>

#include "../src/simple_multiplication.h"

static void BenchSimpleMult(benchmark::State& state) {
    state.PauseTiming();

    std::random_device random_device;
    std::mt19937 random_gen(random_device());
    std::uniform_int_distribution<size_t> range(0, 1000);

    size_t n = range(random_gen);
    size_t m = range(random_gen);
    size_t k = range(random_gen);

    s_fast::Matrix<double> a(n, m);
    s_fast::Matrix<double> b(m, k);

    std::uniform_real_distribution<double> range_double(-1000., 1000.);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            a(i, j) = range_double(random_gen);
        }
    }

    state.ResumeTiming();
    s_fast::SimpleMultiplication(a, b);
}

// BENCHMARK(BmSomeFunction)->Iterations(10);

BENCHMARK(BenchSimpleMult)->Iterations(10)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();

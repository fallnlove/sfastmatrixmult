add_executable(
  bench_mult
  bench/bench_simple_mult.cpp
  bench/bench_simd.cpp
  bench/bench_strassen.cpp
  bench/bench_cache_oblivious_mult.cpp
)

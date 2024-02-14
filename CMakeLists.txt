cmake_minimum_required(VERSION 3.14)
project(s_fast)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG 344117638c8ff7e239044fd0fa7085839fc03021
)
FetchContent_Declare(
  xsimd
  GIT_REPOSITORY https://github.com/xtensor-stack/xsimd.git
  GIT_TAG c1247bffa8fc36de7380a5cd42673a3b32f74c97
)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(benchmark)
FetchContent_MakeAvailable(xsimd)

include(tests/sources.cmake)

include(bench/sources.cmake)

enable_testing()

target_link_libraries(
  bench_mult
  xsimd
  benchmark::benchmark
)

set(COMPILE_RELEASE_OPTIONS "-O3;-DNDEBUG")

target_compile_options(
  bench_mult
  PUBLIC
  $<$<CONFIG:Release>:${COMPILE_RELEASE_OPTIONS}>
  $<$<CONFIG:Debug>:${COMPILE_RELEASE_OPTIONS}>
)

target_link_libraries(
  test_mult
  xsimd
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_mult)

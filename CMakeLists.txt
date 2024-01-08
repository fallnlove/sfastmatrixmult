cmake_minimum_required(VERSION 3.14)
project(s_fast)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

find_package(benchmark REQUIRED)

include(tests/sources.cmake)

include(bench/sources.cmake)

enable_testing()

target_link_libraries(
  bench_mult
  benchmark::benchmark
)

target_link_libraries(
  test_mult
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_mult)

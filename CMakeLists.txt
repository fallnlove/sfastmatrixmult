cmake_minimum_required(VERSION 3.14)
project(s_fast
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Matrix lib for C++")

add_library(s_fast INTERFACE)
add_library(s_fast::s_fast ALIAS s_fast)

set_target_properties(s_fast PROPERTIES LINKER_LANGUAGE CXX)

set(public_headers
    include/s_fast/s_fast.h)
include(CMakePackageConfigHelpers)

target_include_directories(s_fast
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties(s_fast PROPERTIES
    PUBLIC_HEADER "${public_headers}"
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

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
  GIT_TAG 13.0.0
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

target_compile_options(
  bench_mult
  PUBLIC
)

target_link_libraries(
  test_mult
  xsimd
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_mult)

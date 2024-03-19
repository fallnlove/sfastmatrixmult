#pragma once

#include <cstddef>
#include <cstdint>

namespace bench_utils {

struct BenchmarkConstants {
    static constexpr int64_t kRowsLeftMatrix = 1000;
    static constexpr int64_t kColumnsLeftMatrix = 1000;
    static constexpr int64_t kColumnsRightMatrix = 1000;

    static constexpr double kMinElementValue = -1000.;
    static constexpr double kMaxElementValue = 1000.;

    static constexpr size_t kIterationCount = 10;
};

}  // namespace bench_utils

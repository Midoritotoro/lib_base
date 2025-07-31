#include <src/core/string/crt/BaseAnyStrstr.h>
#include <benchmarks/tools/BenchmarkHelper.h>

#include <uchar.h>
#include <wchar.h>


template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class CRTStrstrBenchmark {
public:
    static void Strstr(benchmark::State& state) noexcept {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};
        static constexpr auto needle = FixedReversedArray<_Char_, state.range(0)>{};

        while (state.KeepRunning()) {
            if constexpr (std::is_same_v<_Char_, char>)
                benchmark::DoNotOptimize(strstr(textArray.data, FixedReversedArray<_Char_, needle.data));
            else if constexpr (std::is_same_v<_Char_, wchar_t>)
                benchmark::DoNotOptimize(wcsstr(textArray.data, FixedReversedArray<_Char_, needle.data));
        }
    }
};

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class StrstrBenchmark {
public:
    static void Strstr(benchmark::State& state) noexcept {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};
        static constexpr auto needle = FixedReversedArray<_Char_, state.range(0)>{}

        while (state.KeepRunning())
            benchmark::DoNotOptimize(base::string::__base_any_strstr(textArray.data, needle.data));
    }
};

BASE_ADD_STRING_BENCHMARKS_FOR_EACH_SIZE_ARGS(StrstrBenchmark, CRTStrstrBenchmark, char, Strstr, 14);
//BASE_ADD_STRING_BENCHMARKS_FOR_EACH_SIZE(StrstrBenchmark, CRTStrstrBenchmark, wchar_t, Strstr);

BASE_BENCHMARK_MAIN();
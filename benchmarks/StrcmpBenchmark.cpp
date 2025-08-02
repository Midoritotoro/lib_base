#include <src/core/string/crt/cs/BaseStrcmp.h>
#include <src/core/string/crt/BaseAnyStrcmp.h>

#include <uchar.h>
#include <wchar.h>

#include <benchmarks/tools/BenchmarkHelper.h>

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class CRTStrcmpBenchmark {
public:
    static void Strcmp(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};
        static constexpr auto textReversedArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        while (state.KeepRunning()) {
            if constexpr (std::is_same_v<_Char_, char>)
                benchmark::DoNotOptimize(std::find(textArray.data, textArray.data + 4096,  textReversedArray.data, textReversedArray.data + 16));
            else if constexpr (std::is_same_v<_Char_, wchar_t>)
                benchmark::DoNotOptimize(wcscmp(textArray.data, textReversedArray.data));
        }
    }
};

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class StrcmpBenchmark {
public:
    static auto Strcmp(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};
        static constexpr auto textReversedArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        while (state.KeepRunning())
            benchmark::DoNotOptimize(base::string::__base_any_strcmp(textArray.data, textReversedArray.data));
    }
};


BASE_ADD_STRING_BENCHMARKS_FOR_EACH_SIZE(StrcmpBenchmark, CRTStrcmpBenchmark, char, Strcmp);


BASE_BENCHMARK_MAIN();
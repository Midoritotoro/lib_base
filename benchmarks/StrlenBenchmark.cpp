#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/BaseAnyStrlen.h>

#include <benchmarks/tools/BenchmarkHelper.h>

#include <uchar.h>
#include <wchar.h>


template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark>
class CRTStrlenBenchmark {
public:
    static void Strlen(benchmark::State& state) noexcept {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        while (state.KeepRunning()) {
            if constexpr (std::is_same_v<_Char_, char>)
                benchmark::DoNotOptimize(strlen(textArray.data));
            else if constexpr (std::is_same_v<_Char_, wchar_t>)
                benchmark::DoNotOptimize(wcslen(textArray.data));
        }
    }
};

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark>
class StrlenBenchmark {
public:
    static void Strlen(benchmark::State& state) noexcept {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        while (state.KeepRunning())
            benchmark::DoNotOptimize(base::string::__base_any_strlen(textArray.data));
    }
};

// ========================================================================================


//
//BASE_ADD_BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Small>::Strlen);
//BASE_ADD_BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Medium>::Strlen);
//BASE_ADD_BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Large>::Strlen);
//
//// ========================================================================================
//
//BASE_ADD_BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Small>::Strlen);
//BASE_ADD_BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Medium>::Strlen);
//BASE_ADD_BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Large>::Strlen);
//
//// ========================================================================================
//// ========================================================================================
//// ========================================================================================
//
//BASE_ADD_BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Small>::Strlen);
//BASE_ADD_BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Medium>::Strlen);
//BASE_ADD_BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Large>::Strlen);
//
//// ========================================================================================
//
//BASE_ADD_BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Small>::Strlen);
//BASE_ADD_BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Medium>::Strlen);
//BASE_ADD_BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Large>::Strlen);
//
//// ========================================================================================
//// ========================================================================================
//// ========================================================================================
//
//BASE_ADD_BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Small>::Strlen);
//BASE_ADD_BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Medium>::Strlen);
//BASE_ADD_BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Large>::Strlen);

BASE_ADD_STRING_BENCHMARKS_FOR_EACH_SIZE(StrlenBenchmark, CRTStrlenBenchmark, char, Strlen);

BASE_BENCHMARK_MAIN();
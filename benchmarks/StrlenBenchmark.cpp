#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/BaseAnyStrlen.h>

#include <benchmarks/tools/BenchmarkHelper.h>

#include <uchar.h>
#include <wchar.h>


template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class CRTStrlenBenchmark {
public:
    static void Strlen(benchmark::State& state) noexcept {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        while (state.KeepRunning()) {
            if constexpr (std::is_same_v<_Char_, char>)
                strlen(textArray.data);
            else if constexpr (std::is_same_v<_Char_, wchar_t>)
                wcslen(textArray.data);
        }
    }
};

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class StrlenBenchmark {
public:
    static void Strlen(benchmark::State& state) noexcept {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        while (state.KeepRunning())
            base::string::__base_any_strlen(textArray.data);
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

BASE_ADD_BENCHMARK(
    BASE_ECHO(StrlenBenchmark<char>::Strlen),
    BASE_ECHO(CRTStrlenBenchmark<char>::Strlen)
);

BASE_BENCHMARK_MAIN();
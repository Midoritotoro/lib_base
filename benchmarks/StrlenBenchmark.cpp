#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/BaseAnyStrlen.h>

#include <benchmarks/tools/BenchmarkHelper.h>

#include <uchar.h>
#include <wchar.h>


template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class CRTStrlenBenchmark {
    static auto StrlenHelper(const _Char_* const string)
    {
        if constexpr (std::is_same_v<_Char_, char>)
            return strlen(string);
        else if constexpr (std::is_same_v<_Char_, wchar_t>)
            return wcslen(string);
    }

public:
    static auto Strlen(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                StrlenHelper(textArray.data)
            );
        }
    }
};

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class StrlenBenchmark {
    static auto StrlenHelper(const _Char_* const string)
    {
        return base::string::__base_any_strlen(string);
    }

public:
    static auto Strlen(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                StrlenHelper(textArray.data)
            );
        }
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


int main(int argc, char** argv) {
    ::benchmark::MaybeReenterWithoutASLR(argc, argv);
    char arg0_default[] = "benchmark";

    char* args_default = reinterpret_cast<char*>(arg0_default);

    if (!argv) {
        argc = 1;
        argv = &args_default;
    }

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;

    BenchmarksCompareReporter reporter;
    
    ::benchmark::RunSpecifiedBenchmarks(&reporter);
    ::benchmark::Shutdown();

    return 0;
}
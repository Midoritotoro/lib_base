#include <src/core/string/crt/cs/BaseStrcmp.h>
#include <src/core/string/crt/BaseAnyStrcmp.h>

#include <uchar.h>
#include <wchar.h>

#include <benchmarks/tools/BenchmarkHelper.h>

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class CRTStrlenBenchmark {
    static auto StrcmpHelper(
        const _Char_* const string1,
        const _Char_* const string2)
    {
        if constexpr (std::is_same_v<_Char_, char>)
            return strcmp(string1, string2);
        else if constexpr (std::is_same_v<_Char_, wchar_t>)
            return wcscmp(string1, string2);
    }

public:
    static auto Strcmp(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};
        static constexpr auto textReversedArray = FixedReversedArray<_Char_, stringAlignedSizeForBenchmark>{};

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                StrcmpHelper(textArray.data, textReversedArray.data)
            );
        }
    }
};

template <
    typename _Char_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class StrlenBenchmark {
    static auto StrcmpHelper(
        const _Char_* const string1,
        const _Char_* const string2)
    {
        return base::string::__base_any_strcmp(string1, string2);
    }

public:
    static auto Strcmp(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_Char_, stringAlignedSizeForBenchmark>{};
        static constexpr auto textReversedArray = FixedReversedArray<_Char_, stringAlignedSizeForBenchmark>{};

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                StrcmpHelper(textArray.data, textReversedArray.data)
            );
        }
    }
};

// ========================================================================================

BASE_ADD_BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Small>::Strcmp);
BASE_ADD_BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Medium>::Strcmp);
BASE_ADD_BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Large>::Strcmp);

// ========================================================================================

BASE_ADD_BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Small>::Strcmp);
BASE_ADD_BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Medium>::Strcmp));
BASE_ADD_BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Large>::Strcmp);


// ========================================================================================
// ========================================================================================
// ========================================================================================


BASE_ADD_BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Small>::Strcmp);
BASE_ADD_BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Medium>::Strcmp);
BASE_ADD_BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Large>::Strcmp);

// ========================================================================================

BASE_ADD_BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Small>::Strcmp);
BASE_ADD_BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Medium>::Strcmp);
BASE_ADD_BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Large>::Strcmp);


// ========================================================================================
// ========================================================================================
// ========================================================================================


BASE_ADD_BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Small>::Strcmp);
BASE_ADD_BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Medium>::Strcmp);
BASE_ADD_BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Large>::Strcmp);

// ========================================================================================


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

    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    return 0;
}
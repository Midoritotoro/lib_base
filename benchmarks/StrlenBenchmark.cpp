#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/BaseAnyStrlen.h>

#include <base/core/arch/Platform.h>
#include <benchmark/benchmark.h>

#include <uchar.h>
#include <wchar.h>

#include <iostream>
#include <vector>
#include <iomanip> // For std::setprecision

// #define BASE_BENCHMARK_IN_MILLISECONDS 1

#if defined(BASE_BENCHMARK_IN_MILLISECONDS)
#  define BASE_BENCHMARK_UNIT_OF_MEASUREMENT benchmark::kMillisecond
#else 
#  define BASE_BENCHMARK_UNIT_OF_MEASUREMENT benchmark::kNanosecond
#endif

using namespace base;

enum StringAlignedSizeForBenchmark {
    Large = 4096,
    Medium = 1024,
    Small = 512
};

#define BASE_FIXED_CHAR_ARRAY(name, prefix) \
    template <typename _Type_, size_t N>\
    struct name {\
        _Type_ data[N + 1]{};\
    \
        constexpr name() {\
            for (size_t i = 0; i < N; ++i) {\
                data[i] = prefix'A' + (i % 26);\
            }\
            data[N] = prefix'\0';\
        }\
    };

#define BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(name, type, prefix) \
    template <size_t N>\
    struct name<type, N> {\
        type data[N + 1]{};\
    \
        constexpr name() {\
            for (size_t i = 0; i < N; ++i) {\
                data[i] = prefix'A' + (i % 26);\
            }\
            data[N] = prefix'\0';\
        }\
    };

BASE_FIXED_CHAR_ARRAY(FixedArray, );
#if __cpp_lib_char8_t
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, char8_t, u8);
#endif
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, char16_t, u);
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, char32_t, U);
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, wchar_t, L);

static struct Loc {
    Loc() {
        std::setlocale(LC_ALL, "en_US.UTF-8");
    }
} localeSet;

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



BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Small>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Medium>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(StrlenBenchmark<char, StringAlignedSizeForBenchmark::Large>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

// ========================================================================================

BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Small>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Medium>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(CRTStrlenBenchmark<char, StringAlignedSizeForBenchmark::Large>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);


// ========================================================================================
// ========================================================================================
// ========================================================================================


BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Small>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Medium>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(StrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Large>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

// ========================================================================================

BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Small>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Medium>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Large>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);


// ========================================================================================
// ========================================================================================
// ========================================================================================


BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Small>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Medium>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(StrlenBenchmark<char32_t, StringAlignedSizeForBenchmark::Large>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

// ========================================================================================

BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Small>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Medium>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

BENCHMARK(CRTStrlenBenchmark<wchar_t, StringAlignedSizeForBenchmark::Large>::Strlen)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

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
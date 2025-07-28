#pragma once 

#include <base/core/arch/Platform.h>
#include <benchmark/benchmark.h>

#include <iostream>
#include <map>

#include <sstream>
#include <array>

#if !defined(BASE_BENCHMARK_REPITITIONS)
#  define BASE_BENCHMARK_REPITITIONS 10000
#endif // BASE_BENCHMARK_REPITITIONS

#if !defined(BASE_BENCHMARK_ITERATIONS)
#  define BASE_BENCHMARK_ITERATIONS 10000
#endif // BASE_BENCHMARK_ITERATIONS

#if defined(BASE_BENCHMARK_IN_MILLISECONDS)
#  define BASE_BENCHMARK_UNIT_OF_MEASUREMENT benchmark::kMillisecond
#else 
#  define BASE_BENCHMARK_UNIT_OF_MEASUREMENT benchmark::kNanosecond
#endif // BASE_BENCHMARK_UNIT_OF_MEASUREMENT

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

#define BASE_FIXED_REVERSED_CHAR_ARRAY(name, prefix) \
    template <typename _Type_, size_t N>\
    struct name {\
        _Type_ data[N + 1]{};\
    \
        constexpr name() {\
            for (size_t i = 0; i < N; ++i) {\
                data[i] = prefix'Z' - (i % 26);\
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

#define BASE_ADD_SPECIALIZATION_TO_FIXED_REVERSED_CHAR_ARRAY(name, type, prefix) \
    template <size_t N>\
    struct name<type, N> {\
        type data[N + 1]{};\
    \
        constexpr name() {\
            for (size_t i = 0; i < N; ++i) {\
                data[i] = prefix'Z' - (i % 26);\
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



BASE_FIXED_REVERSED_CHAR_ARRAY(FixedReversedArray, );
#if __cpp_lib_char8_t
BASE_ADD_SPECIALIZATION_TO_FIXED_REVERSED_CHAR_ARRAY(FixedReversedArray, char8_t, u8);
#endif
BASE_ADD_SPECIALIZATION_TO_FIXED_REVERSED_CHAR_ARRAY(FixedReversedArray, char16_t, u);
BASE_ADD_SPECIALIZATION_TO_FIXED_REVERSED_CHAR_ARRAY(FixedReversedArray, char32_t, U);
BASE_ADD_SPECIALIZATION_TO_FIXED_REVERSED_CHAR_ARRAY(FixedReversedArray, wchar_t, L);

#if !defined(BASE_ADD_BENCHMARK)
#  define BASE_ADD_BENCHMARK(...) BENCHMARK(__VA_ARGS__)->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)->Iterations(BASE_BENCHMARK_ITERATIONS)->Repetitions(BASE_BENCHMARK_REPITITIONS)->ReportAggregatesOnly(true)->DisplayAggregatesOnly(true)                                       
#endif // BASE_ADD_BENCHMARK

static struct Loc {
    Loc() {
        std::setlocale(LC_ALL, "en_US.UTF-8");
    }
} localeSet;



inline std::vector<benchmark::BenchmarkReporter::Run> _benchmarks;

class BenchmarksCompareReporter : public benchmark::ConsoleReporter
{
public:
    BenchmarksCompareReporter() {}

    bool ReportContext(const Context& context) override {
        return ConsoleReporter::ReportContext(context);
    }

    void ReportRuns(const std::vector<Run>& reports) override {
        std::ostream& Out = GetOutputStream();

        for (const auto& run : reports) {
            _benchmarks.push_back(run);

            if (_benchmarks.size() % 2 == 0) {
                printf("Real time difference: %f", _benchmarks[0].GetAdjustedRealTime() / _benchmarks[1].GetAdjustedRealTime());
                _benchmarks.resize(0);
            }
        }

        return ConsoleReporter::ReportRuns(reports);
    }

private:
  /*  void PrintRunData(const Run& run) {
        _benchmarks.push_back(run);
        
        if (_benchmarks.size() % 2 == 0) {
            printf("Real time difference: %f", _benchmarks[0].GetAdjustedRealTime() / _benchmarks[1].GetAdjustedRealTime());
            printf("Cpu time difference: %f", _benchmarks[0].GetAdjustedCPUTime() / _benchmarks[1].GetAdjustedCPUTime());

            _benchmarks.resize(0);
        }
    }*/
};
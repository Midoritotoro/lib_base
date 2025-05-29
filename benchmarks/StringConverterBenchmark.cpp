#include <base/core/string/StringConverter.h>
#include <base/core/arch/Platform.h>
#include <benchmark/benchmark.h>

#include <cuchar>
#include <cwchar>

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


template <
    typename _FromChar_,
    typename _ToChar_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class CRTStringConverterBenchmark {
    static auto ConvertStringHelper(
        const _FromChar_* input,
        size_t length, 
        _ToChar_* buffer) 
    {
        if constexpr (std::is_same_v<_FromChar_, char> && std::is_same_v<_ToChar_, wchar_t>)
            return mbstowcs(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, wchar_t> && std::is_same_v<_ToChar_, char>)
            return wcstombs(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, char> && std::is_same_v<_ToChar_, char16_t>)
            return mbrtoc16(buffer, input, length, nullptr);
        else if constexpr (std::is_same_v<_FromChar_, char16_t> && std::is_same_v<_ToChar_, char>) {
            mbstate_t state{};

            for (size_t i = 0; i < length; ++i)
                c16rtomb(buffer, input[i], length, &state);
        }
    }

public:
    static auto ConvertString(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_FromChar_, stringAlignedSizeForBenchmark>{};
        _ToChar_* ch = static_cast<_ToChar_*>(memory::AllocateAligned(stringAlignedSizeForBenchmark * sizeof(_ToChar_), 64));

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                ConvertStringHelper(textArray.data, stringAlignedSizeForBenchmark, ch)
            );
        }

       // memory::Free(ch);
    }
};

template <
    typename _FromChar_,
    typename _ToChar_,
    StringAlignedSizeForBenchmark stringAlignedSizeForBenchmark = StringAlignedSizeForBenchmark::Large>
class StringConverterBenchmark {
    static auto ConvertStringHelper(
        const _FromChar_* input, 
        size_t length, 
        string::StringConversionResult<_ToChar_>& result)
    {
        string::StringConverter<>::convertStringStore<_FromChar_, _ToChar_>(input, length, &result);
        return result;
    }

public:
    static void ConvertString(benchmark::State& state) {
        
        string::StringConversionResult<_ToChar_> result;
        static constexpr auto textArray = FixedArray<_FromChar_, stringAlignedSizeForBenchmark>{};

        result.setData(static_cast<_ToChar_*>(memory::AllocateAligned(stringAlignedSizeForBenchmark * sizeof(_ToChar_), 64)));

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                ConvertStringHelper(textArray.data, stringAlignedSizeForBenchmark, result)
            );
        }

        memory::FreeNullAligned(result.data());
    }
};

//BENCHMARK(StringConverterBenchmark<char, wchar_t>::ConvertString)
//    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
//    ->Repetitions(1000)
//    ->ReportAggregatesOnly(true)
//    ->DisplayAggregatesOnly(true);
//
//BENCHMARK(CRTStringConverterBenchmark<char, wchar_t>::ConvertString)
//    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
//    ->Repetitions(1000)
//    ->ReportAggregatesOnly(true)
//    ->DisplayAggregatesOnly(true);

// ========================================================================================

//BENCHMARK(StringConverterBenchmark<wchar_t, char>::ConvertString)
//    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
//    ->Repetitions(1000)
//    ->ReportAggregatesOnly(true)
//    ->DisplayAggregatesOnly(true);
//
//BENCHMARK(CRTStringConverterBenchmark<wchar_t, char>::ConvertString)
//    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
//    ->Repetitions(1000)
//    ->ReportAggregatesOnly(true)
//    ->DisplayAggregatesOnly(true);

// ========================================================================================

//BENCHMARK(StringConverterBenchmark<char8_t, char>::ConvertString)
//    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
//    ->Repetitions(1000)
//    ->ReportAggregatesOnly(true)
//    ->DisplayAggregatesOnly(true);


// ========================================================================================

BENCHMARK(StringConverterBenchmark<char, char8_t>::ConvertString)
    ->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)
    ->Repetitions(1000)
    ->Iterations(1000)
    ->ReportAggregatesOnly(true)
    ->DisplayAggregatesOnly(true);

// ========================================================================================
// ========================================================================================
// ========================================================================================
// ========================================================================================
// ========================================================================================
// ========================================================================================
// ========================================================================================
// ========================================================================================

int main(int argc, char** argv) {
    benchmark::MaybeReenterWithoutASLR(argc, argv);
    char arg0_default[] = "benchmark"; 

    char* args_default = reinterpret_cast<char*>(arg0_default); 

    if (!argv) {
        argc = 1; argv = &args_default;
    } 

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;
    
    ::benchmark::RunSpecifiedBenchmarks(); 
    ::benchmark::Shutdown();
    
    return 0;
}
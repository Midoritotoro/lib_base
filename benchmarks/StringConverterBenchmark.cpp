#include <base/core/string/StringConverter.h>

#include <benchmarks/tools/BenchmarkHelper.h>

#include <uchar.h>
#include <wchar.h>


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
        else if constexpr (std::is_same_v<_FromChar_, char> && std::is_same_v<_ToChar_, char16_t>) {
            mbstate_t state{};

            for (size_t i = 0; i < length; ++i)
                mbrtoc16(&buffer[i], &input[i], 1, &state);
            return 0;
        }
        else if constexpr (std::is_same_v<_FromChar_, char> && std::is_same_v<_ToChar_, char8_t>)
            return memcpy(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, char8_t> && std::is_same_v<_ToChar_, char>)
            return memcpy(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, char16_t> && std::is_same_v<_ToChar_, char>) {
            mbstate_t state{};

            for (size_t i = 0; i < length; ++i)
                c16rtomb(&buffer[i], input[i], &state);
            return 0;
        }
    }

public:
    static auto ConvertString(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_FromChar_, stringAlignedSizeForBenchmark>{};
        _ToChar_* ch = static_cast<_ToChar_*>(base::memory::AllocateAligned(stringAlignedSizeForBenchmark * sizeof(_ToChar_), 64));

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                ConvertStringHelper(textArray.data, stringAlignedSizeForBenchmark, ch)
            );
        }

        base::memory::FreeNullAligned(ch);
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
        base::string::StringConversionResult<_ToChar_>& result)
    {
        base::string::StringConverter::convertStringStore<_FromChar_, _ToChar_>(input, length, &result);
        return result;
    }

public:
    static void ConvertString(benchmark::State& state) {
        static constexpr auto textArray = FixedArray<_FromChar_, stringAlignedSizeForBenchmark>{};

        base::string::StringConversionResult<_ToChar_> result;
        result.setData(static_cast<_ToChar_*>(base::memory::AllocateAligned(stringAlignedSizeForBenchmark * sizeof(_ToChar_), 64)));

        for (auto _ : state) {
            benchmark::DoNotOptimize(
                ConvertStringHelper(textArray.data, stringAlignedSizeForBenchmark, result)
            );
        }


        base::memory::FreeNullAligned(result.data());
    }
};

// ========================================================================================


//BASE_ADD_BENCHMARK(StringConverterBenchmark<char, wchar_t>::ConvertString);

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
#pragma once 

#include <base/core/arch/Platform.h>
#include <benchmark/benchmark.h>

#include <iostream>
#include <map>

#include <sstream>
#include <array>

#include <iomanip>

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
    Tiny        = 16,       
    VerySmall   = 32,   
    Small       = 64,    
    MediumSmall = 128,
    Medium      = 256,
    MediumLarge = 512,
    Large       = 1024,
    VeryLarge   = 2048,
    Huge        = 4096,
    ExtraHuge   = 8192,
    MegaHuge    = 16384,
    GigaHuge    = 32768,
    TeraHuge    = 65536  
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
#  define BASE_ADD_BENCHMARK(benchFirst, benchSecond)                   \
     BENCHMARK(benchFirst)->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)    \
        ->Iterations(BASE_BENCHMARK_ITERATIONS)                         \
        ->Repetitions(BASE_BENCHMARK_REPITITIONS)                       \
        ->ReportAggregatesOnly(true)                                    \
        ->DisplayAggregatesOnly(true);                                  \
    BENCHMARK(benchSecond)->Unit(BASE_BENCHMARK_UNIT_OF_MEASUREMENT)    \
        ->Iterations(BASE_BENCHMARK_ITERATIONS)                         \
        ->Repetitions(BASE_BENCHMARK_REPITITIONS)                       \
        ->ReportAggregatesOnly(true)                                    \
        ->DisplayAggregatesOnly(true);                                   
#endif // BASE_ADD_BENCHMARK


#if !defined(BASE_ADD_STRING_BENCHMARKS_FOR_EACH_SIZE)
#  define BASE_ADD_STRING_BENCHMARKS_FOR_EACH_SIZE(nameFirst, nameSecond, charType, funcN)              \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::Tiny>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::Tiny>::funcN));                   \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::VerySmall>::funcN),BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::VerySmall>::funcN));              \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::Small>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::Small>::funcN));                  \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::MediumSmall>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::MediumSmall>::funcN));            \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::Medium>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::Medium>::funcN));                 \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::MediumLarge>::funcN),BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::MediumLarge>::funcN));            \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::Large>::funcN),BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::Large>::funcN));                  \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::VeryLarge>::funcN),BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::VeryLarge>::funcN));              \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::Huge>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::Huge>::funcN));                   \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::ExtraHuge>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::ExtraHuge>::funcN));              \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::MegaHuge>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::MegaHuge>::funcN));               \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::GigaHuge>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::GigaHuge>::funcN));               \
    BASE_ADD_BENCHMARK(BASE_ECHO(nameFirst<charType, StringAlignedSizeForBenchmark::GigaHuge>::funcN), BASE_ECHO(nameSecond<charType, StringAlignedSizeForBenchmark::GigaHuge>::funcN));
#endif // BASE_ADD_STRING_BENCHMARKS_FOR_EACH_SIZE

#if !defined(BASE_BENCHMARK_MAIN)
#define BASE_BENCHMARK_MAIN()                                                   \
    int main(int argc, char** argv) {                                           \
        benchmark::MaybeReenterWithoutASLR(argc, argv);                         \
        char arg0_default[] = "benchmark";                                      \
        char* args_default = reinterpret_cast<char*>(arg0_default);             \
        if (!argv) {                                                            \
            argc = 1;                                                           \
            argv = &args_default;                                               \
        }                                                                       \
        ::benchmark::Initialize(&argc, argv);                                   \
        if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;     \
        BenchmarksCompareReporter reporter;                                     \
        ::benchmark::RunSpecifiedBenchmarks(&reporter);                         \
        ::benchmark::Shutdown();                                                \
        return 0;                                                               \
    }                                                                           \
    int main(int, char**)
#endif // BASE_BENCHMARK_MAIN


static struct Loc {
    Loc() {
        std::setlocale(LC_ALL, "en_US.UTF-8");
    }
} localeSet;


enum LogColor {
    COLOR_DEFAULT,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE
};

#ifdef OS_WIN
typedef WORD PlatformColorCode;
#else
typedef const char* PlatformColorCode;
#endif

PlatformColorCode GetPlatformColorCode(LogColor color) {
#ifdef OS_WIN
    switch (color) {
    case COLOR_RED:
        return FOREGROUND_RED;
    case COLOR_GREEN:
        return FOREGROUND_GREEN;
    case COLOR_YELLOW:
        return FOREGROUND_RED | FOREGROUND_GREEN;
    case COLOR_BLUE:
        return FOREGROUND_BLUE;
    case COLOR_MAGENTA:
        return FOREGROUND_BLUE | FOREGROUND_RED;
    case COLOR_CYAN:
        return FOREGROUND_BLUE | FOREGROUND_GREEN;
    case COLOR_WHITE:  // fall through to default
    default:
        return 0;
    }
#else
    switch (color) {
    case COLOR_RED:
        return "1";
    case COLOR_GREEN:
        return "2";
    case COLOR_YELLOW:
        return "3";
    case COLOR_BLUE:
        return "4";
    case COLOR_MAGENTA:
        return "5";
    case COLOR_CYAN:
        return "6";
    case COLOR_WHITE:
        return "7";
    default:
        return nullptr;
    };
#endif
}

std::string FormatString(const char* msg, va_list args) {
    // we might need a second shot at this, so pre-emptivly make a copy
    va_list args_cp;
    va_copy(args_cp, args);

    std::size_t size = 256;
    char local_buff[256];
    auto ret = vsnprintf(local_buff, size, msg, args_cp);

    va_end(args_cp);

    // currently there is no error handling for failure, so this is hack.
    Assert(ret >= 0);

    if (ret == 0) {  // handle empty expansion
        return {};
    }
    if (static_cast<size_t>(ret) < size) {
        return local_buff;
    }
    // we did not provide a long enough buffer on our first attempt.
    size = static_cast<size_t>(ret) + 1;  // + 1 for the null byte
    std::unique_ptr<char[]> buff(new char[size]);
    ret = vsnprintf(buff.get(), size, msg, args);
    Assert(ret > 0 && (static_cast<size_t>(ret)) < size);
    return buff.get();
}

std::string FormatString(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    auto tmp = FormatString(msg, args);
    va_end(args);
    return tmp;
}


void ColorPrintf(
    std::ostream& out,
    LogColor color,
    const char* fmt,
    ...) 
{
    va_list args;
    va_start(args, fmt);

#ifdef OS_WIN
    const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Gets the current text color.
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
    const WORD original_color_attrs = buffer_info.wAttributes;

    const WORD original_background_attrs =
        original_color_attrs & (BACKGROUND_RED | BACKGROUND_GREEN |
            BACKGROUND_BLUE | BACKGROUND_INTENSITY);

    SetConsoleTextAttribute(stdout_handle, GetPlatformColorCode(color) |
        FOREGROUND_INTENSITY |
        original_background_attrs);
    out << FormatString(fmt, args);

    out << std::flush;
    // Restores the text and background color.
    SetConsoleTextAttribute(stdout_handle, original_color_attrs);
#else
    const char* color_code = GetPlatformColorCode(color);
    if (color_code != nullptr) {
        out << FormatString("\033[0;3%sm", color_code);
    }
    out << FormatString(fmt, args) << "\033[m";
#endif
    va_end(args);
}


inline std::vector<benchmark::BenchmarkReporter::Run> _benchmarks;

class BenchmarksCompareReporter : public benchmark::ConsoleReporter
{
public:
    BenchmarksCompareReporter() {}

    bool ReportContext(const Context& context) override {
        return ConsoleReporter::ReportContext(context);
    }

    void ReportRuns(const std::vector<Run>& reports) override {
        std::ostream& out = GetOutputStream();
        ConsoleReporter::ReportRuns(reports);

        for (const auto& run : reports) {
            _benchmarks.push_back(run);

            if (_benchmarks.size() % 8 == 0) {
                const auto realTimeDifference = (_benchmarks[4].GetAdjustedRealTime() / _benchmarks[0].GetAdjustedRealTime());

                out << "Benchmark 1: " << _benchmarks[0].benchmark_name() << std::endl;
                out << "Benchmark 2: " << _benchmarks[4].benchmark_name() << std::endl;

                if (realTimeDifference > 1.0f)
                    ColorPrintf(out, COLOR_BLUE, "Benchmark 1 faster than Benchmark 2 by a %f%s", realTimeDifference * 100, "%");

                else if (realTimeDifference < 1.0f)
                    ColorPrintf(out, COLOR_RED, "Benchmark 1 slower than Benchmark 2 by a %f%s", realTimeDifference * 100, "%");

                else
                    ColorPrintf(out, COLOR_WHITE, "Benchmark 1 and Benchmark 2 are equal");

                _benchmarks.resize(0);
            }
        }
    }
};
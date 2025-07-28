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

void ColorPrintf(std::ostream& out, LogColor color, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ColorPrintf(out, color, fmt, args);
    va_end(args);
}

void ColorPrintf(std::ostream& out, LogColor color, const char* fmt,
    va_list args) {
#ifdef OS_WIN
    ((void)out);  // suppress unused warning

    const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Gets the current text color.
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
    const WORD original_color_attrs = buffer_info.wAttributes;

    // We need to flush the stream buffers into the console before each
    // SetConsoleTextAttribute call lest it affect the text that is already
    // printed but has not yet reached the console.
    out.flush();

    const WORD original_background_attrs =
        original_color_attrs & (BACKGROUND_RED | BACKGROUND_GREEN |
            BACKGROUND_BLUE | BACKGROUND_INTENSITY);

    SetConsoleTextAttribute(stdout_handle, GetPlatformColorCode(color) |
        FOREGROUND_INTENSITY |
        original_background_attrs);
    out << FormatString(fmt, args);

    out.flush();
    // Restores the text and background color.
    SetConsoleTextAttribute(stdout_handle, original_color_attrs);
#else
    const char* color_code = GetPlatformColorCode(color);
    if (color_code != nullptr) {
        out << FormatString("\033[0;3%sm", color_code);
    }
    out << FormatString(fmt, args) << "\033[m";
#endif
}

static std::string FormatTime(double time) {
    // For the time columns of the console printer 13 digits are reserved. One of
    // them is a space and max two of them are the time unit (e.g ns). That puts
    // us at 10 digits usable for the number.
    // Align decimal places...
    if (time < 1.0) {
        return FormatString("%10.3f", time);
    }
    if (time < 10.0) {
        return FormatString("%10.2f", time);
    }
    if (time < 100.0) {
        return FormatString("%10.1f", time);
    }
    // Assuming the time is at max 9.9999e+99 and we have 10 digits for the
    // number, we get 10-1(.)-1(e)-1(sign)-2(exponent) = 5 digits to print.
    if (time > 9999999999 /*max 10 digit number*/) {
        return FormatString("%1.4e", time);
    }
    return FormatString("%10.0f", time);
}

// kilo, Mega, Giga, Tera, Peta, Exa, Zetta, Yotta.
const char* const kBigSIUnits[] = { "k", "M", "G", "T", "P", "E", "Z", "Y" };
// Kibi, Mebi, Gibi, Tebi, Pebi, Exbi, Zebi, Yobi.
const char* const kBigIECUnits[] = { "Ki", "Mi", "Gi", "Ti",
                                    "Pi", "Ei", "Zi", "Yi" };
// milli, micro, nano, pico, femto, atto, zepto, yocto.
const char* const kSmallSIUnits[] = { "m", "u", "n", "p", "f", "a", "z", "y" };

template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];

// That gcc wants both of these prototypes seems mysterious. VC, for
// its part, can't decide which to use (another mystery). Matching of
// template overloads: the final frontier.
#ifndef CPP_MSVC
template <typename T, size_t N>
char(&ArraySizeHelper(const T(&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// We require that all three arrays have the same size.
static_assert(arraysize(kBigSIUnits) == arraysize(kBigIECUnits),
    "SI and IEC unit arrays must be the same size");
static_assert(arraysize(kSmallSIUnits) == arraysize(kBigSIUnits),
    "Small SI and Big SI unit arrays must be the same size");

const int64_t kUnitsSize = arraysize(kBigSIUnits);

void ToExponentAndMantissa(double val, int precision, double one_k,
    std::string* mantissa, int64_t* exponent) {
    std::stringstream mantissa_stream;

    if (val < 0) {
        mantissa_stream << "-";
        val = -val;
    }

    // Adjust threshold so that it never excludes things which can't be rendered
    // in 'precision' digits.
    const double adjusted_threshold =
        std::max(1.0, 1.0 / std::pow(10.0, precision));
    const double big_threshold = (adjusted_threshold * one_k) - 1;
    const double small_threshold = adjusted_threshold;
    // Values in ]simple_threshold,small_threshold[ will be printed as-is
    const double simple_threshold = 0.01;

    if (val > big_threshold) {
        // Positive powers
        double scaled = val;
        for (size_t i = 0; i < arraysize(kBigSIUnits); ++i) {
            scaled /= one_k;
            if (scaled <= big_threshold) {
                mantissa_stream << scaled;
                *exponent = static_cast<int64_t>(i + 1);
                *mantissa = mantissa_stream.str();
                return;
            }
        }
        mantissa_stream << val;
        *exponent = 0;
    }
    else if (val < small_threshold) {
        // Negative powers
        if (val < simple_threshold) {
            double scaled = val;
            for (size_t i = 0; i < arraysize(kSmallSIUnits); ++i) {
                scaled *= one_k;
                if (scaled >= small_threshold) {
                    mantissa_stream << scaled;
                    *exponent = -static_cast<int64_t>(i + 1);
                    *mantissa = mantissa_stream.str();
                    return;
                }
            }
        }
        mantissa_stream << val;
        *exponent = 0;
    }
    else {
        mantissa_stream << val;
        *exponent = 0;
    }
    *mantissa = mantissa_stream.str();
}

std::string ExponentToPrefix(int64_t exponent, bool iec) {
    if (exponent == 0) {
        return {};
    }

    const int64_t index = (exponent > 0 ? exponent - 1 : -exponent - 1);
    if (index >= kUnitsSize) {
        return {};
    }

    const char* const* array =
        (exponent > 0 ? (iec ? kBigIECUnits : kBigSIUnits) : kSmallSIUnits);

    return std::string(array[index]);
}

std::string ToBinaryStringFullySpecified(double value, int precision,
    benchmark::Counter::OneK one_k) {
    std::string mantissa;
    int64_t exponent = 0;
    ToExponentAndMantissa(value, precision,
        one_k == benchmark::Counter::kIs1024 ? 1024.0 : 1000.0, &mantissa,
        &exponent);
    return mantissa + ExponentToPrefix(exponent, one_k == benchmark::Counter::kIs1024);
}

std::string StrFormatImp(const char* msg, va_list args) {
    // we might need a second shot at this, so pre-emptivly make a copy
    va_list args_cp;
    va_copy(args_cp, args);

    // TODO(ericwf): use std::array for first attempt to avoid one memory
    // allocation guess what the size might be
    std::array<char, 256> local_buff = {};

    // 2015-10-08: vsnprintf is used instead of snd::vsnprintf due to a limitation
    // in the android-ndk
    auto ret = vsnprintf(local_buff.data(), local_buff.size(), msg, args_cp);

    va_end(args_cp);

    // handle empty expansion
    if (ret == 0) {
        return {};
    }
    if (static_cast<std::size_t>(ret) < local_buff.size()) {
        return std::string(local_buff.data());
    }

    // we did not provide a long enough buffer on our first attempt.
    // add 1 to size to account for null-byte in size cast to prevent overflow
    std::size_t size = static_cast<std::size_t>(ret) + 1;
    auto buff_ptr = std::unique_ptr<char[]>(new char[size]);
    // 2015-10-08: vsnprintf is used instead of snd::vsnprintf due to a limitation
    // in the android-ndk
    vsnprintf(buff_ptr.get(), size, msg, args);
    return std::string(buff_ptr.get());
}

std::string HumanReadableNumber(double n, benchmark::Counter::OneK one_k) {
    return ToBinaryStringFullySpecified(n, 1, one_k);
}

std::string StrFormat(const char* format, ...) {
  va_list args;
  va_start(args, format);
  std::string tmp = StrFormatImp(format, args);
  va_end(args);
  return tmp;
}


class BenchmarksCompareReporter : public benchmark::ConsoleReporter
{
public:
    BenchmarksCompareReporter() {}

    bool ReportContext(const Context& context) override {
        return ConsoleReporter::ReportContext(context);
    }

    void ReportRuns(const std::vector<Run>& reports) override {
        std::ostream& Out = GetOutputStream();

        for (const auto& run : reports)
            PrintRunData(run);
    }
private:
    static std::map<std::string, Run> _benchmarks;

    void PrintRunData(const Run& run) {
        typedef void(PrinterFn)(std::ostream&, LogColor, const char*, ...);
        std::ostream& Out = GetOutputStream();

        PrinterFn* printer = static_cast<PrinterFn*>(ColorPrintf);
        auto name_color = COLOR_GREEN;

        printer(Out, name_color, "%-*s ", run.benchmark_name().c_str());

       /* if (benchmark::internal::SkippedWithError == run.skipped) {
            printer(Out, COLOR_RED, "ERROR OCCURRED: \'%s\'",
                run.skip_message.c_str());
            printer(Out, COLOR_DEFAULT, "\n");
            return;
        }
        if (benchmark::internal::SkippedWithMessage == run.skipped) {
            printer(Out, COLOR_WHITE, "SKIPPED: \'%s\'", run.skip_message.c_str());
            printer(Out, COLOR_DEFAULT, "\n");
            return;
        }

        const double real_time = run.GetAdjustedRealTime();
        const double cpu_time = run.GetAdjustedCPUTime();

        const std::string real_time_str = FormatTime(real_time);
        const std::string cpu_time_str = FormatTime(cpu_time);

        if (run.report_rms) {
            printer(Out, COLOR_YELLOW, "%10.0f %-4s %10.0f %-4s ", real_time * 100, "%",
                cpu_time * 100, "%");
        }
        else if (run.run_type != Run::RT_Aggregate ||
            run.aggregate_unit == benchmark::StatisticUnit::kTime) {
            const char* timeLabel = GetTimeUnitString(run.time_unit);
            printer(Out, COLOR_YELLOW, "%s %-4s %s %-4s ", real_time_str.c_str(),
                timeLabel, cpu_time_str.c_str(), timeLabel);
        }
        else {
            assert(run.aggregate_unit == benchmark::tatisticUnit::kPercentage);
            printer(Out, COLOR_YELLOW, "%10.2f %-4s %10.2f %-4s ",
                (100. * run.real_accumulated_time), "%",
                (100. * run.cpu_accumulated_time), "%");
        }

        if (!run.report_big_o && !run.report_rms) {
            printer(Out, COLOR_CYAN, "%10lld", run.iterations);
        }

        for (const auto& c : run.counters) {
            const std::size_t cNameLen =
                std::max(static_cast<std::size_t>(10), c.first.length());
            std::string s;
            const char* unit = "";
            if (run.run_type == Run::RT_Aggregate &&
                run.aggregate_unit == benchmark::StatisticUnit::kPercentage) {
                s = StrFormat("%.2f", 100. * c.second.value);
                unit = "%";
            }
            else {
                s = HumanReadableNumber(c.second.value, c.second.oneK);
                if ((c.second.flags & benchmark::Counter::kIsRate) != 0) {
                    unit = (c.second.flags & benchmark::Counter::kInvert) != 0 ? "s" : "/s";
                }
            }
            printer(Out, COLOR_DEFAULT, " %s=%s%s", c.first.c_str(), s.c_str(), unit);
        }

        if (!run.report_label.empty()) {
            printer(Out, COLOR_DEFAULT, " %s", run.report_label.c_str());
        }

        printer(Out, COLOR_DEFAULT, "\n");*/
    }
};
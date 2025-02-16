#pragma once 

#include <base/io/ByteArrayAlgorithms.h>
#include <base/string/StringFwd.h>

#include <algorithm>        // std::find
#include <string.h>         // for memchr

#include <base/utility/TypeTraits.h>
#include <base/string/String.h>

#include <base/io/ByteArray.h>
#include <base/utility/TypeInfo.h>


namespace base::string {
    enum SplitBehaviorFlags {
        KeepEmptyParts = 0,
        SkipEmptyParts = 0x1,
    };

    enum CaseSensitivity {
        CaseInsensitive,
        CaseSensitive
    };

    DECLARE_FLAGS(SplitBehavior, SplitBehaviorFlags)

    [[nodiscard]] DECL_PURE_FUNCTION sizetype qustrlen(const char16_t* str) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype qustrnlen(const char16_t* str, sizetype maxlen) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION const char16_t* qustrchr(StringView str, char16_t ch) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(StringView   lhs, StringView   rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(StringView   lhs, Latin1StringView rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(StringView   lhs, BasicUtf8StringView<false> rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(Latin1StringView lhs, StringView   rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(Latin1StringView lhs, Latin1StringView rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(Latin1StringView lhs, BasicUtf8StringView<false> rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(BasicUtf8StringView<false> lhs, StringView   rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(BasicUtf8StringView<false> lhs, Latin1StringView rhs, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION int compareStrings(BasicUtf8StringView<false> lhs, BasicUtf8StringView<false> rhs, CaseSensitivity cs = CaseSensitive) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(StringView   lhs, StringView   rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(StringView   lhs, Latin1StringView rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(StringView   lhs, BasicUtf8StringView<false> rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(Latin1StringView lhs, StringView   rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(Latin1StringView lhs, Latin1StringView rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(Latin1StringView lhs, BasicUtf8StringView<false> rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(BasicUtf8StringView<false> lhs, StringView   rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(BasicUtf8StringView<false> lhs, Latin1StringView rhs) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool equalStrings(BasicUtf8StringView<false> lhs, BasicUtf8StringView<false> rhs) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION bool startsWith(StringView   haystack, StringView   needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool startsWith(StringView   haystack, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool startsWith(Latin1StringView haystack, StringView   needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool startsWith(Latin1StringView haystack, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION bool endsWith(StringView   haystack, StringView   needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool endsWith(StringView   haystack, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool endsWith(Latin1StringView haystack, StringView   needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool endsWith(Latin1StringView haystack, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION sizetype findString(StringView haystack, sizetype from, StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype findString(StringView haystack, sizetype from, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype findString(Latin1StringView haystack, sizetype from, StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype findString(Latin1StringView haystack, sizetype from, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION sizetype lastIndexOf(StringView haystack, sizetype from, StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype lastIndexOf(StringView haystack, sizetype from, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype lastIndexOf(Latin1StringView haystack, sizetype from, StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype lastIndexOf(Latin1StringView haystack, sizetype from, Latin1StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION StringView   trimmed(StringView   s) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION Latin1StringView trimmed(Latin1StringView s) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION bool isLower(StringView s) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool isUpper(StringView s) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION sizetype count(StringView haystack, Char needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype count(StringView haystack, StringView needle, CaseSensitivity cs = CaseSensitive) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION sizetype count(StringView haystack, Latin1StringView needle, CaseSensitivity cs = CaseSensitive);
    [[nodiscard]] DECL_PURE_FUNCTION sizetype count(Latin1StringView haystack, Latin1StringView needle, CaseSensitivity cs = CaseSensitive);
    [[nodiscard]] DECL_PURE_FUNCTION sizetype count(Latin1StringView haystack, StringView needle, CaseSensitivity cs = CaseSensitive);
    [[nodiscard]] DECL_PURE_FUNCTION sizetype count(Latin1StringView haystack, Char needle, CaseSensitivity cs = CaseSensitive) noexcept;


    [[nodiscard]] sizetype indexOf(StringView viewHaystack,
        const String* stringHaystack,
        const RegularExpression& re,
        sizetype from = 0,
        RegularExpressionMatch* rmatch = nullptr);
    [[nodiscard]] sizetype indexOf(StringView haystack,
        const RegularExpression& re,
        sizetype from = 0,
        RegularExpressionMatch* rmatch = nullptr);
    [[nodiscard]] sizetype lastIndexOf(StringView viewHaystack,
        const String* stringHaystack,
        const RegularExpression& re,
        sizetype from = -1,
        RegularExpressionMatch* rmatch = nullptr);
    [[nodiscard]] sizetype lastIndexOf(StringView haystack,
        const RegularExpression& re,
        sizetype from = -1,
        RegularExpressionMatch* rmatch = nullptr);
    [[nodiscard]] bool contains(StringView viewHaystack,
        const String* stringHaystack,
        const RegularExpression& re,
        RegularExpressionMatch* rmatch = nullptr);
    [[nodiscard]] bool contains(StringView haystack,
        const RegularExpression& re,
        RegularExpressionMatch* rmatch = nullptr);
    [[nodiscard]] sizetype count(StringView haystack, const RegularExpression& re);

    [[nodiscard]] String convertToString(AnyStringView s);

    [[nodiscard]] io::ByteArray convertToLatin1(StringView str);
    [[nodiscard]] io::ByteArray convertToUtf8(StringView str);
    [[nodiscard]] io::ByteArray convertToLocal8Bit(StringView str);
    [[nodiscard]] QList<uint> convertToUcs4(StringView str); // ### Qt 7 char32_t

    [[nodiscard]] DECL_PURE_FUNCTION bool isRightToLeft(StringView string) noexcept;

    [[nodiscard]] DECL_PURE_FUNCTION bool isAscii(Latin1StringView s) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool isAscii(StringView   s) noexcept;
    [[nodiscard]] constexpr inline   bool isLatin1(Latin1StringView s) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool isLatin1(StringView   s) noexcept;
    [[nodiscard]] DECL_PURE_FUNCTION bool isValidUtf16(StringView s) noexcept;

    template <typename _Char, size_t N> [[nodiscard]] constexpr always_inline
        sizetype lengthHelperContainerLoop(const _Char(&str)[N])
    {
#if defined(__cpp_lib_constexpr_algorithms) && defined(CPP_GNU_ONLY)
        // libstdc++'s std::find / std::find_if manages to execute more steps
        // than the loop below
        const auto it = std::find(str, str + N, Char(0));
        return it - str;
#else
        // std::char_traits<C> is deprecated for C not one of the standard char
        // types, so we have to roll out our own loop.
        for (size_t i = 0; i < N; ++i) {
            if (str[i] == _Char(0))
                return sizetype(i);
        }
        return sizetype(N);
#endif
    }

    template <typename _Char, size_t N> [[nodiscard]] constexpr always_inline
        std::enable_if_t<sizeof(_Char) == sizeof(char16_t), sizetype>
        lengthHelperContainer(const _Char(&str)[N])
    {
        // The following values were empirically determined to detect the threshold
        // at which the compiler gives up pre-calculating the std::find() below and
        // instead inserts code to be executed at runtime.
        constexpr size_t RuntimeThreshold =
#if defined(CPP_CLANG)
            // tested on Clang 15, 16 & 17
            1023
#elif defined(CPP_GNU)
            // tested through GCC 13.1 at -O3 compilation level
            // note: at -O2, GCC always generates a loop!
            __cplusplus >= 202002L ? 39 : 17
#else
            0
#endif
            ;
        if constexpr (N == 1) {
            return str[0] == Char(0) ? 0 : 1;
        }
        else if constexpr (N > RuntimeThreshold) {
#ifdef SUPPORTS_IS_CONSTANT_EVALUATED
            if (!is_constant_evaluated())
                return qustrnlen(reinterpret_cast<const char16_t*>(str), N);
#endif
        }

        return lengthHelperContainerLoop(str);
    }

    template <typename _Char, size_t N> [[nodiscard]] constexpr inline
        std::enable_if_t<sizeof(_Char) == 1, sizetype> lengthHelperContainer(const _Char(&str)[N])
    {
#ifdef SUPPORTS_IS_CONSTANT_EVALUATED
        if (!is_constant_evaluated())
            return qstrnlen(reinterpret_cast<const char*>(str), N);
#endif

        return lengthHelperContainerLoop(str);
    }

    template <typename Container>
    constexpr sizetype lengthHelperContainer(const Container& c) noexcept
    {
        return sizetype(std::size(c));
    }
} // namespace base::string

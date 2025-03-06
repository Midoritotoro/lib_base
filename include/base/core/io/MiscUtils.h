#pragma once 


#include <base/core/BaseNamespace.h>
#include <base/system/Platform.h>


__BASE_IO_NAMESPACE_BEGIN

namespace misc {

    constexpr inline char toHexUpper(char32_t value) noexcept
    {
        return "0123456789ABCDEF"[value & 0xF];
    }

    constexpr inline char toHexLower(char32_t value) noexcept
    {
        return "0123456789abcdef"[value & 0xF];
    }

    [[nodiscard]] constexpr inline bool isHexDigit(char32_t c) noexcept
    {
        return (c >= '0' && c <= '9')
            || (c >= 'A' && c <= 'F')
            || (c >= 'a' && c <= 'f');
    }

    constexpr inline int fromHex(char32_t c) noexcept
    {
        return ((c >= '0') && (c <= '9')) ? int(c - '0') :
            ((c >= 'A') && (c <= 'F')) ? int(c - 'A' + 10) :
            ((c >= 'a') && (c <= 'f')) ? int(c - 'a' + 10) :
            /* otherwise */              -1;
    }

    constexpr inline char toOct(char32_t value) noexcept
    {
        return char('0' + (value & 0x7));
    }

    [[nodiscard]] constexpr inline bool isOctalDigit(char32_t c) noexcept
    {
        return c >= '0' && c <= '7';
    }

    constexpr inline int fromOct(char32_t c) noexcept
    {
        return isOctalDigit(c) ? int(c - '0') : -1;
    }

    [[nodiscard]] constexpr inline bool isAsciiDigit(char32_t c) noexcept
    {
        return c >= '0' && c <= '9';
    }

    constexpr inline bool isAsciiUpper(char32_t c) noexcept
    {
        return c >= 'A' && c <= 'Z';
    }

    constexpr inline bool isAsciiLower(char32_t c) noexcept
    {
        return c >= 'a' && c <= 'z';
    }

    constexpr inline bool isAsciiLetterOrNumber(char32_t c) noexcept
    {
        return  isAsciiDigit(c) || isAsciiLower(c) || isAsciiUpper(c);
    }

    constexpr inline char toAsciiLower(char ch) noexcept
    {
        return isAsciiUpper(ch) ? ch - 'A' + 'a' : ch;
    }

    constexpr inline char toAsciiUpper(char ch) noexcept
    {
        return isAsciiLower(ch) ? ch - 'a' + 'A' : ch;
    }

    constexpr inline int caseCompareAscii(char lhs, char rhs) noexcept
    {
        const char lhsLower = toAsciiLower(lhs);
        const char rhsLower = toAsciiLower(rhs);
        return int(uchar(lhsLower)) - int(uchar(rhsLower));
    }

    constexpr inline int isAsciiPrintable(char32_t ch) noexcept
    {
        return ch >= ' ' && ch < 0x7f;
    }

    constexpr inline int base_lencmp(sizetype lhs, sizetype rhs) noexcept
    {
        return lhs == rhs ? 0 :
            lhs > rhs ? 1 :
            /* else */  -1;
    }

    static constexpr inline uchar asciiUpper(uchar c)
    {
        return c >= 'a' && c <= 'z' ? c & ~0x20 : c;
    }

    static constexpr inline uchar asciiLower(uchar c)
    {
        return c >= 'A' && c <= 'Z' ? c | 0x20 : c;
    }


    __BASE_IO_NAMESPACE_END
}
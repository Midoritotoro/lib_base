#pragma once 


#include <base/core/BaseNamespace.h>
#include <base/core/arch/Platform.h>


__BASE_IO_NAMESPACE_BEGIN

constexpr inline NODISCARD
    char toHexUpper(char32_t value) noexcept 
{
    return "0123456789ABCDEF"[value & 0xF];
}

constexpr inline NODISCARD 
    char toHexLower(char32_t value) noexcept
{
    return "0123456789abcdef"[value & 0xF];
}

constexpr inline NODISCARD 
    bool isHexDigit(char32_t c) noexcept
{
    return (c >= '0' && c <= '9')
        || (c >= 'A' && c <= 'F')
        || (c >= 'a' && c <= 'f');
}

constexpr inline NODISCARD 
    int fromHex(char32_t c) noexcept
{
    return ((c >= '0') && (c <= '9')) ? int(c - '0') :
        ((c >= 'A') && (c <= 'F')) ? int(c - 'A' + 10) :
        ((c >= 'a') && (c <= 'f')) ? int(c - 'a' + 10) :
                                                    -1;
}

constexpr inline NODISCARD
    char toOct(char32_t value) noexcept
{
    return char('0' + (value & 0x7));
}

constexpr inline NODISCARD
    bool isOctalDigit(char32_t c) noexcept
{
    return c >= '0' && c <= '7';
}

constexpr inline NODISCARD
    int fromOct(char32_t c) noexcept
{
    return isOctalDigit(c) ? int(c - '0') : -1;
}

constexpr inline NODISCARD 
    bool isAsciiDigit(char32_t c) noexcept
{
    return c >= '0' && c <= '9';
}

constexpr inline NODISCARD 
    bool isAsciiUpper(char32_t c) noexcept
{
    return c >= 'A' && c <= 'Z';
}

constexpr inline NODISCARD 
    bool isAsciiLower(char32_t c) noexcept
{
    return c >= 'a' && c <= 'z';
}

constexpr inline NODISCARD 
    bool isAsciiLetterOrNumber(char32_t c) noexcept
{
    return  isAsciiDigit(c) || isAsciiLower(c) || isAsciiUpper(c);
}

constexpr inline NODISCARD 
    char toAsciiLower(char ch) noexcept
{
    return isAsciiUpper(ch) ? ch - 'A' + 'a' : ch;
}

constexpr inline NODISCARD 
    char toAsciiUpper(char ch) noexcept
{
    return isAsciiLower(ch) ? ch - 'a' + 'A' : ch;
}

constexpr inline NODISCARD
    int caseCompareAscii(char lhs, char rhs) noexcept
{
    const char lhsLower = toAsciiLower(lhs);
    const char rhsLower = toAsciiLower(rhs);

    return int(uchar(lhsLower)) - int(uchar(rhsLower));
}

constexpr inline NODISCARD
    int isAsciiPrintable(char32_t ch) noexcept
{
    return ch >= ' ' && ch < 0x7f;
}

constexpr inline NODISCARD
    int base_lencmp(sizetype lhs, sizetype rhs) noexcept
{
    return lhs == rhs ? 0 :
        lhs > rhs ? 1 :
        /* else */  -1;
}

static constexpr NODISCARD
    inline uchar asciiUpper(uchar c)
{
    return c >= 'a' && c <= 'z' ? c & ~0x20 : c;
}

static constexpr NODISCARD
    inline uchar asciiLower(uchar c)
{
    return c >= 'A' && c <= 'Z' ? c | 0x20 : c;
}

__BASE_IO_NAMESPACE_END

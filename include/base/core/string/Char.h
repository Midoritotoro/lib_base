#pragma once 


#include <base/core/string/StringConfig.h> 
#include <base/core/string/CharacterHelper.h>

__BASE_STRING_NAMESPACE_BEGIN

class Char {
public:
    enum SpecialCharacter {
        Null = 0x0000,
        Tabulation = 0x0009,
        LineFeed = 0x000a,
        FormFeed = 0x000c,
        CarriageReturn = 0x000d,
        Space = 0x0020,
        Nbsp = 0x00a0,
        SoftHyphen = 0x00ad,
        ReplacementCharacter = 0xfffd,
        ObjectReplacementCharacter = 0xfffc,
        ByteOrderMark = 0xfeff,
        ByteOrderSwapped = 0xfffe,
        ParagraphSeparator = 0x2029,
        LineSeparator = 0x2028,
        VisualTabCharacter = 0x2192,
        LastValidCodePoint = 0x10ffff
    };

    enum Direction
    {
        DirL, DirR, DirEN, DirES, DirET, DirAN, DirCS, DirB, DirS, DirWS, DirON,
        DirLRE, DirLRO, DirAL, DirRLE, DirRLO, DirPDF, DirNSM, DirBN,
        DirLRI, DirRLI, DirFSI, DirPDI
    };

    enum Decomposition
    {
        NoDecomposition,
        Canonical,
        Font,
        NoBreak,
        Initial,
        Medial,
        Final,
        Isolated,
        Circle,
        Super,
        Sub,
        Vertical,
        Wide,
        Narrow,
        Small,
        Square,
        Compat,
        Fraction
    };


    enum Category
    {
        Mark_NonSpacing,          //   Mn
        Mark_SpacingCombining,    //   Mc
        Mark_Enclosing,           //   Me

        Number_DecimalDigit,      //   Nd
        Number_Letter,            //   Nl
        Number_Other,             //   No

        Separator_Space,          //   Zs
        Separator_Line,           //   Zl
        Separator_Paragraph,      //   Zp

        Other_Control,            //   Cc
        Other_Format,             //   Cf
        Other_Surrogate,          //   Cs
        Other_PrivateUse,         //   Co
        Other_NotAssigned,        //   Cn

        Letter_Uppercase,         //   Lu
        Letter_Lowercase,         //   Ll
        Letter_Titlecase,         //   Lt
        Letter_Modifier,          //   Lm
        Letter_Other,             //   Lo

        Punctuation_Connector,    //   Pc
        Punctuation_Dash,         //   Pd
        Punctuation_Open,         //   Ps
        Punctuation_Close,        //   Pe
        Punctuation_InitialQuote, //   Pi
        Punctuation_FinalQuote,   //   Pf
        Punctuation_Other,        //   Po

        Symbol_Math,              //   Sm
        Symbol_Currency,          //   Sc
        Symbol_Modifier,          //   Sk
        Symbol_Other              //   So
    };

    base_constexpr_cxx20 base_implicit Char() noexcept:
        ucs(0) 
    {}

    base_constexpr_cxx20 base_implicit Char(ushort rc) noexcept:
        ucs(rc) 
    {}

    base_constexpr_cxx20 base_implicit Char(uchar c, uchar r) noexcept:
        ucs(char16_t((r << 8) | c))
    {}

    base_constexpr_cxx20 base_implicit Char(short rc) noexcept:
        ucs(char16_t(rc)) 
    {}

    base_constexpr_cxx20 base_implicit Char(uint rc) noexcept:
        ucs((Assert(rc <= 0xffff), char16_t(rc))) 
    {}

    base_constexpr_cxx20 base_implicit Char(int rc) noexcept:
        Char(uint(rc)) 
    {}

    base_constexpr_cxx20 base_implicit Char(SpecialCharacter s) noexcept:
        ucs(char16_t(s)) 
    {}

    base_constexpr_cxx20 base_implicit Char(CharacterHelper ch) noexcept:
        ucs(ch.unicode())
    {}

    base_constexpr_cxx20 base_implicit Char(char16_t ch) noexcept: 
        ucs(ch)
    {}
#if defined(OS_WIN)
    base_constexpr_cxx20 base_implicit Char(wchar_t ch) noexcept:
        ucs(char16_t(ch))
    {}
#endif

    base_constexpr_cxx20 base_implicit Char(char c) noexcept: 
        ucs(uchar(c)) 
    {}

    base_constexpr_cxx20 base_implicit Char(uchar c) noexcept :
        ucs(c)
    {}

    static base_constexpr_cxx20 Char fromUcs2(char16_t c) noexcept {
        return Char{c}; 
    }

    base_nodiscard base_constexpr_cxx20 static auto fromUcs4(char32_t c) noexcept;

    inline Category category() const noexcept { 
        return Char::category(ucs);
    }

    inline unsigned char combiningClass() const noexcept { 
        return Char::combiningClass(ucs);
    }

    inline Char mirroredChar() const noexcept {
        return Char(Char::mirroredChar(ucs));
    }

    inline bool hasMirrored() const noexcept { 
        return Char::hasMirrored(ucs);
    }

    inline int digitValue() const noexcept { 
        return Char::digitValue(ucs);
    }

    inline Char toLower() const noexcept { 
        return Char(Char::toLower(ucs));
    }

    inline Char toUpper() const noexcept {
        return Char(Char::toUpper(ucs));
    }

    inline Char toTitleCase() const noexcept { 
        return Char(Char::toTitleCase(ucs)); 
    }

    inline Char toCaseFolded() const noexcept {
        return Char(Char::toCaseFolded(ucs)); 
    }

    base_constexpr_cxx20 inline char toLatin1() const noexcept {
        return ucs > 0xff ? '\0' : char(ucs);
    }

    base_constexpr_cxx20 inline char16_t unicode() const noexcept {
        return ucs; 
    }

    base_constexpr_cxx20 inline char16_t& unicode() noexcept {
        return ucs; 
    }

    static base_constexpr_cxx20 Char fromLatin1(char c) noexcept { 
        return CharacterHelper(c);
    }

    base_constexpr_cxx20 inline bool isNull() const noexcept { 
        return ucs == 0;
    }

    inline bool isPrint() const noexcept { 
        return Char::isPrint(ucs);
    }

    base_constexpr_cxx20 inline bool isSpace() const noexcept { 
        return Char::isSpace(ucs); 
    }

    inline bool isMark() const noexcept { 
        return Char::isMark(ucs); 
    }

    inline bool isPunct() const noexcept {
        return Char::isPunct(ucs);
    }

    inline bool isSymbol() const noexcept { 
        return Char::isSymbol(ucs);
    }

    base_constexpr_cxx20 inline bool isLetter() const noexcept {
        return Char::isLetter(ucs);
    }

    base_constexpr_cxx20 inline bool isNumber() const noexcept { 
        return Char::isNumber(ucs);
    }

    base_constexpr_cxx20 inline bool isLetterOrNumber() const noexcept { 
        return Char::isLetterOrNumber(ucs);
    }

    base_constexpr_cxx20 inline bool isDigit() const noexcept {
        return Char::isDigit(ucs); 
    }

    base_constexpr_cxx20 inline bool isLower() const noexcept { 
        return Char::isLower(ucs);
    }

    base_constexpr_cxx20 inline bool isUpper() const noexcept { 
        return Char::isUpper(ucs); 
    }

    base_constexpr_cxx20 inline bool isTitleCase() const noexcept {
        return Char::isTitleCase(ucs); 
    }

    base_constexpr_cxx20 inline bool isNonCharacter() const noexcept { 
        return Char::isNonCharacter(ucs);
    }

    base_constexpr_cxx20 inline bool isHighSurrogate() const noexcept {
        return Char::isHighSurrogate(ucs); 
    }

    base_constexpr_cxx20 inline bool isLowSurrogate() const noexcept {
        return Char::isLowSurrogate(ucs);
    }

    base_constexpr_cxx20 inline bool isSurrogate() const noexcept { 
        return Char::isSurrogate(ucs); 
    }

    base_constexpr_cxx20 inline uchar cell() const noexcept {
        return uchar(ucs & 0xff);
    }

    base_constexpr_cxx20 inline uchar row() const noexcept {
        return uchar((ucs >> 8) & 0xff); 
    }

    base_constexpr_cxx20 inline void setCell(uchar acell) noexcept {
        ucs = char16_t((ucs & 0xff00) + acell);
    }

    base_constexpr_cxx20 inline void setRow(uchar arow) noexcept {
        ucs = char16_t((char16_t(arow) << 8) + (ucs & 0xff));
    }

    static base_constexpr_cxx20 inline
        bool isNonCharacter(char32_t ucs4) noexcept
    {
        return ucs4 >= 0xfdd0 && 
            (ucs4 <= 0xfdef || 
            (ucs4 & 0xfffe) == 0xfffe);
    }

    static base_constexpr_cxx20 inline 
        bool isHighSurrogate(char32_t ucs4) noexcept
    {
        return (ucs4 & 0xfffffc00) == 0xd800; // 0xd800 + up to 1023 (0x3ff)
    }

    static base_constexpr_cxx20 inline 
        bool isLowSurrogate(char32_t ucs4) noexcept
    {
        return (ucs4 & 0xfffffc00) == 0xdc00; // 0xdc00 + up to 1023 (0x3ff)
    }

    static base_constexpr_cxx20 inline
        bool isSurrogate(char32_t ucs4) noexcept
    {
        return (ucs4 - 0xd800u < 2048u);
    }

    static base_constexpr_cxx20 inline 
        bool requiresSurrogates(char32_t ucs4) noexcept
    {
        return (ucs4 >= 0x10000);
    }

    static base_constexpr_cxx20 inline 
        char32_t surrogateToUcs4(
            char16_t high,
            char16_t low) noexcept
    {
        // 0x010000 through 0x10ffff, provided params are actual high, low surrogates.
        // 0x010000 + ((high - 0xd800) << 10) + (low - 0xdc00), optimized:
        return (char32_t(high) << 10) + low - 0x35fdc00;
    }

    static base_constexpr_cxx20 inline 
        char32_t surrogateToUcs4(
            Char high,
            Char low) noexcept
    {
        return surrogateToUcs4(high.ucs, low.ucs);
    }

    static base_constexpr_cxx20 inline 
        char16_t highSurrogate(char32_t ucs4) noexcept
    {
        return char16_t((ucs4 >> 10) + 0xd7c0);
    }

    static base_constexpr_cxx20 inline 
        char16_t lowSurrogate(char32_t ucs4) noexcept
    {
        return char16_t(ucs4 % 0x400 + 0xdc00);
    }

    static unsigned char base_fastcall
        combiningClass(char32_t ucs4) noexcept;

    static char32_t base_fastcall
        mirroredChar(char32_t ucs4) noexcept;
    static bool base_fastcall 
        hasMirrored(char32_t ucs4) noexcept;

    static int base_fastcall 
        digitValue(char32_t ucs4) noexcept;
    static char32_t base_fastcall 
        toLower(char32_t ucs4) noexcept;

    static char32_t base_fastcall
        toUpper(char32_t ucs4) noexcept;
    static char32_t base_fastcall 
        toTitleCase(char32_t ucs4) noexcept;

    static char32_t base_fastcall 
        toCaseFolded(char32_t ucs4) noexcept;

    static bool base_fastcall 
        isPrint(char32_t ucs4) noexcept;
    static base_constexpr_cxx20 inline bool
        isSpace(char32_t ucs4) noexcept 
    {
        // note that [0x09..0x0d] + 0x85 are exceptional Cc-s and must be handled explicitly
        return ucs4 == 0x20 || (ucs4 <= 0x0d && ucs4 >= 0x09)
            || (ucs4 > 127 && (ucs4 == 0x85 || ucs4 == 0xa0 
            || Char::isSpace_helper(ucs4)));
    }

    static bool base_fastcall 
        isMark(char32_t ucs4) noexcept ;

    static bool base_fastcall 
        isPunct(char32_t ucs4) noexcept ;

    static bool base_fastcall 
        isSymbol(char32_t ucs4) noexcept ;

    static base_constexpr_cxx20 inline 
        bool isLetter(char32_t ucs4) noexcept 
    {
        return (ucs4 >= 'A' && ucs4 <= 'z' 
            && (ucs4 >= 'a' || ucs4 <= 'Z'))
            || (ucs4 > 127 && Char::isLetter_helper(ucs4));
    }

    static base_constexpr_cxx20 inline 
        bool isNumber(char32_t ucs4) noexcept 
    {
        return (ucs4 <= '9' && ucs4 >= '0') 
            || (ucs4 > 127 && Char::isNumber_helper(ucs4));
    }

    static base_constexpr_cxx20 inline 
        bool isLetterOrNumber(char32_t ucs4) noexcept 
    {
        return (ucs4 >= 'A' && ucs4 <= 'z' 
            && (ucs4 >= 'a' || ucs4 <= 'Z'))
            || (ucs4 >= '0' && ucs4 <= '9')
            || (ucs4 > 127 && Char::isLetterOrNumber_helper(ucs4));
    }

    static base_constexpr_cxx20 inline 
        bool isDigit(char32_t ucs4) noexcept 
    {
        return (ucs4 <= '9' && ucs4 >= '0') 
            || (ucs4 > 127 && Char::category(ucs4) 
                == Number_DecimalDigit);
    }

    static base_constexpr_cxx20 inline 
        bool isLower(char32_t ucs4) noexcept 
    {
        return (ucs4 <= 'z' && ucs4 >= 'a') 
            || (ucs4 > 127 && Char::category(ucs4)
                == Letter_Lowercase);
    }

    static base_constexpr_cxx20 inline 
        bool isUpper(char32_t ucs4) noexcept 
    {
        return (ucs4 <= 'Z' && ucs4 >= 'A') 
            || (ucs4 > 127 && Char::category(ucs4) 
                == Letter_Uppercase);
    }

    static base_constexpr_cxx20 inline 
        bool isTitleCase(char32_t ucs4) noexcept 
    {
        return ucs4 > 127 && Char::category(ucs4) 
            == Letter_Titlecase;
    }

    static Category base_fastcall 
        category(char32_t ucs4) noexcept ;

    friend base_constexpr_cxx20 inline
        bool operator==(
            Char c1,
            Char c2) noexcept 
    { 
        return c1.ucs == c2.ucs; 
    }

    friend base_constexpr_cxx20 inline
        bool operator< (
            Char c1,
            Char c2) noexcept 
    {
        return c1.ucs < c2.ucs; 
    }

    friend base_constexpr_cxx20 inline 
        bool operator!=(
            Char c1,
            Char c2) noexcept 
    { 
        return !operator==(c1, c2);
    }

    friend base_constexpr_cxx20 inline
        bool operator>=(
            Char c1, 
            Char c2) noexcept 
    {
        return !operator< (c1, c2);
    }

    friend base_constexpr_cxx20 inline 
        bool operator> (
            Char c1,
            Char c2) noexcept 
    { 
        return  operator< (c2, c1);
    }

    friend base_constexpr_cxx20 inline 
        bool operator<=(
            Char c1,
            Char c2) noexcept
    {
        return !operator< (c2, c1);
    }

    friend base_constexpr_cxx20 inline
        bool operator==(
            Char lhs, 
            std::nullptr_t) noexcept 
    { 
        return lhs.isNull(); 
    }

    friend base_constexpr_cxx20 inline 
        bool operator< (
            Char, 
            std::nullptr_t) noexcept {
        return false; 
    }

    friend base_constexpr_cxx20 inline
        bool operator==(
            std::nullptr_t,
            Char rhs) noexcept 
    { 
        return rhs.isNull();
    }

    friend base_constexpr_cxx20 inline
        bool operator< (
            std::nullptr_t, 
            Char rhs) noexcept 
    { 
        return !rhs.isNull(); 
    }

    friend base_constexpr_cxx20 inline
        bool operator!=(
            Char lhs,
            std::nullptr_t) noexcept 
    { 
        return !operator==(lhs, nullptr);
    }

    friend base_constexpr_cxx20 inline
        bool operator>=(
            Char lhs,
            std::nullptr_t) noexcept
    { 
        return !operator< (lhs, nullptr);
    }

    friend base_constexpr_cxx20 inline 
        bool operator> (
            Char lhs,
            std::nullptr_t) noexcept
    { 
        return operator< (nullptr, lhs);
    }

    friend base_constexpr_cxx20 inline
        bool operator<=(
            Char lhs,
            std::nullptr_t) noexcept 
    { 
        return !operator< (nullptr, lhs); 
    }

    friend base_constexpr_cxx20 inline
        bool operator!=(
            std::nullptr_t,
            Char rhs) noexcept
    {
        return !operator==(nullptr, rhs); 
    }

    friend base_constexpr_cxx20 inline 
        bool operator>=(
            std::nullptr_t,
            Char rhs) noexcept
    { 
        return !operator< (nullptr, rhs); 
    }

    friend base_constexpr_cxx20 inline 
        bool operator> (
            std::nullptr_t,
            Char rhs) noexcept 
    {
        return  operator< (rhs, nullptr);
    }

    friend base_constexpr_cxx20 inline 
        bool operator<=(
            std::nullptr_t, 
            Char rhs) noexcept
    { 
        return !operator< (rhs, nullptr); 
    }
private:
    static bool base_fastcall 
        isSpace_helper(char32_t ucs4) noexcept ;

    static bool base_fastcall 
        isLetter_helper(char32_t ucs4) noexcept ;

    static bool base_fastcall 
        isNumber_helper(char32_t ucs4) noexcept ;

    static bool base_fastcall 
        isLetterOrNumber_helper(char32_t ucs4) noexcept ;

    char16_t ucs;
};

__BASE_STRING_NAMESPACE_END


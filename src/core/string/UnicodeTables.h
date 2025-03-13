#pragma once 

#include <base/core/string/Char.h>

__BASE_STRING_NAMESPACE_BEGIN

enum Case {
    LowerCase,
    UpperCase,
    TitleCase,
    CaseFold,

    NumCases
};

struct Properties {
    ushort category : 8; /* 5 used */
    ushort direction : 8; /* 5 used */
    ushort combiningClass : 8;
    ushort joining : 3;
    signed short digitValue : 5;
    signed short mirrorDiff : 16;
    ushort unicodeVersion : 5; /* 5 used */
    ushort eastAsianWidth : 3; /* 3 used */
    ushort nfQuickCheck : 8;

    struct {
        ushort special : 1;
        signed short diff : 15;
    } cases[NumCases];

    ushort graphemeBreakClass : 5; /* 5 used */
    ushort wordBreakClass : 5; /* 5 used */
    ushort lineBreakClass : 6; /* 6 used */
    ushort sentenceBreakClass : 4; /* 4 used */
    ushort idnaStatus : 4; /* 3 used */
    ushort script : 8;
};

const Properties* FASTCALL properties(char32_t ucs4) noexcept;
const Properties* FASTCALL properties(char16_t ucs2) noexcept;

static_assert(sizeof(Properties) == 20);

enum class EastAsianWidth : unsigned int {
    A,
    F,
    H,
    N,
    Na,
    W,
};

enum GraphemeBreakClass {
    GraphemeBreak_Any,
    GraphemeBreak_CR,
    GraphemeBreak_LF,
    GraphemeBreak_Control,
    GraphemeBreak_Extend,
    GraphemeBreak_ZWJ,
    GraphemeBreak_RegionalIndicator,
    GraphemeBreak_Prepend,
    GraphemeBreak_SpacingMark,
    GraphemeBreak_L,
    GraphemeBreak_V,
    GraphemeBreak_T,
    GraphemeBreak_LV,
    GraphemeBreak_LVT,
    GraphemeBreak_Extended_Pictographic,

    NumGraphemeBreakClasses
};

enum WordBreakClass {
    WordBreak_Any,
    WordBreak_CR,
    WordBreak_LF,
    WordBreak_Newline,
    WordBreak_Extend,
    WordBreak_ZWJ,
    WordBreak_Format,
    WordBreak_RegionalIndicator,
    WordBreak_Katakana,
    WordBreak_HebrewLetter,
    WordBreak_ALetter,
    WordBreak_SingleQuote,
    WordBreak_DoubleQuote,
    WordBreak_MidNumLet,
    WordBreak_MidLetter,
    WordBreak_MidNum,
    WordBreak_Numeric,
    WordBreak_ExtendNumLet,
    WordBreak_WSegSpace,

    NumWordBreakClasses
};

enum SentenceBreakClass {
    SentenceBreak_Any,
    SentenceBreak_CR,
    SentenceBreak_LF,
    SentenceBreak_Sep,
    SentenceBreak_Extend,
    SentenceBreak_Sp,
    SentenceBreak_Lower,
    SentenceBreak_Upper,
    SentenceBreak_OLetter,
    SentenceBreak_Numeric,
    SentenceBreak_ATerm,
    SentenceBreak_SContinue,
    SentenceBreak_STerm,
    SentenceBreak_Close,

    NumSentenceBreakClasses
};

enum LineBreakClass {
    LineBreak_OP, LineBreak_CL, LineBreak_CP, LineBreak_QU, LineBreak_GL,
    LineBreak_NS, LineBreak_EX, LineBreak_SY, LineBreak_IS, LineBreak_PR,
    LineBreak_PO, LineBreak_NU, LineBreak_AL, LineBreak_HL, LineBreak_ID,
    LineBreak_IN, LineBreak_HY, LineBreak_BA, LineBreak_BB, LineBreak_B2,
    LineBreak_ZW, LineBreak_CM, LineBreak_WJ, LineBreak_H2, LineBreak_H3,
    LineBreak_JL, LineBreak_JV, LineBreak_JT, LineBreak_RI, LineBreak_CB,
    LineBreak_EB, LineBreak_EM, LineBreak_ZWJ,
    LineBreak_SA, LineBreak_SG, LineBreak_SP,
    LineBreak_CR, LineBreak_LF, LineBreak_BK,

    NumLineBreakClasses
};

enum class IdnaStatus : unsigned int {
    Disallowed,
    Valid,
    Ignored,
    Mapped,
    Deviation
};

GraphemeBreakClass FASTCALL graphemeBreakClass(char32_t ucs4) noexcept;
inline GraphemeBreakClass graphemeBreakClass(QChar ch) noexcept
{
    return graphemeBreakClass(ch.unicode());
}

WordBreakClass FASTCALL wordBreakClass(char32_t ucs4) noexcept;
inline WordBreakClass wordBreakClass(QChar ch) noexcept
{
    return wordBreakClass(ch.unicode());
}

SentenceBreakClass FASTCALL sentenceBreakClass(char32_t ucs4) noexcept;
inline SentenceBreakClass sentenceBreakClass(QChar ch) noexcept
{
    return sentenceBreakClass(ch.unicode());
}

LineBreakClass FASTCALL lineBreakClass(char32_t ucs4) noexcept;
inline LineBreakClass lineBreakClass(QChar ch) noexcept
{
    return lineBreakClass(ch.unicode());
}

IdnaStatus FASTCALL idnaStatus(char32_t ucs4) noexcept;
inline IdnaStatus idnaStatus(QChar ch) noexcept
{
    return idnaStatus(ch.unicode());
}

//QStringView FASTCALL idnaMapping(char32_t usc4) noexcept;
//inline QStringView idnaMapping(QChar ch) noexcept
//{
//    return idnaMapping(ch.unicode());
//}

EastAsianWidth FASTCALL eastAsianWidth(char32_t ucs4) noexcept;
inline EastAsianWidth eastAsianWidth(QChar ch) noexcept
{
    return eastAsianWidth(ch.unicode());
}

__BASE_STRING_NAMESPACE_END

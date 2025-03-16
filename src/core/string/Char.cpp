#include <base/core/string/Char.h>
#include <base/core/string/String.h>

#include <src/core/string/UnicodeTables.h>
#include <src/core/string/UnicodeTables.cpp>

#define FLAG(x) (1 << (x))


__BASE_STRING_NAMESPACE_BEGIN


bool Char::isPrint(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Other_Control) |
        FLAG(Other_Format) |
        FLAG(Other_Surrogate) |
        FLAG(Other_PrivateUse) |
        FLAG(Other_NotAssigned);
    return !(FLAG(_GetProp(ucs4)->category) & test);
}

bool FASTCALL Char::isSpace_helper(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Separator_Space) |
        FLAG(Separator_Line) |
        FLAG(Separator_Paragraph);
    return FLAG(_GetProp(ucs4)->category) & test;
}

bool Char::isMark(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Mark_NonSpacing) |
        FLAG(Mark_SpacingCombining) |
        FLAG(Mark_Enclosing);
    return FLAG(_GetProp(ucs4)->category) & test;
}

bool Char::isPunct(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Punctuation_Connector) |
        FLAG(Punctuation_Dash) |
        FLAG(Punctuation_Open) |
        FLAG(Punctuation_Close) |
        FLAG(Punctuation_InitialQuote) |
        FLAG(Punctuation_FinalQuote) |
        FLAG(Punctuation_Other);
    return FLAG(_GetProp(ucs4)->category) & test;
}

bool Char::isSymbol(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Symbol_Math) |
        FLAG(Symbol_Currency) |
        FLAG(Symbol_Modifier) |
        FLAG(Symbol_Other);
    return FLAG(_GetProp(ucs4)->category) & test;
}


bool FASTCALL Char::isLetter_helper(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Letter_Uppercase) |
        FLAG(Letter_Lowercase) |
        FLAG(Letter_Titlecase) |
        FLAG(Letter_Modifier) |
        FLAG(Letter_Other);
    return FLAG(_GetProp(ucs4)->category) & test;
}


bool FASTCALL Char::isNumber_helper(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Number_DecimalDigit) |
        FLAG(Number_Letter) |
        FLAG(Number_Other);
    return FLAG(_GetProp(ucs4)->category) & test;
}


bool FASTCALL Char::isLetterOrNumber_helper(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    const int test = FLAG(Letter_Uppercase) |
        FLAG(Letter_Lowercase) |
        FLAG(Letter_Titlecase) |
        FLAG(Letter_Modifier) |
        FLAG(Letter_Other) |
        FLAG(Number_DecimalDigit) |
        FLAG(Number_Letter) |
        FLAG(Number_Other);
    return FLAG(_GetProp(ucs4)->category) & test;
}

int Char::digitValue(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return -1;
    return _GetProp(ucs4)->digitValue;
}

Char::Category Char::category(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return Char::Other_NotAssigned;
    return (Char::Category)_GetProp(ucs4)->category;
}

bool Char::hasMirrored(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return false;
    return _GetProp(ucs4)->mirrorDiff != 0;
}

char32_t Char::mirroredChar(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return ucs4;
    return ucs4 + _GetProp(ucs4)->mirrorDiff;
}

// Constants for Hangul (de)composition, see UAX #15:
static constexpr char32_t Hangul_SBase = 0xac00;
static constexpr char32_t Hangul_LBase = 0x1100;
static constexpr char32_t Hangul_VBase = 0x1161;
static constexpr char32_t Hangul_TBase = 0x11a7;
static constexpr uint32 Hangul_LCount = 19;
static constexpr uint32 Hangul_VCount = 21;
static constexpr uint32 Hangul_TCount = 28;
static constexpr uint32 Hangul_NCount = Hangul_VCount * Hangul_TCount;
static constexpr uint32 Hangul_SCount = Hangul_LCount * Hangul_NCount;

// buffer has to have a length of 3. It's needed for Hangul decomposition
static const Char* FASTCALL decompositionHelper(
    char32_t ucs4, sizetype* length, Char::Decomposition* tag, Char* buffer)
{
    if (ucs4 >= Hangul_SBase && ucs4 < Hangul_SBase + Hangul_SCount) {
        // compute Hangul syllable decomposition as per UAX #15
        const char32_t SIndex = ucs4 - Hangul_SBase;
        buffer[0] = Char(Hangul_LBase + SIndex / Hangul_NCount); // L
        buffer[1] = Char(Hangul_VBase + (SIndex % Hangul_NCount) / Hangul_TCount); // V
        buffer[2] = Char(Hangul_TBase + SIndex % Hangul_TCount); // T
        *length = buffer[2].unicode() == Hangul_TBase ? 2 : 3;
        *tag = Char::Canonical;
        return buffer;
    }

    const unsigned short index = GET_DECOMPOSITION_INDEX(ucs4);
    if (index == 0xffff) {
        *length = 0;
        *tag = Char::NoDecomposition;
        return nullptr;
    }

    const unsigned short* decomposition = uc_decomposition_map + index;
    *tag = Char::Decomposition((*decomposition) & 0xff);
    *length = (*decomposition) >> 8;
    return reinterpret_cast<const Char*>(decomposition + 1);
}

unsigned char Char::combiningClass(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return 0;
    return (unsigned char)_GetProp(ucs4)->combiningClass;
}

static auto fullConvertCase(char32_t uc, Case which) noexcept
{
    struct R {
        char16_t chars[MaxSpecialCaseLength + 1];
        int8 sz;

        // iterable
        auto begin() const { return chars; }
        auto end() const { return chars + sz; }
        // QStringView-compatible
        auto data() const { return chars; }
        auto size() const { return sz; }
    } result;
    Assert(uc <= Char::LastValidCodePoint);

    auto pp = result.chars;

    const auto fold = _GetProp(uc)->cases[which];
    const auto caseDiff = fold.diff;

    if (UNLIKELY(fold.special)) {
        const auto* specialCase = specialCaseMap + caseDiff;
        auto length = *specialCase++;
        while (length--)
            *pp++ = *specialCase++;
    }
    else {
        // so far, case conversion never changes planes (guaranteed by the qunicodetables generator)
        for (char16_t c : Char::fromUcs4(uc + caseDiff))
            *pp++ = c;
    }
    result.sz = pp - result.chars;
    return result;
}

template <typename T>
DECL_CONST_FUNCTION static inline T convertCase_helper(T uc, Case which) noexcept
{
    const auto fold = _GetProp(uc)->cases[which];

    if (UNLIKELY(fold.special)) {
        const ushort* specialCase = specialCaseMap + fold.diff;
        // so far, there are no special cases beyond BMP (guaranteed by the qunicodetables generator)
        return *specialCase == 1 ? specialCase[1] : uc;
    }

    return uc + fold.diff;
}

char32_t Char::toLower(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return ucs4;
    return convertCase_helper(ucs4, LowerCase);
}

char32_t Char::toUpper(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return ucs4;
    return convertCase_helper(ucs4, UpperCase);
}

char32_t Char::toTitleCase(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return ucs4;
    return convertCase_helper(ucs4, TitleCase);
}

static inline char32_t foldCase(const char16_t* ch, const char16_t* start)
{
    char32_t ucs4 = *ch;
    if (Char::isLowSurrogate(ucs4) && ch > start && Char::isHighSurrogate(*(ch - 1)))
        ucs4 = Char::surrogateToUcs4(*(ch - 1), ucs4);
    return convertCase_helper(ucs4, CaseFold);
}

static inline char32_t foldCase(char32_t ch, char32_t& last) noexcept
{
    char32_t ucs4 = ch;
    if (Char::isLowSurrogate(ucs4) && Char::isHighSurrogate(last))
        ucs4 = Char::surrogateToUcs4(last, ucs4);
    last = ch;
    return convertCase_helper(ucs4, CaseFold);
}

static inline char16_t foldCase(char16_t ch) noexcept
{
    return convertCase_helper(ch, CaseFold);
}

static inline Char foldCase(Char ch) noexcept
{
    return Char(foldCase(ch.unicode()));
}

char32_t Char::toCaseFolded(char32_t ucs4) noexcept
{
    if (ucs4 > LastValidCodePoint)
        return ucs4;
    return convertCase_helper(ucs4, CaseFold);
}


struct UCS2Pair {
    ushort u1;
    ushort u2;
};

inline bool operator<(const UCS2Pair& ligature1, const UCS2Pair& ligature2)
{
    return ligature1.u1 < ligature2.u1;
}
inline bool operator<(ushort u1, const UCS2Pair& ligature)
{
    return u1 < ligature.u1;
}
inline bool operator<(const UCS2Pair& ligature, ushort u1)
{
    return ligature.u1 < u1;
}

struct UCS2SurrogatePair {
    UCS2Pair p1;
    UCS2Pair p2;
};

inline bool operator<(const UCS2SurrogatePair& ligature1, const UCS2SurrogatePair& ligature2)
{
    return Char::surrogateToUcs4(ligature1.p1.u1, ligature1.p1.u2) < Char::surrogateToUcs4(ligature2.p1.u1, ligature2.p1.u2);
}
inline bool operator<(char32_t u1, const UCS2SurrogatePair& ligature)
{
    return u1 < Char::surrogateToUcs4(ligature.p1.u1, ligature.p1.u2);
}
inline bool operator<(const UCS2SurrogatePair& ligature, char32_t u1)
{
    return Char::surrogateToUcs4(ligature.p1.u1, ligature.p1.u2) < u1;
}

static char32_t inline ligatureHelper(char32_t u1, char32_t u2)
{
    if (u1 >= Hangul_LBase && u1 < Hangul_SBase + Hangul_SCount) {
        // compute Hangul syllable composition as per UAX #15
        // hangul L-V pair
        const char32_t LIndex = u1 - Hangul_LBase;
        if (LIndex < Hangul_LCount) {
            const char32_t VIndex = u2 - Hangul_VBase;
            if (VIndex < Hangul_VCount)
                return Hangul_SBase + (LIndex * Hangul_VCount + VIndex) * Hangul_TCount;
        }
        // hangul LV-T pair
        const char32_t SIndex = u1 - Hangul_SBase;
        if (SIndex < Hangul_SCount && (SIndex % Hangul_TCount) == 0) {
            const char32_t TIndex = u2 - Hangul_TBase;
            if (TIndex < Hangul_TCount && TIndex)
                return u1 + TIndex;
        }
    }

    const unsigned short index = GET_LIGATURE_INDEX(u2);
    if (index == 0xffff)
        return 0;
    const unsigned short* ligatures = uc_ligature_map + index;
    ushort length = *ligatures++;
    if (Char::requiresSurrogates(u1)) {
        const UCS2SurrogatePair* data = reinterpret_cast<const UCS2SurrogatePair*>(ligatures);
        const UCS2SurrogatePair* r = std::lower_bound(data, data + length, u1);
        if (r != data + length && Char::surrogateToUcs4(r->p1.u1, r->p1.u2) == u1)
            return Char::surrogateToUcs4(r->p2.u1, r->p2.u2);
    }
    else {
        const UCS2Pair* data = reinterpret_cast<const UCS2Pair*>(ligatures);
        const UCS2Pair* r = std::lower_bound(data, data + length, ushort(u1));
        if (r != data + length && r->u1 == ushort(u1))
            return r->u2;
    }

    return 0;
}

__BASE_STRING_NAMESPACE_END
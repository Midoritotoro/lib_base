#include <base/string/Char.h>


#include <algorithm>

#define FLAG(x) (1 << (x))

namespace base::string {

    bool Char::isPrint(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return false;
        const int test = FLAG(Other_Control) |
            FLAG(Other_Format) |
            FLAG(Other_Surrogate) |
            FLAG(Other_PrivateUse) |
            FLAG(Other_NotAssigned);
        return !(FLAG(qGetProp(ucs4)->category) & test);
    }

    bool FASTCALL Char::isSpace_helper(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return false;
        const int test = FLAG(Separator_Space) |
            FLAG(Separator_Line) |
            FLAG(Separator_Paragraph);
        return FLAG(qGetProp(ucs4)->category) & test;
    }

    bool Char::isMark(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return false;
        const int test = FLAG(Mark_NonSpacing) |
            FLAG(Mark_SpacingCombining) |
            FLAG(Mark_Enclosing);
        return FLAG(qGetProp(ucs4)->category) & test;
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
        return FLAG(qGetProp(ucs4)->category) & test;
    }

    bool Char::isSymbol(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return false;
        const int test = FLAG(Symbol_Math) |
            FLAG(Symbol_Currency) |
            FLAG(Symbol_Modifier) |
            FLAG(Symbol_Other);
        return FLAG(qGetProp(ucs4)->category) & test;
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
        return FLAG(qGetProp(ucs4)->category) & test;
    }

    bool FASTCALL Char::isNumber_helper(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return false;
        const int test = FLAG(Number_DecimalDigit) |
            FLAG(Number_Letter) |
            FLAG(Number_Other);
        return FLAG(qGetProp(ucs4)->category) & test;
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
        return FLAG(qGetProp(ucs4)->category) & test;
    }

    int Char::digitValue(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return -1;
        return qGetProp(ucs4)->digitValue;
    }

    Char::Category Char::category(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return Char::Other_NotAssigned;
        return (Char::Category)qGetProp(ucs4)->category;
    }

    Char::Direction Char::direction(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return Char::DirL;
        return (Char::Direction)qGetProp(ucs4)->direction;
    }


    Char::JoiningType Char::joiningType(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return Char::Joining_None;
        return Char::JoiningType(qGetProp(ucs4)->joining);
    }


    bool Char::hasMirrored(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return false;
        return qGetProp(ucs4)->mirrorDiff != 0;
    }


    char32_t Char::mirroredChar(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return ucs4;
        return ucs4 + qGetProp(ucs4)->mirrorDiff;
    }

    // Constants for Hangul (de)composition, see UAX #15:
    static constexpr char32_t Hangul_SBase = 0xac00;
    static constexpr char32_t Hangul_LBase = 0x1100;
    static constexpr char32_t Hangul_VBase = 0x1161;
    static constexpr char32_t Hangul_TBase = 0x11a7;
    static constexpr quint32 Hangul_LCount = 19;
    static constexpr quint32 Hangul_VCount = 21;
    static constexpr quint32 Hangul_TCount = 28;
    static constexpr quint32 Hangul_NCount = Hangul_VCount * Hangul_TCount;
    static constexpr quint32 Hangul_SCount = Hangul_LCount * Hangul_NCount;

    // buffer has to have a length of 3. It's needed for Hangul decomposition
    static const Char* FASTCALL decompositionHelper(
        char32_t ucs4, qsizetype* length, Char::Decomposition* tag, Char* buffer)
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


    QString Char::decomposition() const
    {
        return Char::decomposition(ucs);
    }


    QString Char::decomposition(char32_t ucs4)
    {
        Char buffer[3];
        qsizetype length;
        Char::Decomposition tag;
        const Char* d = decompositionHelper(ucs4, &length, &tag, buffer);
        return QString(d, length);
    }


    Char::Decomposition Char::decompositionTag(char32_t ucs4) noexcept
    {
        if (ucs4 >= Hangul_SBase && ucs4 < Hangul_SBase + Hangul_SCount)
            return Char::Canonical;
        const unsigned short index = GET_DECOMPOSITION_INDEX(ucs4);
        if (index == 0xffff)
            return Char::NoDecomposition;
        return (Char::Decomposition)(uc_decomposition_map[index] & 0xff);
    }

    unsigned char Char::combiningClass(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return 0;
        return (unsigned char)qGetProp(ucs4)->combiningClass;
    }

    Char::Script Char::script(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return Char::Script_Unknown;
        return (Char::Script)qGetProp(ucs4)->script;
    }

    Char::UnicodeVersion Char::unicodeVersion(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return Char::Unicode_Unassigned;
        return (Char::UnicodeVersion)qGetProp(ucs4)->unicodeVersion;
    }


    Char::UnicodeVersion Char::currentUnicodeVersion() noexcept
    {
        return UNICODE_DATA_VERSION;
    }

    static auto fullConvertCase(char32_t uc, QUnicodeTables::Case which) noexcept
    {
        struct R {
            char16_t chars[MaxSpecialCaseLength + 1];
            qint8 sz;

            // iterable
            auto begin() const { return chars; }
            auto end() const { return chars + sz; }
            // QStringView-compatible
            auto data() const { return chars; }
            auto size() const { return sz; }
        } result;
        Q_ASSERT(uc <= Char::LastValidCodePoint);

        auto pp = result.chars;

        const auto fold = qGetProp(uc)->cases[which];
        const auto caseDiff = fold.diff;

        if (Q_UNLIKELY(fold.special)) {
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
    Q_DECL_CONST_FUNCTION static inline T convertCase_helper(T uc, QUnicodeTables::Case which) noexcept
    {
        const auto fold = qGetProp(uc)->cases[which];

        if (Q_UNLIKELY(fold.special)) {
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
        return convertCase_helper(ucs4, QUnicodeTables::LowerCase);
    }


    char32_t Char::toUpper(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return ucs4;
        return convertCase_helper(ucs4, QUnicodeTables::UpperCase);
    }


    char32_t Char::toTitleCase(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return ucs4;
        return convertCase_helper(ucs4, QUnicodeTables::TitleCase);
    }

    static inline char32_t foldCase(const char16_t* ch, const char16_t* start)
    {
        char32_t ucs4 = *ch;
        if (Char::isLowSurrogate(ucs4) && ch > start && Char::isHighSurrogate(*(ch - 1)))
            ucs4 = Char::surrogateToUcs4(*(ch - 1), ucs4);
        return convertCase_helper(ucs4, QUnicodeTables::CaseFold);
    }

    static inline char32_t foldCase(char32_t ch, char32_t& last) noexcept
    {
        char32_t ucs4 = ch;
        if (Char::isLowSurrogate(ucs4) && Char::isHighSurrogate(last))
            ucs4 = Char::surrogateToUcs4(last, ucs4);
        last = ch;
        return convertCase_helper(ucs4, QUnicodeTables::CaseFold);
    }

    static inline char16_t foldCase(char16_t ch) noexcept
    {
        return convertCase_helper(ch, QUnicodeTables::CaseFold);
    }

    static inline Char foldCase(Char ch) noexcept
    {
        return Char(foldCase(ch.unicode()));
    }

    char32_t Char::toCaseFolded(char32_t ucs4) noexcept
    {
        if (ucs4 > LastValidCodePoint)
            return ucs4;
        return convertCase_helper(ucs4, QUnicodeTables::CaseFold);
    }



    static void decomposeHelper(QString* str, bool canonical, Char::UnicodeVersion version, qsizetype from)
    {
        qsizetype length;
        Char::Decomposition tag;
        Char buffer[3];

        QString& s = *str;

        const unsigned short* utf16 = reinterpret_cast<unsigned short*>(s.data());
        const unsigned short* uc = utf16 + s.size();
        while (uc != utf16 + from) {
            char32_t ucs4 = *(--uc);
            if (Char(ucs4).isLowSurrogate() && uc != utf16) {
                ushort high = *(uc - 1);
                if (Char(high).isHighSurrogate()) {
                    --uc;
                    ucs4 = Char::surrogateToUcs4(high, ucs4);
                }
            }

            if (Char::unicodeVersion(ucs4) > version)
                continue;

            const Char* d = decompositionHelper(ucs4, &length, &tag, buffer);
            if (!d || (canonical && tag != Char::Canonical))
                continue;

            qsizetype pos = uc - utf16;
            s.replace(pos, Char::requiresSurrogates(ucs4) ? 2 : 1, d, length);
            // since the replace invalidates the pointers and we do decomposition recursive
            utf16 = reinterpret_cast<unsigned short*>(s.data());
            uc = utf16 + pos + length;
        }
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

    static void composeHelper(QString* str, Char::UnicodeVersion version, qsizetype from)
    {
        QString& s = *str;

        if (from < 0 || s.size() - from < 2)
            return;

        char32_t stcode = 0; // starter code point
        qsizetype starter = -1; // starter position
        qsizetype next = -1; // to prevent i == next
        int lastCombining = 255; // to prevent combining > lastCombining

        qsizetype pos = from;
        while (pos < s.size()) {
            qsizetype i = pos;
            char32_t uc = s.at(pos).unicode();
            if (Char(uc).isHighSurrogate() && pos < s.size() - 1) {
                ushort low = s.at(pos + 1).unicode();
                if (Char(low).isLowSurrogate()) {
                    uc = Char::surrogateToUcs4(uc, low);
                    ++pos;
                }
            }

            const QUnicodeTables::Properties* p = qGetProp(uc);
            if (p->unicodeVersion > version) {
                starter = -1;
                next = -1; // to prevent i == next
                lastCombining = 255; // to prevent combining > lastCombining
                ++pos;
                continue;
            }

            int combining = p->combiningClass;
            if ((i == next || combining > lastCombining) && starter >= from) {
                // allowed to form ligature with S
                char32_t ligature = ligatureHelper(stcode, uc);
                if (ligature) {
                    stcode = ligature;
                    Char* d = s.data();
                    // ligatureHelper() never changes planes
                    qsizetype j = 0;
                    for (Char ch : Char::fromUcs4(ligature))
                        d[starter + j++] = ch;
                    s.remove(i, j);
                    continue;
                }
            }
            if (combining == 0) {
                starter = i;
                stcode = uc;
                next = pos + 1;
            }
            lastCombining = combining;

            ++pos;
        }
    }


    static void canonicalOrderHelper(QString* str, Char::UnicodeVersion version, qsizetype from)
    {
        QString& s = *str;
        const qsizetype l = s.size() - 1;

        char32_t u1, u2;
        char16_t c1, c2;

        qsizetype pos = from;
        while (pos < l) {
            qsizetype p2 = pos + 1;
            u1 = s.at(pos).unicode();
            if (Char::isHighSurrogate(u1)) {
                const char16_t low = s.at(p2).unicode();
                if (Char::isLowSurrogate(low)) {
                    u1 = Char::surrogateToUcs4(u1, low);
                    if (p2 >= l)
                        break;
                    ++p2;
                }
            }
            c1 = 0;

        advance:
            u2 = s.at(p2).unicode();
            if (Char::isHighSurrogate(u2) && p2 < l) {
                const char16_t low = s.at(p2 + 1).unicode();
                if (Char::isLowSurrogate(low)) {
                    u2 = Char::surrogateToUcs4(u2, low);
                    ++p2;
                }
            }

            c2 = 0;
            {
                const QUnicodeTables::Properties* p = qGetProp(u2);
                if (p->unicodeVersion <= version)
                    c2 = p->combiningClass;
            }
            if (c2 == 0) {
                pos = p2 + 1;
                continue;
            }

            if (c1 == 0) {
                const QUnicodeTables::Properties* p = qGetProp(u1);
                if (p->unicodeVersion <= version)
                    c1 = p->combiningClass;
            }

            if (c1 > c2) {
                Char* uc = s.data();
                qsizetype p = pos;
                // exchange characters
                for (Char ch : Char::fromUcs4(u2))
                    uc[p++] = ch;
                for (Char ch : Char::fromUcs4(u1))
                    uc[p++] = ch;
                if (pos > 0)
                    --pos;
                if (pos > 0 && s.at(pos).isLowSurrogate())
                    --pos;
            }
            else {
                ++pos;
                if (Char::requiresSurrogates(u1))
                    ++pos;

                u1 = u2;
                c1 = c2; // != 0
                p2 = pos + 1;
                if (Char::requiresSurrogates(u1))
                    ++p2;
                if (p2 > l)
                    break;

                goto advance;
            }
        }
    }

    // returns true if the text is in a desired Normalization Form already; false otherwise.
    // sets lastStable to the position of the last stable code point
    static bool normalizationQuickCheckHelper(QString* str, QString::NormalizationForm mode, qsizetype from, qsizetype* lastStable)
    {
        static_assert(QString::NormalizationForm_D == 0);
        static_assert(QString::NormalizationForm_C == 1);
        static_assert(QString::NormalizationForm_KD == 2);
        static_assert(QString::NormalizationForm_KC == 3);

        enum { NFQC_YES = 0, NFQC_NO = 1, NFQC_MAYBE = 3 };

        const auto* string = reinterpret_cast<const char16_t*>(str->constData());
        qsizetype length = str->size();

        // this avoids one out of bounds check in the loop
        while (length > from && Char::isHighSurrogate(string[length - 1]))
            --length;

        uchar lastCombining = 0;
        for (qsizetype i = from; i < length; ++i) {
            qsizetype pos = i;
            char32_t uc = string[i];
            if (uc < 0x80) {
                // ASCII characters are stable code points
                lastCombining = 0;
                *lastStable = pos;
                continue;
            }

            if (Char::isHighSurrogate(uc)) {
                ushort low = string[i + 1];
                if (!Char::isLowSurrogate(low)) {
                    // treat surrogate like stable code point
                    lastCombining = 0;
                    *lastStable = pos;
                    continue;
                }
                ++i;
                uc = Char::surrogateToUcs4(uc, low);
            }

            const QUnicodeTables::Properties* p = qGetProp(uc);

            if (p->combiningClass < lastCombining && p->combiningClass > 0)
                return false;

            const uchar check = (p->nfQuickCheck >> (mode << 1)) & 0x03;
            if (check != NFQC_YES)
                return false; // ### can we quick check NFQC_MAYBE ?

            lastCombining = p->combiningClass;
            if (lastCombining == 0)
                *lastStable = pos;
        }

        if (length != str->size()) // low surrogate parts at the end of text
            *lastStable = str->size() - 1;

        return true;
    }

} // namespace base::string
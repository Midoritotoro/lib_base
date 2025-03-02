#include <base/io/ByteArray.h>

#include <base/system/Platform.h>
#include <base/io/ByteArrayPrivate.h>

#include <base/io/ByteArrayAlgorithms.h>
#include <base/utility/Math.h>

#include <base/system/Endian.h>
#include <base/io/ByteArrayView.h>

#include <base/io/ByteArrayMatcher.h>

#include <zconf.h>
#include <zlib.h>

#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include <algorithm>
#include <qxpfunctional.h>

// / /  // 
#include <qvarlengtharray.h>
#include <qlocale.h>

#include <qhash.h>
#include <private/qlocale_data_p.h>
#include <private/qlocale_tools_p.h>

#include <base/io/MiscUtils.h>


template <typename StringType> struct StringAlgorithms
{
    typedef typename StringType::value_type Char;
    typedef typename StringType::size_type size_type;
    typedef typename std::remove_cv<StringType>::type NakedStringType;
    static const bool isConst = std::is_const<StringType>::value;

    static inline bool isSpace(char ch) { return ascii_isspace(ch); }
    static inline bool isSpace(QChar ch) { return ch.isSpace(); }

    // Surrogate pairs are not handled in either of the functions below. That is
    // not a problem because there are no space characters (Zs, Zl, Zp) outside the
    // Basic Multilingual Plane.

    static inline StringType trimmed_helper_inplace(NakedStringType& str, const Char* begin, const Char* end)
    {
        // in-place trimming:
        Char* data = const_cast<Char*>(str.cbegin());
        if (begin != data)
            memmove(data, begin, (end - begin) * sizeof(Char));
        str.resize(end - begin);
        return std::move(str);
    }

    static inline StringType trimmed_helper_inplace(const NakedStringType&, const Char*, const Char*)
    {
        // can't happen
        AssertUnreachable();
        return {};
    }

    struct TrimPositions {
        const Char* begin;
        const Char* end;
    };
    // Returns {begin, end} where:
    // - "begin" refers to the first non-space character
    // - if there is a sequence of one or more space chacaters at the end,
    //   "end" refers to the first character in that sequence, otherwise
    //   "end" is str.cend()
    [[nodiscard]] static TrimPositions trimmed_helper_positions(const StringType& str)
    {
        const Char* begin = str.cbegin();
        const Char* end = str.cend();
        // skip white space from end
        while (begin < end && isSpace(end[-1]))
            --end;
        // skip white space from start
        while (begin < end && isSpace(*begin))
            begin++;
        return { begin, end };
    }

    static inline StringType trimmed_helper(StringType& str)
    {
        const auto [begin, end] = trimmed_helper_positions(str);
        if (begin == str.cbegin() && end == str.cend())
            return str;
        if (!isConst && str.isDetached())
            return trimmed_helper_inplace(str, begin, end);
        return StringType(begin, end - begin);
    }

    static inline StringType simplified_helper(StringType& str)
    {
        if (str.isEmpty())
            return str;
        const Char* src = str.cbegin();
        const Char* end = str.cend();
        NakedStringType result = isConst || !str.isDetached() ?
            StringType(str.size(), base::io::Initialization::Uninitialized) :
            std::move(str);

        Char* dst = const_cast<Char*>(result.cbegin());
        Char* ptr = dst;
        bool unmodified = true;
        forever{
            while (src != end && isSpace(*src))
                ++src;
            while (src != end && !isSpace(*src))
                *ptr++ = *src++;
            if (src == end)
                break;
            if (*src != 0x0020)
                unmodified = false;
            *ptr++ = 0x0020;
        }
            if (ptr != dst && ptr[-1] == 0x0020)
                --ptr;

        qsizetype newlen = ptr - dst;
        if (isConst && newlen == str.size() && unmodified) {
            // nothing happened, return the original
            return str;
        }
        result.resize(newlen);
        return result;
    }
};


struct ValidUtf8Result {
    bool isValidUtf8;
    bool isValidAscii;
};


static inline const uchar* simdFindNonAscii(const uchar* src, const uchar* end, const uchar*& nextAscii)
{
#ifdef __AVX2__
    // do 32 characters at a time
    // (this is similar to simdTestMask in qstring.cpp)
    const __m256i mask = _mm256_set1_epi8(char(0x80));
    for (; end - src >= 32; src += 32) {
        __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src));
        if (_mm256_testz_si256(mask, data))
            continue;

        uint n = _mm256_movemask_epi8(data);
        Assert(n);

        // find the next probable ASCII character
        // we don't want to load 32 bytes again in this loop if we know there are non-ASCII
        // characters still coming
        nextAscii = src + qBitScanReverse(n) + 1;

        // return the non-ASCII character
        return src + qCountTrailingZeroBits(n);
    }
#endif

    // do sixteen characters at a time
    for (; end - src >= 16; src += 16) {
        __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(src));

        // check if everything is ASCII
        // movemask extracts the high bit of every byte, so n is non-zero if something isn't ASCII
        uint n = _mm_movemask_epi8(data);
        if (!n)
            continue;

        // find the next probable ASCII character
        // we don't want to load 16 bytes again in this loop if we know there are non-ASCII
        // characters still coming
        nextAscii = src + base::__BitScanReverse(n) + 1;

        // return the non-ASCII character
        return src + base::CountTrailingZeroBits(n);
    }

    // do four characters at a time
    for (; end - src >= 4; src += 4) {
        base::uint32 data = base::FromUnaligned<base::uint32>(src);
        data &= 0x80808080U;
        if (!data)
            continue;

        // We don't try to guess which of the three bytes is ASCII and which
        // one isn't. The chance that at least two of them are non-ASCII is
        // better than 75%.
        nextAscii = src;
        return src;
    }
    nextAscii = end;
    return src;
}


template <typename Traits, typename OutputPtr, typename InputPtr> inline
int toUtf8(char16_t u, OutputPtr& dst, InputPtr& src, InputPtr end)
{
    if (!Traits::skipAsciiHandling && u < 0x80) {
        // U+0000 to U+007F (US-ASCII) - one byte
        Traits::appendByte(dst, uchar(u));
        return 0;
    }
    else if (u < 0x0800) {
        // U+0080 to U+07FF - two bytes
        // first of two bytes
        Traits::appendByte(dst, 0xc0 | uchar(u >> 6));
    }
    else {
        if (!QChar::isSurrogate(u)) {
            // U+0800 to U+FFFF (except U+D800-U+DFFF) - three bytes
            if (!Traits::allowNonCharacters && QChar::isNonCharacter(u))
                return Traits::Error;

            // first of three bytes
            Traits::appendByte(dst, 0xe0 | uchar(u >> 12));
        }
        else {
            // U+10000 to U+10FFFF - four bytes
            // need to get one extra codepoint
            if (Traits::availableUtf16(src, end) == 0)
                return Traits::EndOfString;

            char16_t low = Traits::peekUtf16(src);
            if (!QChar::isHighSurrogate(u))
                return Traits::Error;
            if (!QChar::isLowSurrogate(low))
                return Traits::Error;

            Traits::advanceUtf16(src);
            char32_t ucs4 = QChar::surrogateToUcs4(u, low);

            if (!Traits::allowNonCharacters && QChar::isNonCharacter(ucs4))
                return Traits::Error;

            // first byte
            Traits::appendByte(dst, 0xf0 | (uchar(ucs4 >> 18) & 0xf));

            // second of four bytes
            Traits::appendByte(dst, 0x80 | (uchar(ucs4 >> 12) & 0x3f));

            // for the rest of the bytes
            u = char16_t(ucs4);
        }

        // second to last byte
        Traits::appendByte(dst, 0x80 | (uchar(u >> 6) & 0x3f));
    }

    // last byte
    Traits::appendByte(dst, 0x80 | (u & 0x3f));
    return 0;
}

inline bool isContinuationByte(uchar b)
{
    return (b & 0xc0) == 0x80;
}


template <typename Traits, typename OutputPtr, typename InputPtr> inline
qsizetype fromUtf8(uchar b, OutputPtr& dst, InputPtr& src, InputPtr end)
{
    qsizetype charsNeeded;
    char32_t min_uc;
    char32_t uc;

    if (!Traits::skipAsciiHandling && b < 0x80) {
        // US-ASCII
        Traits::appendUtf16(dst, b);
        return 1;
    }

    if (!Traits::isTrusted && Q_UNLIKELY(b <= 0xC1)) {
        // an UTF-8 first character must be at least 0xC0
        // however, all 0xC0 and 0xC1 first bytes can only produce overlong sequences
        return Traits::Error;
    }
    else if (b < 0xe0) {
        charsNeeded = 2;
        min_uc = 0x80;
        uc = b & 0x1f;
    }
    else if (b < 0xf0) {
        charsNeeded = 3;
        min_uc = 0x800;
        uc = b & 0x0f;
    }
    else if (b < 0xf5) {
        charsNeeded = 4;
        min_uc = 0x10000;
        uc = b & 0x07;
    }
    else {
        // the last Unicode character is U+10FFFF
        // it's encoded in UTF-8 as "\xF4\x8F\xBF\xBF"
        // therefore, a byte higher than 0xF4 is not the UTF-8 first byte
        return Traits::Error;
    }

    qptrdiff bytesAvailable = Traits::availableBytes(src, end);
    if (Q_UNLIKELY(bytesAvailable < charsNeeded - 1)) {
        // it's possible that we have an error instead of just unfinished bytes
        if (bytesAvailable > 0 && !isContinuationByte(Traits::peekByte(src, 0)))
            return Traits::Error;
        if (bytesAvailable > 1 && !isContinuationByte(Traits::peekByte(src, 1)))
            return Traits::Error;
        return Traits::EndOfString;
    }

    // first continuation character
    b = Traits::peekByte(src, 0);
    if (!isContinuationByte(b))
        return Traits::Error;
    uc <<= 6;
    uc |= b & 0x3f;

    if (charsNeeded > 2) {
        // second continuation character
        b = Traits::peekByte(src, 1);
        if (!isContinuationByte(b))
            return Traits::Error;
        uc <<= 6;
        uc |= b & 0x3f;

        if (charsNeeded > 3) {
            // third continuation character
            b = Traits::peekByte(src, 2);
            if (!isContinuationByte(b))
                return Traits::Error;
            uc <<= 6;
            uc |= b & 0x3f;
        }
    }

    // we've decoded something; safety-check it
    if (!Traits::isTrusted) {
        if (uc < min_uc)
            return Traits::Error;
        if (QChar::isSurrogate(uc) || uc > QChar::LastValidCodePoint)
            return Traits::Error;
        if (!Traits::allowNonCharacters && QChar::isNonCharacter(uc))
            return Traits::Error;
    }

    // write the UTF-16 sequence
    if (!QChar::requiresSurrogates(uc)) {
        // UTF-8 decoded and no surrogates are required
        // detach if necessary
        Traits::appendUtf16(dst, char16_t(uc));
    }
    else {
        // UTF-8 decoded to something that requires a surrogate pair
        Traits::appendUcs4(dst, uc);
    }

    Traits::advanceByte(src, charsNeeded - 1);
    return charsNeeded;
}

namespace base {
    struct Utf8BaseTraits
    {
        static const bool isTrusted = false;
        static const bool allowNonCharacters = true;
        static const bool skipAsciiHandling = false;
        static const int Error = -1;
        static const int EndOfString = -2;

        static void appendByte(uchar*& ptr, uchar b)
        {
            *ptr++ = b;
        }

        static void appendByte(char8_t*& ptr, char8_t b)
        {
            *ptr++ = b;
        }

        static uchar peekByte(const uchar* ptr, sizetype n = 0)
        {
            return ptr[n];
        }

        static uchar peekByte(const char8_t* ptr, sizetype n = 0)
        {
            return ptr[n];
        }

        static ptrdiff availableBytes(const uchar* ptr, const uchar* end)
        {
            return end - ptr;
        }

        static ptrdiff availableBytes(const char8_t* ptr, const char8_t* end)
        {
            return end - ptr;
        }

        static void advanceByte(const uchar*& ptr, sizetype n = 1)
        {
            ptr += n;
        }

        static void advanceByte(const char8_t*& ptr, sizetype n = 1)
        {
            ptr += n;
        }

        static void appendUtf16(char16_t*& ptr, char16_t uc)
        {
            *ptr++ = char16_t(uc);
        }

        static constexpr inline char16_t highSurrogate(char32_t ucs4) noexcept
        {
            return char16_t((ucs4 >> 10) + 0xd7c0);
        }
        static constexpr inline char16_t lowSurrogate(char32_t ucs4) noexcept
        {
            return char16_t(ucs4 % 0x400 + 0xdc00);
        }

        static void appendUcs4(char16_t*& ptr, char32_t uc)
        {
            appendUtf16(ptr, highSurrogate(uc));
            appendUtf16(ptr, lowSurrogate(uc));
        }

        static char16_t peekUtf16(const char16_t* ptr, sizetype n = 0) { return ptr[n]; }

        static ptrdiff availableUtf16(const char16_t* ptr, const char16_t* end)
        {
            return end - ptr;
        }

        static void advanceUtf16(const char16_t*& ptr, sizetype n = 1) { ptr += n; }

        static void appendUtf16(char32_t*& ptr, char16_t uc)
        {
            *ptr++ = char32_t(uc);
        }

        static void appendUcs4(char32_t*& ptr, char32_t uc)
        {
            *ptr++ = uc;
        }
    };

    struct Utf8BaseTraitsNoAscii : public Utf8BaseTraits
    {
        static const bool skipAsciiHandling = true;
    };

    struct Utf8NoOutputTraits : public Utf8BaseTraitsNoAscii
    {
        struct NoOutput {};
        static void appendUtf16(const NoOutput&, char16_t) {}
        static void appendUcs4(const NoOutput&, char32_t) {}
    };

} 
namespace base {
    ValidUtf8Result isValidUtf8(base::io::ByteArrayView in)
    {
        const uchar* src = reinterpret_cast<const uchar*>(in.data());
        const uchar* end = src + in.size();
        const uchar* nextAscii = src;
        bool isValidAscii = true;

        while (src < end) {
            if (src >= nextAscii)
                src = simdFindNonAscii(src, end, nextAscii);
            if (src == end)
                break;

            do {
                uchar b = *src++;
                if ((b & 0x80) == 0)
                    continue;

                isValidAscii = false;
                base::Utf8NoOutputTraits::NoOutput output;
                const qsizetype res = fromUtf8<base::Utf8NoOutputTraits>(b, output, src, end);
                if (res < 0) {
                    // decoding error
                    return { false, false };
                }
            } while (src < nextAscii);
        }

        return { true, isValidAscii };
    }
} 

int base::io::algorithms::compareMemory(ByteArrayView lhs, ByteArrayView rhs)
{
    if (!lhs.isNull() && !rhs.isNull()) {
        int ret = memcmp(lhs.data(), rhs.data(), qMin(lhs.size(), rhs.size()));
        if (ret != 0)
            return ret;
    }

    // they matched qMin(l1, l2) bytes
    // so the longer one is lexically after the shorter one
    return lhs.size() == rhs.size() ? 0 : lhs.size() > rhs.size() ? 1 : -1;
}

/*!
    \internal
*/
bool base::io::algorithms::isValidUtf8(ByteArrayView s) noexcept
{
    return base::isValidUtf8(s).isValidUtf8;
}


__BASE_IO_NAMESPACE_BEGIN

const char ByteArray::_empty = '\0';


sizetype FindByteArray(
    const char* haystack0, sizetype haystackLen, sizetype from,
    const char* needle0, sizetype needleLen);


char* qstrdup(const char* src)
{
    if (!src)
        return nullptr;
    char* dst = new char[strlen(src) + 1];
    return qstrcpy(dst, src);
}


char* qstrcpy(char* dst, const char* src)
{
    if (!src)
        return nullptr;
#ifdef Q_CC_MSVC
    const size_t len = strlen(src);
    // This is actually not secure!!! It will be fixed
    // properly in a later release!
    if (len >= 0 && strcpy_s(dst, len + 1, src) == 0)
        return dst;
    return nullptr;
#else
    return strcpy(dst, src);
#endif
}


char* qstrncpy(char* dst, const char* src, size_t len)
{
    if (dst && len > 0) {
        *dst = '\0';
        if (src)
            std::strncat(dst, src, len - 1);
    }
    return src ? dst : nullptr;
}

int qstrcmp(const char* str1, const char* str2)
{
    return (str1 && str2) ? strcmp(str1, str2)
        : (str1 ? 1 : (str2 ? -1 : 0));
}

int qstricmp(const char* str1, const char* str2)
{
    const uchar* s1 = reinterpret_cast<const uchar*>(str1);
    const uchar* s2 = reinterpret_cast<const uchar*>(str2);
    if (!s1)
        return s2 ? -1 : 0;
    if (!s2)
        return 1;

    enum { Incomplete = 256 };
    qptrdiff offset = 0;
    auto innerCompare = [=, &offset](qptrdiff max, bool unlimited) {
        max += offset;
        do {
            uchar c = s1[offset];
            if (int res = misc::caseCompareAscii(c, s2[offset]))
                return res;
            if (!c)
                return 0;
            ++offset;
        } while (unlimited || offset < max);
        return int(Incomplete);
        };

#if defined(__SSE4_1__) && !(defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer))
    enum { PageSize = 4096, PageMask = PageSize - 1 };
    const __m128i zero = _mm_setzero_si128();
    forever{
        // Calculate how many bytes we can load until we cross a page boundary
        // for either source. This isn't an exact calculation, just something
        // very quick.
        uintptr u1 = uintptr(s1 + offset);
        uintptr u2 = uintptr(s2 + offset);
        size_t n = PageSize - ((u1 | u2) & PageMask);

        ptrdiff maxoffset = offset + n;
        for (; offset + 16 <= maxoffset; offset += sizeof(__m128i)) {
            // load 16 bytes from either source
            __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i*>(s1 + offset));
            __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i*>(s2 + offset));

            // compare the two against each other
            __m128i cmp = _mm_cmpeq_epi8(a, b);

            // find NUL terminators too
            cmp = _mm_min_epu8(cmp, a);
            cmp = _mm_cmpeq_epi8(cmp, zero);

            // was there any difference or a NUL?
            uint mask = _mm_movemask_epi8(cmp);
            if (mask) {
                // yes, find out where
                uint start = CountTrailingZeroBits(mask);
                uint end = sizeof(mask) * 8 - qCountLeadingZeroBits(mask);
                Assert(end >= start);
                offset += start;
                n = end - start;
                break;
            }
        }

        // using SIMD could cause a page fault, so iterate byte by byte
        int res = innerCompare(n, false);
        if (res != Incomplete)
            return res;
    }
#endif

    return innerCompare(-1, true);
}

int qstrnicmp(const char* str1, const char* str2, size_t len)
{
    const uchar* s1 = reinterpret_cast<const uchar*>(str1);
    const uchar* s2 = reinterpret_cast<const uchar*>(str2);
    if (!s1 || !s2)
        return s1 ? 1 : (s2 ? -1 : 0);
    for (; len--; ++s1, ++s2) {
        const uchar c = *s1;
        if (int res = misc::caseCompareAscii(c, *s2))
            return res;
        if (!c)                                // strings are equal
            break;
    }
    return 0;
}




int qstrnicmp(const char* str1, sizetype len1, const char* str2, sizetype len2)
{
    Assert(len1 >= 0);
    Assert(len2 >= -1);

    const uchar* s1 = reinterpret_cast<const uchar*>(str1);
    const uchar* s2 = reinterpret_cast<const uchar*>(str2);

    if (!s1 || !len1) {
        if (len2 == 0)
            return 0;
        if (len2 == -1)
            return (!s2 || !*s2) ? 0 : -1;

        Assert(s2);
    }

    if (!s2)
        return len1 == 0 ? 0 : 1;

    if (len2 == -1) {
        // null-terminated str2
        sizetype i;
        for (i = 0; i < len1; ++i) {
            const uchar c = s2[i];
            if (!c)
                return 1;

            if (int res = misc::caseCompareAscii(s1[i], c))
                return res;
        }
        return s2[i] ? -1 : 0;
    }
    else {
        // not null-terminated
        const sizetype len = qMin(len1, len2);
        for (sizetype i = 0; i < len; ++i) {
            if (int res = misc::caseCompareAscii(s1[i], s2[i]))
                return res;
        }
        if (len1 == len2)
            return 0;
        return len1 < len2 ? -1 : 1;
    }
}

static const quint16 crc_tbl[16] = {
    0x0000, 0x1081, 0x2102, 0x3183,
    0x4204, 0x5285, 0x6306, 0x7387,
    0x8408, 0x9489, 0xa50a, 0xb58b,
    0xc60c, 0xd68d, 0xe70e, 0xf78f
};

uint16 qChecksum(ByteArrayView data, Qt::ChecksumType standard)
{
    quint16 crc = 0x0000;
    switch (standard) {
    case Qt::ChecksumIso3309:
        crc = 0xffff;
        break;
    case Qt::ChecksumItuV41:
        crc = 0x6363;
        break;
    }
    uchar c;
    const uchar* p = reinterpret_cast<const uchar*>(data.data());
    sizetype len = data.size();
    while (len--) {
        c = *p++;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
        c >>= 4;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
    }
    switch (standard) {
    case Qt::ChecksumIso3309:
        crc = ~crc;
        break;
    case Qt::ChecksumItuV41:
        break;
    }
    return crc & 0xffff;
}

#ifndef QT_NO_COMPRESS
using CompressSizeHint_t = uint32; // 32-bit BE, historically

enum class ZLibOp : bool { Compression, Decompression };

DECL_COLD_FUNCTION
static const char* zlibOpAsString(ZLibOp op)
{
    switch (op) {
    case ZLibOp::Compression: return "qCompress";
    case ZLibOp::Decompression: return "qUncompress";
    }
    AssertUnreachable();
}

Q_DECL_COLD_FUNCTION
static ByteArray zlibError(ZLibOp op, const char* what)
{
    qWarning("%s: %s", zlibOpAsString(op), what);
    return ByteArray();
}

Q_DECL_COLD_FUNCTION
static ByteArray dataIsNull(ZLibOp op)
{
    return zlibError(op, "Data is null");
}

Q_DECL_COLD_FUNCTION
static ByteArray lengthIsNegative(ZLibOp op)
{
    return zlibError(op, "Input length is negative");
}

Q_DECL_COLD_FUNCTION
static ByteArray tooMuchData(ZLibOp op)
{
    return zlibError(op, "Not enough memory");
}

Q_DECL_COLD_FUNCTION
static ByteArray invalidCompressedData()
{
    return zlibError(ZLibOp::Decompression, "Input data is corrupted");
}

Q_DECL_COLD_FUNCTION
static ByteArray unexpectedZlibError(ZLibOp op, int err, const char* msg)
{
    qWarning("%s unexpected zlib error: %s (%d)",
        zlibOpAsString(op),
        msg ? msg : "",
        err);
    return ByteArray();
}

static ByteArray xxflate(ZLibOp op, ArrayDataPointer<char> out, ByteArrayView input,
    qxp::function_ref<int(z_stream*) const> init,
    qxp::function_ref<int(z_stream*, size_t) const> processChunk,
    qxp::function_ref<void(z_stream*) const> deinit)
{
    if (out.data() == nullptr) // allocation failed
        return tooMuchData(op);
    sizetype capacity = out.allocatedCapacity();

    const auto initalSize = out.size;

    z_stream zs = {};
    zs.next_in = reinterpret_cast<uchar*>(const_cast<char*>(input.data())); // 1980s C API...
    if (const int err = init(&zs); err != Z_OK)
        return unexpectedZlibError(op, err, zs.msg);
    const auto sg = gsl::finally([&] { deinit(&zs); });

    using ZlibChunkSize_t = decltype(zs.avail_in);
    static_assert(!std::is_signed_v<ZlibChunkSize_t>);
    static_assert(std::is_same_v<ZlibChunkSize_t, decltype(zs.avail_out)>);
    constexpr auto MaxChunkSize = std::numeric_limits<ZlibChunkSize_t>::max();
    [[maybe_unused]]
    constexpr auto MaxStatisticsSize = std::numeric_limits<decltype(zs.total_out)>::max();

    size_t inputLeft = size_t(input.size());

    int res;
    do {
        Assert(out.freeSpaceAtBegin() == 0); // ensure prepend optimization stays out of the way
        Assert(capacity == out.allocatedCapacity());

        if (zs.avail_out == 0) {
            Assert(size_t(out.size) - initalSize > MaxStatisticsSize || // total_out overflow
                size_t(out.size) - initalSize == zs.total_out);
            Assert(out.size <= capacity);

            sizetype avail_out = capacity - out.size;
            if (avail_out == 0) {
                out->reallocateAndGrow(ArrayData::GrowsAtEnd, 1); // grow to next natural capacity
                if (out.data() == nullptr) // reallocation failed
                    return tooMuchData(op);
                capacity = out.allocatedCapacity();
                avail_out = capacity - out.size;
            }
            zs.next_out = reinterpret_cast<uchar*>(out.data()) + out.size;
            zs.avail_out = size_t(avail_out) > size_t(MaxChunkSize) ? MaxChunkSize
                : ZlibChunkSize_t(avail_out);
            out.size += zs.avail_out;

            Assert(zs.avail_out > 0);
        }

        if (zs.avail_in == 0) {
            // zs.next_in is kept up-to-date by processChunk(), so nothing to do
            zs.avail_in = inputLeft > MaxChunkSize ? MaxChunkSize : ZlibChunkSize_t(inputLeft);
            inputLeft -= zs.avail_in;
        }

        res = processChunk(&zs, inputLeft);
    } while (res == Z_OK);

    switch (res) {
    case Z_STREAM_END:
        out.size -= zs.avail_out;
        Assert(size_t(out.size) - initalSize > MaxStatisticsSize || // total_out overflow
            size_t(out.size) - initalSize == zs.total_out);
        Assert(out.size <= out.allocatedCapacity());
        out.data()[out.size] = '\0';
        return ByteArray(std::move(out));

    case Z_MEM_ERROR:
        return tooMuchData(op);

    case Z_BUF_ERROR:
        AssertUnreachable(); // cannot happen - we supply a buffer that can hold the result,
        // or else error out early

    case Z_DATA_ERROR:   // can only happen on decompression
        Assert(op == ZLibOp::Decompression);
        return invalidCompressedData();

    default:
        return unexpectedZlibError(op, res, zs.msg);
    }
}

ByteArray qCompress(const uchar* data, sizetype nbytes, int compressionLevel)
{
    constexpr sizetype HeaderSize = sizeof(CompressSizeHint_t);
    if (nbytes == 0) {
        return ByteArray(HeaderSize, '\0');
    }
    if (!data)
        return dataIsNull(ZLibOp::Compression);

    if (nbytes < 0)
        return lengthIsNegative(ZLibOp::Compression);

    if (compressionLevel < -1 || compressionLevel > 9)
        compressionLevel = -1;

    ArrayDataPointer out = [&] {
        constexpr sizetype SingleAllocLimit = 256 * 1024; // the maximum size for which we use
        sizetype capacity = HeaderSize;
        if (nbytes < SingleAllocLimit) {
            // use maximum size
            capacity += compressBound(uLong(nbytes)); // cannot overflow (both times)!
            return ArrayDataPointer<char>(capacity);
        }


        constexpr sizetype MaxCompressionFactor = 1024; // max theoretical factor is 1032

        capacity += std::max(sizetype(compressBound(uLong(SingleAllocLimit))),
            nbytes / MaxCompressionFactor);
        return ArrayDataPointer<char>(capacity, 0, ArrayData::Grow);
        }();

    if (out.data() == nullptr) // allocation failed
        return tooMuchData(ZLibOp::Compression);

    ToBigEndian(saturate<CompressSizeHint_t>(nbytes), out.data());
    out.size = HeaderSize;

    return xxflate(ZLibOp::Compression, std::move(out), { data, nbytes },
        [=](z_stream* zs) { return deflateInit(zs, compressionLevel); },
        [](z_stream* zs, size_t inputLeft) {
            return deflate(zs, inputLeft ? Z_NO_FLUSH : Z_FINISH);
        },
        [](z_stream* zs) { deflateEnd(zs); });
}
#endif

#ifndef QT_NO_COMPRESS

ByteArray qUncompress(const uchar* data, sizetype nbytes)
{
    if (!data)
        return dataIsNull(ZLibOp::Decompression);

    if (nbytes < 0)
        return lengthIsNegative(ZLibOp::Decompression);

    constexpr sizetype HeaderSize = sizeof(CompressSizeHint_t);
    if (nbytes < HeaderSize)
        return invalidCompressedData();

    const auto expectedSize = FromBigEndian<CompressSizeHint_t>(data);
    if (nbytes == HeaderSize) {
        if (expectedSize != 0)
            return invalidCompressedData();
        return ByteArray();
    }

    constexpr auto MaxDecompressedSize = size_t(MaxByteArraySize);
    if constexpr (MaxDecompressedSize < std::numeric_limits<CompressSizeHint_t>::max()) {
        if (expectedSize > MaxDecompressedSize)
            return tooMuchData(ZLibOp::Decompression);
    }

    // expectedSize may be truncated, so always use at least nbytes
    // (larger by at most 1%, according to zlib docs)
    sizetype capacity = std::max(sizetype(expectedSize), // cannot overflow!
        nbytes);

    ArrayDataPointer<char> d(capacity);
    return xxflate(ZLibOp::Decompression, std::move(d), { data + HeaderSize, nbytes - HeaderSize },
        [](z_stream* zs) { return inflateInit(zs); },
        [](z_stream* zs, size_t) { return inflate(zs, Z_NO_FLUSH); },
        [](z_stream* zs) { inflateEnd(zs); });
}
#endif

ByteArray::iterator ByteArray::erase(ByteArray::const_iterator first, ByteArray::const_iterator last)
{
    const auto start = std::distance(cbegin(), first);
    const auto len = std::distance(first, last);
    remove(start, len);
    return begin() + start;
}

ByteArray& ByteArray::operator=(const ByteArray& other) noexcept
{
    d = other.d;
    return *this;
}

ByteArray& ByteArray::operator=(const char* str)
{
    if (!str) {
        d.clear();
    }
    else if (!*str) {
        d = DataPointer::fromRawData(&_empty, 0);
    }
    else {
        assign(str);
    }
    return *this;
}

void ByteArray::truncate(sizetype pos)
{
    if (pos < size())
        resize(pos);
}


void ByteArray::chop(sizetype n)
{
    if (n > 0)
        resize(size() - n);
}

ByteArray::ByteArray(const char* data, sizetype size)
{
    if (!data) {
        d = DataPointer();
    }
    else {
        if (size < 0)
            size = qstrlen(data);
        if (!size) {
            d = DataPointer::fromRawData(&_empty, 0);
        }
        else {
            d = DataPointer(size, size);
            Q_CHECK_PTR(d.data());
            memcpy(d.data(), data, size);
            d.data()[size] = '\0';
        }
    }
}



ByteArray::ByteArray(sizetype size, char ch)
{
    if (size <= 0) {
        d = DataPointer::fromRawData(&_empty, 0);
    }
    else {
        d = DataPointer(size, size);
        Assert(d.data() != nullptr);
        memset(d.data(), ch, size);
        d.data()[size] = '\0';
    }
}


ByteArray::ByteArray(sizetype size, Initialization)
{
    if (size <= 0) {
        d = DataPointer::fromRawData(&_empty, 0);
    }
    else {
        d = DataPointer(size, size);
        Assert(d.data() != nullptr);
        d.data()[size] = '\0';
    }
}

void ByteArray::resize(sizetype size)
{
    if (size < 0)
        size = 0;

    const auto capacityAtEnd = capacity() - d.freeSpaceAtBegin();
    if (d->needsDetach() || size > capacityAtEnd)
        reallocData(size, ArrayData::Grow);
    d.size = size;
    if (d->allocatedCapacity())
        d.data()[size] = 0;
}

void ByteArray::resize(sizetype newSize, char c)
{
    const auto old = d.size;
    resize(newSize);
    if (old < d.size)
        memset(d.data() + old, c, d.size - old);
}

ByteArray& ByteArray::fill(char ch, sizetype size)
{
    resize(size < 0 ? this->size() : size);
    if (this->size())
        memset(d.data(), ch, this->size());
    return *this;
}

void ByteArray::reallocData(sizetype alloc, ArrayData::AllocationOption option)
{
    if (!alloc) {
        d = DataPointer::fromRawData(&_empty, 0);
        return;
    }

    const bool cannotUseReallocate = d.freeSpaceAtBegin() > 0;

    if (d->needsDetach() || cannotUseReallocate) {
        DataPointer dd(alloc, qMin(alloc, d.size), option);
        Assert(dd.data() != nullptr);
        if (dd.size > 0)
            ::memcpy(dd.data(), d.data(), dd.size);
        dd.data()[dd.size] = 0;
        d = dd;
    }
    else {
        d->reallocate(alloc, option);
    }
}

void ByteArray::reallocGrowData(sizetype n)
{
    if (!n)  // expected to always allocate
        n = 1;

    if (d->needsDetach()) {
        DataPointer dd(DataPointer::allocateGrow(d, n, ArrayData::GrowsAtEnd));
        Assert(dd.data() != nullptr);
        dd->copyAppend(d.data(), d.data() + d.size);
        dd.data()[dd.size] = 0;
        d = dd;
    }
    else {
        d->reallocate(d.constAllocatedCapacity() + n, ArrayData::Grow);
    }
}

void ByteArray::expand(sizetype i)
{
    resize(qMax(i + 1, size()));
}


ByteArray& ByteArray::prepend(const ByteArray& ba)
{
    if (size() == 0 && ba.size() > d.constAllocatedCapacity() && ba.d.isMutable())
        return (*this = ba);
    return prepend(ByteArrayView(ba));
}

ByteArray& ByteArray::append(const ByteArray& ba)
{
    if (!ba.isNull()) {
        if (isNull()) {
            if (UNLIKELY(!ba.d.isMutable()))
                assign(ba); // fromRawData, so we do a deep copy
            else
                operator=(ba);
        }
        else if (ba.size()) {
            append(ByteArrayView(ba));
        }
    }
    return *this;
}

ByteArray& ByteArray::append(char ch)
{
    d.detachAndGrow(ArrayData::GrowsAtEnd, 1, nullptr, nullptr);
    d->copyAppend(1, ch);
    d.data()[d.size] = '\0';
    return *this;
}

ByteArray& ByteArray::assign(ByteArrayView v)
{
    const auto len = v.size();

    if (len <= capacity() && isDetached()) {
        const auto offset = d.freeSpaceAtBegin();
        if (offset)
            d.setBegin(d.begin() - offset);
        std::memcpy(d.begin(), v.data(), len);
        d.size = len;
        d.data()[d.size] = '\0';
    }
    else {
        *this = v.toByteArray();
    }
    return *this;
}

ByteArray& ByteArray::insert(sizetype i, ByteArrayView data)
{
    const char* str = data.data();
    sizetype size = data.size();
    if (i < 0 || size <= 0)
        return *this;

    if (i >= d->size) {
        DataPointer detached{};  // construction is free
        d.detachAndGrow(ArrayData::GrowsAtEnd, (i - d.size) + size, &str, &detached);
        Assert(d.data() != nullptr);
        d->copyAppend(i - d->size, ' ');
        d->copyAppend(str, str + size);
        d.data()[d.size] = '\0';
        return *this;
    }

    if (!d->needsDetach() && points_into_range(str, d)) {
        QVarLengthArray a(str, str + size);
        return insert(i, a);
    }

    d->insert(i, str, size);
    d.data()[d.size] = '\0';
    return *this;
}

ByteArray& ByteArray::insert(sizetype i, sizetype count, char ch)
{
    if (i < 0 || count <= 0)
        return *this;

    if (i >= d->size) {
        // handle this specially, as ArrayDataOps::insert() doesn't handle out of bounds positions
        d.detachAndGrow(ArrayData::GrowsAtEnd, (i - d.size) + count, nullptr, nullptr);
        Assert(d.data() != nullptr);
        d->copyAppend(i - d->size, ' ');
        d->copyAppend(count, ch);
        d.data()[d.size] = '\0';
        return *this;
    }

    d->insert(i, count, ch);
    d.data()[d.size] = '\0';
    return *this;
}

ByteArray& ByteArray::remove(sizetype pos, sizetype len)
{
    if (len <= 0 || pos < 0 || size_t(pos) >= size_t(size()))
        return *this;
    if (pos + len > d->size)
        len = d->size - pos;

    auto begin = d.begin();
    if (!d->isShared()) {
        d->erase(begin + pos, len);
        d.data()[d.size] = '\0';
    }
    else {
        ByteArray copy{ size() - len, Initialization::Uninitialized };
        const auto toRemove_start = d.begin() + pos;
        copy.d->copyRanges({ {d.begin(), toRemove_start},
                           {toRemove_start + len, d.end()} });
        swap(copy);
    }
    return *this;
}

ByteArray& ByteArray::replace(sizetype pos, sizetype len, ByteArrayView after)
{
    if (points_into_range(after.data(), d)) {
        QVarLengthArray copy(after.data(), after.data() + after.size());
        return replace(pos, len, ByteArrayView{ copy });
    }
    if (len == after.size() && (pos + len <= size())) {
        // same size: in-place replacement possible
        if (len > 0) {
            detach();
            memcpy(d.data() + pos, after.data(), len * sizeof(char));
        }
        return *this;
    }
    else {
        // ### optimize me
        remove(pos, len);
        return insert(pos, after);
    }
}

ByteArray& ByteArray::replace(ByteArrayView before, ByteArrayView after)
{
    const char* b = before.data();
    sizetype bsize = before.size();
    const char* a = after.data();
    sizetype asize = after.size();

    if (isNull() || (b == a && bsize == asize))
        return *this;

    // protect against before or after being part of this
    if (points_into_range(a, d)) {
        QVarLengthArray copy(a, a + asize);
        return replace(before, ByteArrayView{ copy });
    }
    if (points_into_range(b, d)) {
        QVarLengthArray copy(b, b + bsize);
        return replace(ByteArrayView{ copy }, after);
    }

    ByteArrayMatcher matcher(b, bsize);
    sizetype index = 0;
    sizetype len = size();
    char* d = data(); // detaches

    if (bsize == asize) {
        if (bsize) {
            while ((index = matcher.indexIn(*this, index)) != -1) {
                memcpy(d + index, a, asize);
                index += bsize;
            }
        }
    }
    else if (asize < bsize) {
        size_t to = 0;
        size_t movestart = 0;
        size_t num = 0;
        while ((index = matcher.indexIn(*this, index)) != -1) {
            if (num) {
                sizetype msize = index - movestart;
                if (msize > 0) {
                    memmove(d + to, d + movestart, msize);
                    to += msize;
                }
            }
            else {
                to = index;
            }
            if (asize) {
                memcpy(d + to, a, asize);
                to += asize;
            }
            index += bsize;
            movestart = index;
            num++;
        }
        if (num) {
            sizetype msize = len - movestart;
            if (msize > 0)
                memmove(d + to, d + movestart, msize);
            resize(len - num * (bsize - asize));
        }
    }
    else {
        while (index != -1) {
            size_t indices[4096];
            size_t pos = 0;
            while (pos < 4095) {
                index = matcher.indexIn(*this, index);
                if (index == -1)
                    break;
                indices[pos++] = index;
                index += bsize;
                // avoid infinite loop
                if (!bsize)
                    index++;
            }
            if (!pos)
                break;

            // we have a table of replacement positions, use them for fast replacing
            sizetype adjust = pos * (asize - bsize);
            // index has to be adjusted in case we get back into the loop above.
            if (index != -1)
                index += adjust;
            sizetype newlen = len + adjust;
            sizetype moveend = len;
            if (newlen > len) {
                resize(newlen);
                len = newlen;
            }
            d = this->d.data(); // data(), without the detach() check

            while (pos) {
                pos--;
                sizetype movestart = indices[pos] + bsize;
                sizetype insertstart = indices[pos] + pos * (asize - bsize);
                sizetype moveto = insertstart + asize;
                memmove(d + moveto, d + movestart, (moveend - movestart));
                if (asize)
                    memcpy(d + insertstart, a, asize);
                moveend = movestart - bsize;
            }
        }
    }
    return *this;
}

ByteArray& ByteArray::replace(char before, char after)
{
    if (before != after) {
        if (const auto pos = indexOf(before); pos >= 0) {
            const auto detachedData = data();
            std::replace(detachedData + pos, detachedData + size(), before, after);
        }
    }
    return *this;
}

std::vector<ByteArray> ByteArray::split(char sep) const
{
    std::vector<ByteArray> list;
    sizetype start = 0;
    sizetype end;
    while ((end = indexOf(sep, start)) != -1) {
        list.push_back(mid(start, end - start));
        start = end + 1;
    }
    list.push_back(mid(start));
    return list;
}

ByteArray ByteArray::repeated(sizetype times) const
{
    if (isEmpty())
        return *this;

    if (times <= 1) {
        if (times == 1)
            return *this;
        return ByteArray();
    }

    const sizetype resultSize = times * size();

    ByteArray result;
    result.reserve(resultSize);
    if (result.capacity() != resultSize)
        return ByteArray(); // not enough memory

    memcpy(result.d.data(), data(), size());

    sizetype sizeSoFar = size();
    char* end = result.d.data() + sizeSoFar;

    const sizetype halfResultSize = resultSize >> 1;
    while (sizeSoFar <= halfResultSize) {
        memcpy(end, result.d.data(), sizeSoFar);
        end += sizeSoFar;
        sizeSoFar <<= 1;
    }
    memcpy(end, result.d.data(), resultSize - sizeSoFar);
    result.d.data()[resultSize] = '\0';
    result.d.size = resultSize;
    return result;
}

#define REHASH(a) \
    if (ol_minus_1 < sizeof(std::size_t) * CHAR_BIT) \
        hashHaystack -= std::size_t(a) << ol_minus_1; \
    hashHaystack <<= 1

static inline sizetype findCharHelper(ByteArrayView haystack, sizetype from, char needle) noexcept
{
    if (from < 0)
        from = qMax(from + haystack.size(), sizetype(0));
    if (from < haystack.size()) {
        const char* const b = haystack.data();
        if (const auto n = static_cast<const char*>(
            memchr(b + from, needle, static_cast<size_t>(haystack.size() - from)))) {
            return n - b;
        }
    }
    return -1;
}

sizetype algorithms::findByteArray(ByteArrayView haystack, sizetype from, ByteArrayView needle) noexcept
{
    const auto ol = needle.size();
    const auto l = haystack.size();
    if (ol == 0) {
        if (from < 0)
            return qMax(from + l, 0);
        else
            return from > l ? -1 : from;
    }

    if (ol == 1)
        return findCharHelper(haystack, from, needle.front());

    if (from > l || ol + from > l)
        return -1;

    return FindByteArray(haystack.data(), haystack.size(), from, needle.data(), ol);
}


sizetype ByteArray::indexOf(char ch, sizetype from) const
{
    return ToByteArrayViewIgnoringNull(*this).indexOf(ch, from);
}

static sizetype lastIndexOfHelper(const char* haystack, sizetype l, const char* needle,
    sizetype ol, sizetype from)
{
    auto delta = l - ol;
    if (from < 0)
        from = delta;
    if (from < 0 || from > l)
        return -1;
    if (from > delta)
        from = delta;

    const char* end = haystack;
    haystack += from;
    const auto ol_minus_1 = std::size_t(ol - 1);
    const char* n = needle + ol_minus_1;
    const char* h = haystack + ol_minus_1;
    std::size_t hashNeedle = 0, hashHaystack = 0;
    sizetype idx;
    for (idx = 0; idx < ol; ++idx) {
        hashNeedle = ((hashNeedle << 1) + *(n - idx));
        hashHaystack = ((hashHaystack << 1) + *(h - idx));
    }
    hashHaystack -= *haystack;
    while (haystack >= end) {
        hashHaystack += *haystack;
        if (hashHaystack == hashNeedle && memcmp(needle, haystack, ol) == 0)
            return haystack - end;
        --haystack;
        REHASH(*(haystack + ol));
    }
    return -1;

}

static inline sizetype lastIndexOfCharHelper(ByteArrayView haystack, sizetype from, char needle) noexcept
{
    if (haystack.size() == 0)
        return -1;
    if (from < 0)
        from += haystack.size();
    else if (from > haystack.size())
        from = haystack.size() - 1;
    if (from >= 0) {
        const char* b = haystack.data();
        const char* n = b + from + 1;
        while (n-- != b) {
            if (*n == needle)
                return n - b;
        }
    }
    return -1;
}

sizetype algorithms::lastIndexOf(ByteArrayView haystack, sizetype from, ByteArrayView needle) noexcept
{
    if (haystack.isEmpty()) {
        if (needle.isEmpty() && from == 0)
            return 0;
        return -1;
    }
    const auto ol = needle.size();
    if (ol == 1)
        return lastIndexOfCharHelper(haystack, from, needle.front());

    return lastIndexOfHelper(haystack.data(), haystack.size(), needle.data(), ol, from);
}

sizetype ByteArray::lastIndexOf(char ch, sizetype from) const
{
    return ToByteArrayViewIgnoringNull(*this).lastIndexOf(ch, from);
}

static inline sizetype countCharHelper(ByteArrayView haystack, char needle) noexcept
{
    sizetype num = 0;
    for (char ch : haystack) {
        if (ch == needle)
            ++num;
    }
    return num;
}

sizetype count(ByteArrayView haystack, ByteArrayView needle) noexcept
{
    if (needle.size() == 0)
        return haystack.size() + 1;

    if (needle.size() == 1)
        return countCharHelper(haystack, needle[0]);

    sizetype num = 0;
    sizetype i = -1;
    if (haystack.size() > 500 && needle.size() > 5) {
        ByteArrayMatcher matcher(needle);
        while ((i = matcher.indexIn(haystack, i + 1)) != -1)
            ++num;
    }
    else {
        while ((i = haystack.indexOf(needle, i + 1)) != -1)
            ++num;
    }
    return num;
}


sizetype ByteArray::count(char ch) const
{
    return countCharHelper(*this, ch);
}


bool algorithms::startsWith(ByteArrayView haystack, ByteArrayView needle) noexcept
{
    if (haystack.size() < needle.size())
        return false;
    if (haystack.data() == needle.data() || needle.size() == 0)
        return true;
    return memcmp(haystack.data(), needle.data(), needle.size()) == 0;
}

bool algorithms::endsWith(ByteArrayView haystack, ByteArrayView needle) noexcept
{
    if (haystack.size() < needle.size())
        return false;
    if (haystack.end() == needle.end() || needle.size() == 0)
        return true;
    return memcmp(haystack.end() - needle.size(), needle.data(), needle.size()) == 0;
}

static constexpr inline bool isUpperCaseAscii(char c)
{
    return c >= 'A' && c <= 'Z';
}

/*
    Returns true if \a c is an lowercase ASCII letter.
 */
static constexpr inline bool isLowerCaseAscii(char c)
{
    return c >= 'a' && c <= 'z';
}

bool ByteArray::isUpper() const
{
    return std::none_of(begin(), end(), isLowerCaseAscii);
}

bool ByteArray::isLower() const
{
    return std::none_of(begin(), end(), isUpperCaseAscii);
}

ByteArray ByteArray::mid(sizetype pos, sizetype len) const&
{
    sizetype p = pos;
    sizetype l = len;
    using namespace QtPrivate;
    switch (QContainerImplHelper::mid(size(), &p, &l)) {
    case QContainerImplHelper::Null:
        return ByteArray();
    case QContainerImplHelper::Empty:
    {
        return ByteArray(DataPointer::fromRawData(&_empty, 0));
    }
    case QContainerImplHelper::Full:
        return *this;
    case QContainerImplHelper::Subset:
        return sliced(p, l);
    }
    Q_UNREACHABLE_RETURN(ByteArray());
}

ByteArray ByteArray::mid(sizetype pos, sizetype len)&&
{
    sizetype p = pos;
    sizetype l = len;
    using namespace QtPrivate;
    switch (QContainerImplHelper::mid(size(), &p, &l)) {
    case QContainerImplHelper::Null:
        return ByteArray();
    case QContainerImplHelper::Empty:
        resize(0);      // keep capacity if we've reserve()d
        [[fallthrough]];
    case QContainerImplHelper::Full:
        return std::move(*this);
    case QContainerImplHelper::Subset:
        return std::move(*this).sliced(p, l);
    }
    Q_UNREACHABLE_RETURN(ByteArray());
}

ByteArray ByteArray::sliced_helper(ByteArray& a, sizetype pos, sizetype n)
{
    if (n == 0)
        return fromRawData(&_empty, 0);
    DataPointer d = std::move(a.d).sliced(pos, n);
    d.data()[n] = 0;
    return ByteArray(std::move(d));
}


template <typename T>
static ByteArray toCase_template(T& input, uchar(*lookup)(uchar))
{
    // find the first bad character in input
    const char* orig_begin = input.constBegin();
    const char* firstBad = orig_begin;
    const char* e = input.constEnd();
    for (; firstBad != e; ++firstBad) {
        uchar ch = uchar(*firstBad);
        uchar converted = lookup(ch);
        if (ch != converted)
            break;
    }

    if (firstBad == e)
        return std::move(input);

    // transform the rest
    ByteArray s = std::move(input);    // will copy if T is const ByteArray
    char* b = s.begin();            // will detach if necessary
    char* p = b + (firstBad - orig_begin);
    e = b + s.size();
    for (; p != e; ++p)
        *p = char(lookup(uchar(*p)));
    return s;
}

ByteArray ByteArray::toLower_helper(const ByteArray& a)
{
    return toCase_template(a, misc::asciiLower);
}

ByteArray ByteArray::toLower_helper(ByteArray& a)
{
    return toCase_template(a, misc::asciiLower);
}

ByteArray ByteArray::toUpper_helper(const ByteArray& a)
{
    return toCase_template(a, misc::asciiUpper);
}

ByteArray ByteArray::toUpper_helper(ByteArray& a)
{
    return toCase_template(a, misc::asciiUpper);
}

void ByteArray::clear()
{
    d.clear();
}


ByteArray ByteArray::simplified_helper(const ByteArray& a)
{
    return StringAlgorithms<const ByteArray>::simplified_helper(a);
}

ByteArray ByteArray::simplified_helper(ByteArray& a)
{
    return StringAlgorithms<ByteArray>::simplified_helper(a);
}


ByteArray ByteArray::trimmed_helper(const ByteArray& a)
{
    return StringAlgorithms<const ByteArray>::trimmed_helper(a);
}

ByteArray ByteArray::trimmed_helper(ByteArray& a)
{
    return StringAlgorithms<ByteArray>::trimmed_helper(a);
}

ByteArrayView algorithms::trimmed(ByteArrayView view) noexcept
{
    const auto [start, stop] = StringAlgorithms<ByteArrayView>::trimmed_helper_positions(view);
    return ByteArrayView(start, stop);
}

ByteArray ByteArray::leftJustified(sizetype width, char fill, bool truncate) const
{
    ByteArray result;
    sizetype len = size();
    sizetype padlen = width - len;
    if (padlen > 0) {
        result.resize(len + padlen);
        if (len)
            memcpy(result.d.data(), data(), len);
        memset(result.d.data() + len, fill, padlen);
    }
    else {
        if (truncate)
            result = left(width);
        else
            result = *this;
    }
    return result;
}

ByteArray ByteArray::rightJustified(sizetype width, char fill, bool truncate) const
{
    ByteArray result;
    sizetype len = size();
    sizetype padlen = width - len;
    if (padlen > 0) {
        result.resize(len + padlen);
        if (len)
            memcpy(result.d.data() + padlen, data(), len);
        memset(result.d.data(), fill, padlen);
    }
    else {
        if (truncate)
            result = left(width);
        else
            result = *this;
    }
    return result;
}

auto algorithms::toSignedInteger(ByteArrayView data, int base) -> ParsedNumber<longlong>
{
    if (data.isEmpty())
        return {};

    const QSimpleParsedNumber r = QLocaleData::bytearrayToLongLong(data, base);
    if (r.ok())
        return ParsedNumber(r.result);
    return {};
}

auto algorithms::toUnsignedInteger(ByteArrayView data, int base) -> ParsedNumber<ulonglong>
{
    if (data.isEmpty())
        return {};

    const QSimpleParsedNumber r = QLocaleData::bytearrayToUnsLongLong(data, base);
    if (r.ok())
        return ParsedNumber(r.result);
    return {};
}

qlonglong ByteArray::toLongLong(bool* ok, int base) const
{
    return algorithms::toIntegral<qlonglong>(ToByteArrayViewIgnoringNull(*this), ok, base);
}

qulonglong ByteArray::toULongLong(bool* ok, int base) const
{
    return algorithms::toIntegral<qulonglong>(ToByteArrayViewIgnoringNull(*this), ok, base);
}


int ByteArray::toInt(bool* ok, int base) const
{
    return algorithms::toIntegral<int>(ToByteArrayViewIgnoringNull(*this), ok, base);
}


uint ByteArray::toUInt(bool* ok, int base) const
{
    return algorithms::toIntegral<uint>(ToByteArrayViewIgnoringNull(*this), ok, base);
}

long ByteArray::toLong(bool* ok, int base) const
{
    return algorithms::toIntegral<long>(ToByteArrayViewIgnoringNull(*this), ok, base);
}

ulong ByteArray::toULong(bool* ok, int base) const
{
    return algorithms::toIntegral<ulong>(ToByteArrayViewIgnoringNull(*this), ok, base);
}

short ByteArray::toShort(bool* ok, int base) const
{
    return algorithms::toIntegral<short>(ToByteArrayViewIgnoringNull(*this), ok, base);
}

ushort ByteArray::toUShort(bool* ok, int base) const
{
    return algorithms::toIntegral<ushort>(ToByteArrayViewIgnoringNull(*this), ok, base);
}

double ByteArray::toDouble(bool* ok) const
{
    return ByteArrayView(*this).toDouble(ok);
}

auto algorithms::toDouble(ByteArrayView a) noexcept -> ParsedNumber<double>
{
  /*  auto r = qt_asciiToDouble(a.data(), a.size(), WhitespacesAllowed);
    if (r.ok())
        return ParsedNumber{ r.result };
    else*/
        return {};
}

float ByteArray::toFloat(bool* ok) const
{
    return QLocaleData::convertDoubleToFloat(toDouble(ok), ok);
}

auto algorithms::toFloat(ByteArrayView a) noexcept -> ParsedNumber<float>
{
    if (const auto r = algorithms::toDouble(a)) {
        bool ok = true;
        const auto f = QLocaleData::convertDoubleToFloat(*r, &ok);
        if (ok)
            return ParsedNumber(f);
    }
    return {};
}

ByteArray ByteArray::toBase64(Base64Options options) const
{
    constexpr char alphabet_base64[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
        "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
    constexpr char alphabet_base64url[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
        "ghijklmn" "opqrstuv" "wxyz0123" "456789-_";
    const char* const alphabet = options & Base64UrlEncoding ? alphabet_base64url : alphabet_base64;
    constexpr char padchar = '=';
    sizetype padlen = 0;

    const sizetype sz = size();

    ByteArray tmp((sz + 2) / 3 * 4, Initialization::Uninitialized);

    sizetype i = 0;
    char* out = tmp.data();
    while (i < sz) {
        // encode 3 bytes at a time
        int chunk = 0;
        chunk |= int(uchar(data()[i++])) << 16;
        if (i == sz) {
            padlen = 2;
        }
        else {
            chunk |= int(uchar(data()[i++])) << 8;
            if (i == sz)
                padlen = 1;
            else
                chunk |= int(uchar(data()[i++]));
        }

        int j = (chunk & 0x00fc0000) >> 18;
        int k = (chunk & 0x0003f000) >> 12;
        int l = (chunk & 0x00000fc0) >> 6;
        int m = (chunk & 0x0000003f);
        *out++ = alphabet[j];
        *out++ = alphabet[k];

        if (padlen > 1) {
            if ((options & OmitTrailingEquals) == 0)
                *out++ = padchar;
        }
        else {
            *out++ = alphabet[l];
        }
        if (padlen > 0) {
            if ((options & OmitTrailingEquals) == 0)
                *out++ = padchar;
        }
        else {
            *out++ = alphabet[m];
        }
    }
    Assert((options & OmitTrailingEquals) || (out == tmp.size() + tmp.data()));
    if (options & OmitTrailingEquals)
        tmp.truncate(out - tmp.data());
    return tmp;
}

static char* qulltoa2(char* p, qulonglong n, int base)
{
    constexpr char b = 'a' - 10;
    do {
        const int c = n % base;
        n /= base;
        *--p = c + (c < 10 ? '0' : b);
    } while (n);

    return p;
}

ByteArray& ByteArray::setNum(qlonglong n, int base)
{
    constexpr int buffsize = 66; // big enough for MAX_ULLONG in base 2
    char buff[buffsize];
    char* p;

    if (n < 0) {
        // Take care to avoid overflow on negating min value:
        p = qulltoa2(buff + buffsize, qulonglong(-(1 + n)) + 1, base);
        *--p = '-';
    }
    else {
        p = qulltoa2(buff + buffsize, qulonglong(n), base);
    }

    clear();
    append(p, buffsize - (p - buff));
    return *this;
}


ByteArray& ByteArray::setNum(qulonglong n, int base)
{
    constexpr int buffsize = 66; // big enough for MAX_ULLONG in base 2
    char buff[buffsize];
    char* p = qulltoa2(buff + buffsize, n, base);

    clear();
    append(p, buffsize - (p - buff));
    return *this;
}

ByteArray& ByteArray::setNum(double n, char format, int precision)
{
    return *this = ByteArray::number(n, format, precision);
}

ByteArray ByteArray::number(int n, int base)
{
    ByteArray s;
    s.setNum(n, base);
    return s;
}


ByteArray ByteArray::number(uint n, int base)
{
    ByteArray s;
    s.setNum(n, base);
    return s;
}


ByteArray ByteArray::number(long n, int base)
{
    ByteArray s;
    s.setNum(n, base);
    return s;
}


ByteArray ByteArray::number(ulong n, int base)
{
    ByteArray s;
    s.setNum(n, base);
    return s;
}


ByteArray ByteArray::number(qlonglong n, int base)
{
    ByteArray s;
    s.setNum(n, base);
    return s;
}


ByteArray ByteArray::number(qulonglong n, int base)
{
    ByteArray s;
    s.setNum(n, base);
    return s;
}


ByteArray ByteArray::number(double n, char format, int precision)
{
    QLocaleData::DoubleForm form = QLocaleData::DFDecimal;

    switch (misc::toAsciiLower(format)) {
    case 'f':
        form = QLocaleData::DFDecimal;
        break;
    case 'e':
        form = QLocaleData::DFExponent;
        break;
    case 'g':
        form = QLocaleData::DFSignificantDigits;
        break;
    default:
        break;
    }

    return ByteArray(qdtoAscii(n, form, precision, isUpperCaseAscii(format)));
}

ByteArray& ByteArray::setRawData(const char* data, sizetype size)
{
    if (!data || !size)
        clear();
    else
        *this = fromRawData(data, size);
    return *this;
}

namespace {
    struct fromBase64_helper_result {
        sizetype decodedLength;
        ByteArray::Base64DecodingStatus status;
    };

    fromBase64_helper_result fromBase64_helper(const char* input, sizetype inputSize,
        char* output /* may alias input */,
        ByteArray::Base64Options options)
    {
        fromBase64_helper_result result{ 0, ByteArray::Base64DecodingStatus::Ok };

        unsigned int buf = 0;
        int nbits = 0;

        sizetype offset = 0;
        for (sizetype i = 0; i < inputSize; ++i) {
            int ch = input[i];
            int d;

            if (ch >= 'A' && ch <= 'Z') {
                d = ch - 'A';
            }
            else if (ch >= 'a' && ch <= 'z') {
                d = ch - 'a' + 26;
            }
            else if (ch >= '0' && ch <= '9') {
                d = ch - '0' + 52;
            }
            else if (ch == '+' && (options & ByteArray::Base64UrlEncoding) == 0) {
                d = 62;
            }
            else if (ch == '-' && (options & ByteArray::Base64UrlEncoding) != 0) {
                d = 62;
            }
            else if (ch == '/' && (options & ByteArray::Base64UrlEncoding) == 0) {
                d = 63;
            }
            else if (ch == '_' && (options & ByteArray::Base64UrlEncoding) != 0) {
                d = 63;
            }
            else {
                if (options & ByteArray::AbortOnBase64DecodingErrors) {
                    if (ch == '=') {
                        // can have 1 or 2 '=' signs, in both cases padding base64Size to
                        // a multiple of 4. Any other case is illegal.
                        if ((inputSize % 4) != 0) {
                            result.status = ByteArray::Base64DecodingStatus::IllegalInputLength;
                            return result;
                        }
                        else if ((i == inputSize - 1) ||
                            (i == inputSize - 2 && input[++i] == '=')) {
                            d = -1; // ... and exit the loop, normally
                        }
                        else {
                            result.status = ByteArray::Base64DecodingStatus::IllegalPadding;
                            return result;
                        }
                    }
                    else {
                        result.status = ByteArray::Base64DecodingStatus::IllegalCharacter;
                        return result;
                    }
                }
                else {
                    d = -1;
                }
            }

            if (d != -1) {
                buf = (buf << 6) | d;
                nbits += 6;
                if (nbits >= 8) {
                    nbits -= 8;
                    Assert(offset < i);
                    output[offset++] = buf >> nbits;
                    buf &= (1 << nbits) - 1;
                }
            }
        }

        result.decodedLength = offset;
        return result;
    }
} // anonymous namespace

ByteArray::FromBase64Result ByteArray::fromBase64Encoding(ByteArray&& base64, Base64Options options)
{
    if (base64.isDetached()) {
        const auto base64result = fromBase64_helper(base64.data(),
            base64.size(),
            base64.data(), // in-place
            options);
        base64.truncate(base64result.decodedLength);
        return { std::move(base64), base64result.status };
    }

    return fromBase64Encoding(base64, options);
}


ByteArray::FromBase64Result ByteArray::fromBase64Encoding(const ByteArray& base64, Base64Options options)
{
    const auto base64Size = base64.size();
    ByteArray result((base64Size * 3) / 4, Initialization::Uninitialized);
    const auto base64result = fromBase64_helper(base64.data(),
        base64Size,
        const_cast<char*>(result.constData()),
        options);
    result.truncate(base64result.decodedLength);
    return { std::move(result), base64result.status };
}

ByteArray ByteArray::fromBase64(const ByteArray& base64, Base64Options options)
{
    if (auto result = fromBase64Encoding(base64, options))
        return std::move(result.decoded);
    return ByteArray();
}

ByteArray ByteArray::fromHex(const ByteArray& hexEncoded)
{
    ByteArray res((hexEncoded.size() + 1) / 2, Initialization::Uninitialized);
    uchar* result = (uchar*)res.data() + res.size();

    bool odd_digit = true;
    for (sizetype i = hexEncoded.size() - 1; i >= 0; --i) {
        uchar ch = uchar(hexEncoded.at(i));
        int tmp = misc::fromHex(ch);
        if (tmp == -1)
            continue;
        if (odd_digit) {
            --result;
            *result = tmp;
            odd_digit = false;
        }
        else {
            *result |= tmp << 4;
            odd_digit = true;
        }
    }

    res.remove(0, result - (const uchar*)res.constData());
    return res;
}


ByteArray ByteArray::toHex(char separator) const
{
    if (isEmpty())
        return ByteArray();

    const sizetype length = separator ? (size() * 3 - 1) : (size() * 2);
    ByteArray hex(length, Initialization::Uninitialized);
    char* hexData = hex.data();
    const uchar* data = (const uchar*)this->data();
    for (sizetype i = 0, o = 0; i < size(); ++i) {
        hexData[o++] = misc::toHexLower(data[i] >> 4);
        hexData[o++] = misc::toHexLower(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}

static void q_fromPercentEncoding(ByteArray* ba, char percent)
{
    if (ba->isEmpty())
        return;

    char* data = ba->data();
    const char* inputPtr = data;

    sizetype i = 0;
    sizetype len = ba->size();
    sizetype outlen = 0;
    int a, b;
    char c;
    while (i < len) {
        c = inputPtr[i];
        if (c == percent && i + 2 < len) {
            a = inputPtr[++i];
            b = inputPtr[++i];

            if (a >= '0' && a <= '9') a -= '0';
            else if (a >= 'a' && a <= 'f') a = a - 'a' + 10;
            else if (a >= 'A' && a <= 'F') a = a - 'A' + 10;

            if (b >= '0' && b <= '9') b -= '0';
            else if (b >= 'a' && b <= 'f') b = b - 'a' + 10;
            else if (b >= 'A' && b <= 'F') b = b - 'A' + 10;

            *data++ = (char)((a << 4) | b);
        }
        else {
            *data++ = c;
        }

        ++i;
        ++outlen;
    }

    if (outlen != len)
        ba->truncate(outlen);
}

ByteArray ByteArray::percentDecoded(char percent) const
{
    if (isEmpty())
        return *this; // Preserves isNull().

    ByteArray tmp = *this;
    q_fromPercentEncoding(&tmp, percent);
    return tmp;
}

ByteArray ByteArray::fromPercentEncoding(const ByteArray& input, char percent)
{
    return input.percentDecoded(percent);
}


ByteArray ByteArray::fromStdString(const std::string& s)
{
    return ByteArray(s.data(), sizetype(s.size()));
}

std::string ByteArray::toStdString() const
{
    return std::string(data(), size_t(size()));
}

ByteArray ByteArray::toPercentEncoding(const ByteArray& exclude, const ByteArray& include,
    char percent) const
{
    if (isNull())
        return ByteArray();    // preserve null
    if (isEmpty())
        return ByteArray(data(), 0);

    const auto contains = [](const ByteArray& view, char c) {
        // As view.contains(c), but optimised to bypass a lot of overhead:
        return view.size() > 0 && memchr(view.data(), c, view.size()) != nullptr;
        };

    ByteArray result = *this;
    char* output = nullptr;
    sizetype length = 0;

    for (unsigned char c : *this) {
        if (char(c) != percent
            && ((c >= 0x61 && c <= 0x7A) // ALPHA
                || (c >= 0x41 && c <= 0x5A) // ALPHA
                || (c >= 0x30 && c <= 0x39) // DIGIT
                || c == 0x2D // -
                || c == 0x2E // .
                || c == 0x5F // _
                || c == 0x7E // ~
                || contains(exclude, c))
            && !contains(include, c)) {
            if (output)
                output[length] = c;
            ++length;
        }
        else {
            if (!output) {
                // detach now
                result.resize(size() * 3); // worst case
                output = result.data();
            }
            output[length++] = percent;
            output[length++] = misc::toHexUpper((c & 0xf0) >> 4);
            output[length++] = misc::toHexUpper(c & 0xf);
        }
    }
    if (output)
        result.truncate(length);

    return result;
}


size_t qHash(const ByteArray::FromBase64Result& key, size_t seed) noexcept
{
    return qHashMulti(seed, key.decoded, static_cast<int>(key.decodingStatus));
}

#undef REHASH


__BASE_IO_NAMESPACE_END
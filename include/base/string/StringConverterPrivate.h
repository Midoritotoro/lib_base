#pragma once


#include <base/system/Platform.h>
#include <base/string/String.h>

#include <base/system/Endian.h>
#include <base/system/SimdHelpers.h>

#include <base/io/ByteArrayView.h>
#include <base/string/StringConverterBase.h>

#include <base/string/Char.h>


namespace base::string {
    struct Latin1
    {
        // Defined in String.cpp
        static char16_t* convertToUnicode(char16_t* dst, Latin1StringView in) noexcept;

        static Char* convertToUnicode(Char* buffer, Latin1StringView in) noexcept
        {
            char16_t* dst = reinterpret_cast<char16_t*>(buffer);
            dst = convertToUnicode(dst, in);
            return reinterpret_cast<Char*>(dst);
        }

        static Char* convertToUnicode(Char* dst, io::ByteArrayView in,
            [[maybe_unused]] StringConverterBase::State* state) noexcept
        {
            Assert(state);
            return convertToUnicode(dst, Latin1StringView(in.data(), in.size()));
        }

        static char* convertFromUnicode(char* out, StringView in, StringConverter::State* state) noexcept;

        // Defined in String.cpp
        static char* convertFromUnicode(char* out, StringView in) noexcept;
    };

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

        static void appendUcs4(char16_t*& ptr, char32_t uc)
        {
            appendUtf16(ptr, Char::highSurrogate(uc));
            appendUtf16(ptr, Char::lowSurrogate(uc));
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

    namespace Utf8Functions
    {
        /// returns 0 on success; errors can only happen if \a u is a surrogate:
        /// Error if \a u is a low surrogate;
        /// if \a u is a high surrogate, Error if the next isn't a low one,
        /// EndOfString if we run into the end of the string.
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

        /// returns the number of characters consumed (including \a b) in case of success;
        /// returns negative in case of error: Traits::Error or Traits::EndOfString
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
    }

    enum DataEndianness
    {
        DetectEndianness,
        BigEndianness,
        LittleEndianness
    };

    struct Utf8
    {
        static QChar* convertToUnicode(QChar* buffer, QByteArrayView in) noexcept
        {
            char16_t* dst = reinterpret_cast<char16_t*>(buffer);
            dst = Utf8::convertToUnicode(dst, in);
            return reinterpret_cast<QChar*>(dst);
        }

        Q_CORE_EXPORT static char16_t* convertToUnicode(char16_t* dst, QByteArrayView in) noexcept;
        static String convertToUnicode(QByteArrayView in);
        Q_CORE_EXPORT static String convertToUnicode(QByteArrayView in, StringConverter::State* state);

        static QChar* convertToUnicode(QChar* out, QByteArrayView in, StringConverter::State* state)
        {
            char16_t* buffer = reinterpret_cast<char16_t*>(out);
            buffer = convertToUnicode(buffer, in, state);
            return reinterpret_cast<QChar*>(buffer);
        }

        static char16_t* convertToUnicode(char16_t* dst, QByteArrayView in, StringConverter::State* state);

        Q_CORE_EXPORT static QByteArray convertFromUnicode(StringView in);
        Q_CORE_EXPORT static QByteArray convertFromUnicode(StringView in, StringConverterBase::State* state);
        static char* convertFromUnicode(char* out, StringView in, StringConverter::State* state);
        Q_CORE_EXPORT static char* convertFromLatin1(char* out, QLatin1StringView in);
        struct ValidUtf8Result {
            bool isValidUtf8;
            bool isValidAscii;
        };
        static ValidUtf8Result isValidUtf8(QByteArrayView in);
        static int compareUtf8(QByteArrayView utf8, StringView utf16,
            Qt::CaseSensitivity cs = Qt::CaseSensitive) noexcept;
        static int compareUtf8(QByteArrayView utf8, QLatin1StringView s,
            Qt::CaseSensitivity cs = Qt::CaseSensitive);
        static int compareUtf8(QByteArrayView lhs, QByteArrayView rhs,
            Qt::CaseSensitivity cs = Qt::CaseSensitive) noexcept;
    };

    struct QUtf16
    {
        Q_CORE_EXPORT static String convertToUnicode(QByteArrayView, StringConverter::State*, DataEndianness = DetectEndianness);
        static QChar* convertToUnicode(QChar* out, QByteArrayView, StringConverter::State* state, DataEndianness endian);
        Q_CORE_EXPORT static QByteArray convertFromUnicode(StringView, StringConverter::State*, DataEndianness = DetectEndianness);
        static char* convertFromUnicode(char* out, StringView in, StringConverter::State* state, DataEndianness endian);
    };

    struct QUtf32
    {
        static QChar* convertToUnicode(QChar* out, QByteArrayView, StringConverter::State* state, DataEndianness endian);
        Q_CORE_EXPORT static String convertToUnicode(QByteArrayView, StringConverter::State*, DataEndianness = DetectEndianness);
        Q_CORE_EXPORT static QByteArray convertFromUnicode(StringView, StringConverter::State*, DataEndianness = DetectEndianness);
        static char* convertFromUnicode(char* out, StringView in, StringConverter::State* state, DataEndianness endian);
    };

    struct Q_CORE_EXPORT QLocal8Bit
    {
#if !defined(Q_OS_WIN) || defined(QT_BOOTSTRAPPED)
        static String convertToUnicode(QByteArrayView in, StringConverter::State* state)
        {
            return Utf8::convertToUnicode(in, state);
        }
        static QByteArray convertFromUnicode(QStringView in, StringConverter::State* state)
        {
            return Utf8::convertFromUnicode(in, state);
        }
#else
        static int checkUtf8();
        static bool isUtf8()
        {
            
            static std::atomic<qint8> result = { 0 };
            int r = result.load(std::memory_order_relaxed);
            if (r == 0) {
                r = checkUtf8();
                result.store(r, std::memory_order_relaxed);
            }
            return r > 0;
        }
        static String convertToUnicode_sys(io::ByteArrayView, uint32, StringConverter::State*);
        static String convertToUnicode_sys(io::ByteArrayView, StringConverter::State*);
        static String convertToUnicode(io::ByteArrayView in, StringConverter::State* state)
        {
            if (isUtf8())
                return Utf8::convertToUnicode(in, state);
            return convertToUnicode_sys(in, state);
        }
        static QByteArray convertFromUnicode_sys(StringView, uint32, StringConverter::State*);
        static QByteArray convertFromUnicode_sys(StringView, StringConverter::State*);
        static QByteArray convertFromUnicode(StringView in, StringConverter::State* state)
        {
            if (isUtf8())
                return Utf8::convertFromUnicode(in, state);
            return convertFromUnicode_sys(in, state);
        }
#endif
    };
} // namespace base::string
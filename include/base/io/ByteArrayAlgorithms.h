#pragma once

#include <string.h>
#include <stdarg.h>

#include <base/system/Platform.h>

namespace base::io {
    
    class ByteArrayView;
    namespace algorithms {
        [[nodiscard]] bool startsWith(ByteArrayView haystack, ByteArrayView needle) noexcept;

        [[nodiscard]] bool endsWith(ByteArrayView haystack, ByteArrayView needle) noexcept;

        [[nodiscard]]
        sizetype findByteArray(ByteArrayView haystack, sizetype from, ByteArrayView needle) noexcept;

        [[nodiscard]]
        sizetype lastIndexOf(ByteArrayView haystack, sizetype from, ByteArrayView needle) noexcept;

        [[nodiscard]]
        sizetype count(ByteArrayView haystack, ByteArrayView needle) noexcept;

        [[nodiscard]] int compareMemory(ByteArrayView lhs, ByteArrayView rhs);

        [[nodiscard]] ByteArrayView trimmed(ByteArrayView s) noexcept;

        [[nodiscard]] bool isValidUtf8(ByteArrayView s) noexcept;

        template <typename T>
        class ParsedNumber
        {
            T m_value;
            uint32 m_error : 1;
            uint32 m_reserved : 31;
            void* m_reserved2 = nullptr;
        public:
            constexpr ParsedNumber() noexcept : m_value(), m_error(true), m_reserved(0) {}
            constexpr explicit ParsedNumber(T v) : m_value(v), m_error(false), m_reserved(0) {}

            // minimal optional-like API:
            explicit operator bool() const noexcept { return !m_error; }
            T& operator*() { Assert(*this); return m_value; }
            const T& operator*() const { Assert(*this); return m_value; }
            T* operator->() noexcept { return *this ? &m_value : nullptr; }
            const T* operator->() const noexcept { return *this ? &m_value : nullptr; }
            template <typename U> // not = T, as that'd allow calls that are incompatible with std::optional
            T value_or(U&& u) const { return *this ? m_value : T(std::forward<U>(u)); }
        };

        [[nodiscard]] ParsedNumber<double> toDouble(ByteArrayView a) noexcept;
        [[nodiscard]] ParsedNumber<float> toFloat(ByteArrayView a) noexcept;
        [[nodiscard]] ParsedNumber<longlong> toSignedInteger(ByteArrayView data, int base);
        [[nodiscard]] ParsedNumber<ulonglong> toUnsignedInteger(ByteArrayView data, int base);

        // QByteArrayView has incomplete type here, and we can't include qbytearrayview.h,
        // since it includes qbytearrayalgorithms.h. Use the ByteArrayView template type as
        // a workaround.
        template <typename T, typename _ByteArrayView,
            typename = std::enable_if_t<std::is_same_v<_ByteArrayView, ByteArrayView>>>
        static inline T toIntegral(_ByteArrayView data, bool* ok, int base)
        {
            const auto val = [&] {
                if constexpr (std::is_unsigned_v<T>)
                    return toUnsignedInteger(data, base);
                else
                    return toSignedInteger(data, base);
                }();
            const bool failed = !val || T(*val) != *val;
            if (ok)
                *ok = !failed;
            if (failed)
                return 0;
            return T(*val);
        }
    } // algorithms

    /*****************************************************************************
      Safe and portable C string functions; extensions to standard string.h
     *****************************************************************************/

    char* qstrdup(const char*);

    inline size_t qstrlen(const char* str)
    {
        return str ? strlen(str) : 0;
    }

    inline size_t qstrnlen(const char* str, size_t maxlen)
    {
        if (!str)
            return 0;
        auto end = static_cast<const char*>(memchr(str, '\0', maxlen));
        return end ? end - str : maxlen;
    }

// implemented in qbytearray.cpp
    char* qstrcpy(char* dst, const char* src);
    char* qstrncpy(char* dst, const char* src, size_t len);

    int qstrcmp(const char* str1, const char* str2);

    inline int qstrncmp(const char* str1, const char* str2, size_t len)
    {
        return (str1 && str2) ? strncmp(str1, str2, len)
            : (str1 ? 1 : (str2 ? -1 : 0));
    }
    int qstricmp(const char*, const char*);
    int qstrnicmp(const char*, const char*, size_t len);
    int qstrnicmp(const char*, qsizetype, const char*, qsizetype = -1);

    // implemented in qvsnprintf.cpp
    int qvsnprintf(char* str, size_t n, const char* fmt, va_list ap);
    int qsnprintf(char* str, size_t n, const char* fmt, ...);

    quint16 qChecksum(QByteArrayView data, Qt::ChecksumType standard = Qt::ChecksumIso3309);
}
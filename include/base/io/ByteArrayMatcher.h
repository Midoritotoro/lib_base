#pragma once

#include <base/core/BaseNamespace.h>

#include <base/io/ByteArrayView.h>
#include <base/io/ByteArray.h>


__BASE_IO_NAMESPACE_BEGIN

class ByteArrayMatcher
{
public:
    ByteArrayMatcher();
    explicit ByteArrayMatcher(const ByteArray &pattern);
    explicit ByteArrayMatcher(ByteArrayView pattern)
        : ByteArrayMatcher(pattern.data(), pattern.size())
    {}
    explicit ByteArrayMatcher(const char *pattern, sizetype length = -1);
    ByteArrayMatcher(const ByteArrayMatcher &other);
    ~ByteArrayMatcher();

    ByteArrayMatcher &operator=(const ByteArrayMatcher &other);

    void setPattern(const ByteArray &pattern);

#if QT_CORE_REMOVED_SINCE(6, 3)
    qsizetype indexIn(const QByteArray &ba, qsizetype from = 0) const;
#else
    Q_WEAK_OVERLOAD
    sizetype indexIn(const ByteArray &ba, sizetype from = 0) const
    { return indexIn(ByteArrayView{ba}, from); }
#endif
    sizetype indexIn(const char *str, sizetype len, sizetype from = 0) const;
    sizetype indexIn(ByteArrayView data, sizetype from = 0) const;
    inline ByteArray pattern() const
    {
        if (q_pattern.isNull())
            return ByteArray(reinterpret_cast<const char*>(p.p), p.l);
        return q_pattern;
    }

private:
    ByteArray q_pattern;
    struct Data {
        uchar q_skiptable[256];
        const uchar *p;
        sizetype l;
    };
    union {
        uint dummy[256];
        Data p;
    };
};

class StaticByteArrayMatcherBase
{
    alignas(16)
    struct Skiptable {
        uchar data[256];
    } m_skiptable;
protected:
    explicit constexpr StaticByteArrayMatcherBase(const char *pattern, size_t n) noexcept
        : m_skiptable(generate(pattern, n)) {}
    // compiler-generated copy/more ctors/assignment operators are ok!
    ~StaticByteArrayMatcherBase() = default;

    sizetype indexOfIn(const char *needle, size_t nlen,
                                      const char *haystack, sizetype hlen,
                                      sizetype from) const noexcept;

private:
    static constexpr Skiptable generate(const char *pattern, size_t n) noexcept
    {
        const auto uchar_max = (std::numeric_limits<uchar>::max)();
        uchar max = n > uchar_max ? uchar_max : uchar(n);
        Skiptable table = {};
        std::fill(std::begin(table.data), std::end(table.data), max);
        pattern += n - max;
        while (max--)
            table.data[uchar(*pattern++)] = max;
        return table;
    }
};

template <size_t N>
class StaticByteArrayMatcher : StaticByteArrayMatcherBase
{
    char m_pattern[N];
    // N includes the terminating '\0'!
    static_assert(N > 2, "StaticByteArrayMatcher makes no sense for finding a single-char pattern");
public:
    explicit constexpr StaticByteArrayMatcher(const char (&patternToMatch)[N]) noexcept
        : StaticByteArrayMatcher(patternToMatch, N - 1), m_pattern()
    {
        for (size_t i = 0; i < N; ++i)
            m_pattern[i] = patternToMatch[i];
    }

    WEAK_OVERLOAD
    sizetype indexIn(const ByteArray &haystack, sizetype from = 0) const noexcept
    { return this->indexOfIn(m_pattern, N - 1, haystack.data(), haystack.size(), from); }
    sizetype indexIn(const char *haystack, sizetype hlen, sizetype from = 0) const noexcept
    { return this->indexOfIn(m_pattern, N - 1, haystack, hlen, from); }
    sizetype indexIn(ByteArrayView haystack, sizetype from = 0) const noexcept
    { return this->indexOfIn(m_pattern, N - 1, haystack.data(), haystack.size(), from); }

    ByteArray pattern() const { return ByteArray(m_pattern, sizetype(N - 1)); }
};

template <size_t N>
constexpr StaticByteArrayMatcher<N> MakeStaticByteArrayMatcher(const char (&pattern)[N]) noexcept
{ return StaticByteArrayMatcher<N>(pattern); }


__BASE_IO_NAMESPACE_END

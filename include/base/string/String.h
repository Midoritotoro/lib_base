#pragma once

#include <base/string/Char.h>
#include <base/io/ByteArray.h>

#include <base/io/ByteArrayView.h>
#include <base/io/ArrayData.h>

#include <base/string/Latin1StringView.h>
#include <base/string/StringLiteral.h>

#include <base/string/StringAlgorithms.h>
#include <base/string/StringTokenizer.h>

#include <base/string/StringView.h>


#include <base/core/Types.h>

#include <base/string/AnyStringView.h>

#include <string>
#include <iterator>

#include <string_view>
#include <stdarg.h>


namespace base::string {
    class RegularExpression;
    class RegularExpressionMatch;
    class String;

    template <bool...B> class BoolList;

    template <typename _Char>
    using IsCompatibleChar32TypeHelper =
        std::is_same<_Char, char32_t>;

    template <typename _Char>
    using IsCompatibleChar32Type
        = IsCompatibleChar32TypeHelper<std::remove_cvref_t<_Char>>;

    constexpr bool isLatin1(Latin1StringView) noexcept
    {
        return true;
    }

    int StringView::compare(Latin1StringView s, CaseSensitivity cs) const noexcept
    {
        return compareStrings(*this, s, cs);
    }

    bool StringView::startsWith(Latin1StringView s, CaseSensitivity cs) const noexcept
    {
        return startsWith(*this, s, cs);
    }

    bool StringView::endsWith(Latin1StringView s, CaseSensitivity cs) const noexcept
    {
        return endsWith(*this, s, cs);
    }

    sizetype StringView::indexOf(Latin1StringView s, sizetype from, CaseSensitivity cs) const noexcept
    {
        return findString(*this, from, s, cs);
    }

    bool StringView::contains(Latin1StringView s, CaseSensitivity cs) const noexcept
    {
        return indexOf(s, 0, cs) != sizetype(-1);
    }

    sizetype StringView::lastIndexOf(Latin1StringView s, CaseSensitivity cs) const noexcept
    {
        return lastIndexOf(*this, size(), s, cs);
    }

    sizetype StringView::lastIndexOf(Latin1StringView s, sizetype from, CaseSensitivity cs) const noexcept
    {
        return lastIndexOf(*this, from, s, cs);
    }

    sizetype StringView::count(Latin1StringView s, CaseSensitivity cs) const
    {
        return count(*this, s, cs);
    }


constexpr AnyStringView::AnyStringView(Latin1StringView str) noexcept
    : m_data{ str.data() }, m_size{ size_t(str.size() << SizeShift) | Tag::Latin1 } {}

constexpr Latin1StringView AnyStringView::asLatin1StringView() const
{
    Assert(isLatin1());
    return { m_data_utf8, size() };
}


template <typename Visitor>
constexpr decltype(auto) AnyStringView::visit(Visitor&& v) const
{
    if (isUtf16())
        return std::forward<Visitor>(v)(asStringView());
    else if (isLatin1())
        return std::forward<Visitor>(v)(asLatin1StringView());
    else
        return std::forward<Visitor>(v)(asUtf8StringView());
}

//
// AnyStringView members that require AnyStringView::visit()
//

constexpr Char AnyStringView::front() const
{
    return visit([](auto that) { return AnyStringView::toChar(that.front()); });
}
constexpr Char AnyStringView::back() const
{
    return visit([](auto that) { return AnyStringView::toChar(that.back()); });
}


class String
{
    typedef io::TypedArrayData<char16_t> Data;

    template <typename Iterator>
    static constexpr bool is_contiguous_iterator_v =
        // Can't use contiguous_iterator_tag here, as STL impls can't agree on feature macro.
        // To avoid differences in C++20 and C++17 builds, treat only pointers as contiguous
        // for now:
        // std::contiguous_iterator<Iterator>;
        std::is_pointer_v<Iterator>;

    template <typename Char>
    using is_compatible_char_helper = std::disjunction<
        IsCompatibleCharType<Char>,
        IsCompatibleChar32Type<Char>,
        IsCompatibleChar8Type<Char>,
        std::is_same<Char, Latin1Char> // special case
    >;

    template <typename Iterator>
    static constexpr bool is_compatible_iterator_v = std::conjunction_v<
        std::is_convertible<
        typename std::iterator_traits<Iterator>::iterator_category,
        std::input_iterator_tag
        >,
        is_compatible_char_helper<typename std::iterator_traits<Iterator>::value_type>
    >;

    template <typename Iterator>
    using if_compatible_iterator = std::enable_if_t<is_compatible_iterator_v<Iterator>, bool>;

public:
    typedef StringPrivate DataPointer;

    constexpr String() noexcept;
    explicit String(const Char* unicode, sizetype size = -1);
    String(Char c);
    String(sizetype size, Char c);
    inline String(Latin1StringView latin1);
#if defined(__cpp_char8_t) || defined(Q_QDOC)
    Q_WEAK_OVERLOAD
        inline String(const char8_t* str)
        : String(fromUtf8(str))
    {}
#endif
    inline String(const String&) noexcept;
    inline ~String();
    String& operator=(Char c);
    String& operator=(const String&) noexcept;
    String& operator=(Latin1StringView latin1);
    inline String(String&& other) noexcept
        = default;
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(String)
        void swap(String& other) noexcept { d.swap(other.d); }
    inline sizetype size() const noexcept { return d.size; }
#if QT_DEPRECATED_SINCE(6, 4)
    QT_DEPRECATED_VERSION_X_6_4("Use size() or length() instead.")
        inline sizetype count() const { return d.size; }
#endif
    inline sizetype length() const noexcept { return d.size; }
    inline bool isEmpty() const noexcept { return d.size == 0; }
    void resize(sizetype size);
    void resize(sizetype size, Char fillChar);

    String& fill(Char c, sizetype size = -1);
    void truncate(sizetype pos);
    void chop(sizetype n);

    inline sizetype capacity() const;
    inline void reserve(sizetype size);
    inline void squeeze();

    inline const Char* unicode() const;
    inline Char* data();
    inline const Char* data() const;
    inline const Char* constData() const;

    inline void detach();
    inline bool isDetached() const;
    inline bool isSharedWith(const String& other) const { return d.isSharedWith(other.d); }
    inline void clear();

    inline const Char at(sizetype i) const;
    inline const Char operator[](sizetype i) const;
    [[nodiscard]] inline Char& operator[](sizetype i);

    [[nodiscard]] inline Char front() const { return at(0); }
    [[nodiscard]] inline Char& front();
    [[nodiscard]] inline Char back() const { return at(size() - 1); }
    [[nodiscard]] inline Char& back();

    [[nodiscard]] String arg(qlonglong a, int fieldwidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] String arg(qulonglong a, int fieldwidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] inline String arg(long a, int fieldwidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] inline String arg(ulong a, int fieldwidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] inline String arg(int a, int fieldWidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] inline String arg(uint a, int fieldWidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] inline String arg(short a, int fieldWidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] inline String arg(ushort a, int fieldWidth = 0, int base = 10,
        Char fillChar = u' ') const;
    [[nodiscard]] String arg(double a, int fieldWidth = 0, char format = 'g', int precision = -1,
        Char fillChar = u' ') const;
    [[nodiscard]] String arg(char a, int fieldWidth = 0,
        Char fillChar = u' ') const;
    [[nodiscard]] String arg(Char a, int fieldWidth = 0,
        Char fillChar = u' ') const;
    [[nodiscard]] String arg(const String& a, int fieldWidth = 0,
        Char fillChar = u' ') const;
    [[nodiscard]] String arg(StringView a, int fieldWidth = 0,
        Char fillChar = u' ') const;
    [[nodiscard]] String arg(Latin1StringView a, int fieldWidth = 0,
        Char fillChar = u' ') const;
private:
    template <typename T>
    using is_convertible_to_view_or_String = std::disjunction<
        std::is_convertible<T, String>,
        std::is_convertible<T, StringView>,
        std::is_convertible<T, Latin1StringView>
    >;
public:
    template <typename...Args>
    [[nodiscard]]
#ifdef Q_QDOC
    String
#else
    typename std::enable_if<
        sizeof...(Args) >= 2 && std::is_same<
        BoolList<is_convertible_to_view_or_String<Args>::value..., true>,
        BoolList<true, is_convertible_to_view_or_String<Args>::value...>
        >::value,
        String
    >::type
#endif
        arg(Args &&...args) const
    {
        return qToStringViewIgnoringNull(*this).arg(std::forward<Args>(args)...);
    }

    static String vasprintf(const char* format, va_list ap) Q_ATTRIBUTE_FORMAT_PRINTF(1, 0);
    static String asprintf(const char* format, ...) Q_ATTRIBUTE_FORMAT_PRINTF(1, 2);

    [[nodiscard]] sizetype indexOf(Char c, sizetype from = 0, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] sizetype indexOf(Latin1StringView s, sizetype from = 0, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] sizetype indexOf(const String& s, sizetype from = 0, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] sizetype indexOf(StringView s, sizetype from = 0, CaseSensitivity cs = CaseSensitive) const noexcept
    {
        return findString(*this, from, s, cs);
    }
    [[nodiscard]] sizetype lastIndexOf(Char c, CaseSensitivity cs = CaseSensitive) const noexcept
    {
        return lastIndexOf(c, -1, cs);
    }
    [[nodiscard]] sizetype lastIndexOf(Char c, sizetype from, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] sizetype lastIndexOf(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const
    {
        return lastIndexOf(s, size(), cs);
    }
    [[nodiscard]] sizetype lastIndexOf(Latin1StringView s, sizetype from, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] sizetype lastIndexOf(const String& s, CaseSensitivity cs = CaseSensitive) const
    {
        return lastIndexOf(s, size(), cs);
    }
    [[nodiscard]] sizetype lastIndexOf(const String& s, sizetype from, CaseSensitivity cs = CaseSensitive) const;

    [[nodiscard]] sizetype lastIndexOf(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
    {
        return lastIndexOf(s, size(), cs);
    }
    [[nodiscard]] sizetype lastIndexOf(StringView s, sizetype from, CaseSensitivity cs = CaseSensitive) const noexcept
    {
        return lastIndexOf(*this, from, s, cs);
    }

    [[nodiscard]] inline bool contains(Char c, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] inline bool contains(const String& s, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] inline bool contains(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] inline bool contains(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept;
    [[nodiscard]] sizetype count(Char c, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] sizetype count(const String& s, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] sizetype count(StringView s, CaseSensitivity cs = CaseSensitive) const;

#if QT_CONFIG(regularexpression)
    [[nodiscard]] sizetype indexOf(const RegularExpression& re, sizetype from = 0,
        RegularExpressionMatch* rmatch = nullptr) const;
#ifdef Q_QDOC
    [[nodiscard]] sizetype lastIndexOf(const RegularExpression& re, RegularExpressionMatch* rmatch = nullptr) const;
#else
    // prevent an ambiguity when called like this: lastIndexOf(re, 0)
    template <typename T = RegularExpressionMatch, std::enable_if_t<std::is_same_v<T, RegularExpressionMatch>, bool> = false>
    [[nodiscard]] sizetype lastIndexOf(const RegularExpression& re, T* rmatch = nullptr) const
    {
        return lastIndexOf(re, size(), rmatch);
    }
#endif
    [[nodiscard]] sizetype lastIndexOf(const RegularExpression& re, sizetype from,
        RegularExpressionMatch* rmatch = nullptr) const;
    [[nodiscard]] bool contains(const RegularExpression& re, RegularExpressionMatch* rmatch = nullptr) const;
    [[nodiscard]] sizetype count(const RegularExpression& re) const;
#endif

    enum SectionFlag {
        SectionDefault = 0x00,
        SectionSkipEmpty = 0x01,
        SectionIncludeLeadingSep = 0x02,
        SectionIncludeTrailingSep = 0x04,
        SectionCaseInsensitiveSeps = 0x08
    };
    Q_DECLARE_FLAGS(SectionFlags, SectionFlag)

        [[nodiscard]] inline String section(Char sep, sizetype start, sizetype end = -1, SectionFlags flags = SectionDefault) const;
    [[nodiscard]] String section(const String& in_sep, sizetype start, sizetype end = -1, SectionFlags flags = SectionDefault) const;
#if QT_CONFIG(regularexpression)
    [[nodiscard]] String section(const RegularExpression& re, sizetype start, sizetype end = -1, SectionFlags flags = SectionDefault) const;
#endif

#if QT_CORE_REMOVED_SINCE(6, 7)
    String left(sizetype n) const;
    String right(sizetype n) const;
    String mid(sizetype position, sizetype n = -1) const;

    String first(sizetype n) const;
    String last(sizetype n) const;
    String sliced(sizetype pos) const;
    String sliced(sizetype pos, sizetype n) const;
    String chopped(sizetype n) const;
#else
    [[nodiscard]] String left(sizetype n) const&
    {
        if (size_t(n) >= size_t(size()))
            return *this;
        return first(n);
    }
    [[nodiscard]] String left(sizetype n)&&
    {
        if (size_t(n) >= size_t(size()))
            return std::move(*this);
        return std::move(*this).first(n);
    }
    [[nodiscard]] String right(sizetype n) const&
    {
        if (size_t(n) >= size_t(size()))
            return *this;
        return last(n);
    }
    [[nodiscard]] String right(sizetype n)&&
    {
        if (size_t(n) >= size_t(size()))
            return std::move(*this);
        return std::move(*this).last(n);
    }
    [[nodiscard]] String mid(sizetype position, sizetype n = -1) const&;
    [[nodiscard]] String mid(sizetype position, sizetype n = -1)&&;

    [[nodiscard]] String first(sizetype n) const&
    {
        verify(0, n); return sliced(0, n);
    }
    [[nodiscard]] String last(sizetype n) const&
    {
        verify(0, n); return sliced(size() - n, n);
    }
    [[nodiscard]] String sliced(sizetype pos) const&
    {
        verify(pos, 0); return sliced(pos, size() - pos);
    }
    [[nodiscard]] String sliced(sizetype pos, sizetype n) const&
    {
        verify(pos, n); return String(begin() + pos, n);
    }
    [[nodiscard]] String chopped(sizetype n) const&
    {
        verify(0, n); return sliced(0, size() - n);
    }

    [[nodiscard]] String first(sizetype n)&&
    {
        verify(0, n);
        resize(n);      // may detach and allocate memory
        return std::move(*this);
    }
    [[nodiscard]] String last(sizetype n)&&
    {
        verify(0, n); return sliced_helper(*this, size() - n, n);
    }
    [[nodiscard]] String sliced(sizetype pos)&&
    {
        verify(pos, 0); return sliced_helper(*this, pos, size() - pos);
    }
    [[nodiscard]] String sliced(sizetype pos, sizetype n)&&
    {
        verify(pos, n); return sliced_helper(*this, pos, n);
    }
    [[nodiscard]] String chopped(sizetype n)&&
    {
        verify(0, n); return std::move(*this).first(size() - n);
    }
#endif
    bool startsWith(const String& s, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] bool startsWith(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
    {
        return startsWith(*this, s, cs);
    }
    bool startsWith(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const;
    bool startsWith(Char c, CaseSensitivity cs = CaseSensitive) const;

    bool endsWith(const String& s, CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]] bool endsWith(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
    {
        return endsWith(*this, s, cs);
    }
    bool endsWith(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const;
    bool endsWith(Char c, CaseSensitivity cs = CaseSensitive) const;

    bool isUpper() const;
    bool isLower() const;

    [[nodiscard]] String leftJustified(sizetype width, Char fill = u' ', bool trunc = false) const;
    [[nodiscard]] String rightJustified(sizetype width, Char fill = u' ', bool trunc = false) const;

#if !defined(Q_QDOC)
    [[nodiscard]] String toLower() const&
    {
        return toLower_helper(*this);
    }
    [[nodiscard]] String toLower()&&
    {
        return toLower_helper(*this);
    }
    [[nodiscard]] String toUpper() const&
    {
        return toUpper_helper(*this);
    }
    [[nodiscard]] String toUpper()&&
    {
        return toUpper_helper(*this);
    }
    [[nodiscard]] String toCaseFolded() const&
    {
        return toCaseFolded_helper(*this);
    }
    [[nodiscard]] String toCaseFolded()&&
    {
        return toCaseFolded_helper(*this);
    }
    [[nodiscard]] String trimmed() const&
    {
        return trimmed_helper(*this);
    }
    [[nodiscard]] String trimmed()&&
    {
        return trimmed_helper(*this);
    }
    [[nodiscard]] String simplified() const&
    {
        return simplified_helper(*this);
    }
    [[nodiscard]] String simplified()&&
    {
        return simplified_helper(*this);
    }
#else
    [[nodiscard]] String toLower() const;
    [[nodiscard]] String toUpper() const;
    [[nodiscard]] String toCaseFolded() const;
    [[nodiscard]] String trimmed() const;
    [[nodiscard]] String simplified() const;
#endif
    [[nodiscard]] String toHtmlEscaped() const;

    String& insert(sizetype i, Char c);
    String& insert(sizetype i, const Char* uc, sizetype len);
    inline String& insert(sizetype i, const String& s) { return insert(i, s.constData(), s.size()); }
    inline String& insert(sizetype i, StringView v) { return insert(i, v.data(), v.size()); }
    String& insert(sizetype i, Latin1StringView s);
    String& insert(sizetype i, QUtf8StringView s);

    String& append(Char c);
    String& append(const Char* uc, sizetype len);
    String& append(const String& s);
    inline String& append(StringView v) { return append(v.data(), v.size()); }
    String& append(Latin1StringView s);
    String& append(QUtf8StringView s);

    inline String& prepend(Char c) { return insert(0, c); }
    inline String& prepend(const Char* uc, sizetype len) { return insert(0, uc, len); }
    inline String& prepend(const String& s) { return insert(0, s); }
    inline String& prepend(StringView v) { return prepend(v.data(), v.size()); }
    inline String& prepend(Latin1StringView s) { return insert(0, s); }
    String& prepend(QUtf8StringView s) { return insert(0, s); }

    String& assign(AnyStringView s);
    inline String& assign(sizetype n, Char c)
    {
        Assert(n >= 0);
        return fill(c, n);
    }
    template <typename InputIterator, if_compatible_iterator<InputIterator> = true>
    String& assign(InputIterator first, InputIterator last)
    {
        using V = typename std::iterator_traits<InputIterator>::value_type;
        constexpr bool IsL1C = std::is_same_v<std::remove_cv_t<V>, QLatin1Char>;
        constexpr bool IsFwdIt = std::is_convertible_v<
            typename std::iterator_traits<InputIterator>::iterator_category,
            std::forward_iterator_tag
        >;

        if constexpr (is_contiguous_iterator_v<InputIterator>) {
            const auto p = q20::to_address(first);
            const auto len = sizetype(last - first);
            if constexpr (IsL1C)
                return assign(Latin1StringView(reinterpret_cast<const char*>(p), len));
            else if constexpr (sizeof(V) == 4)
                return assign_helper(p, len);
            else
                return assign(AnyStringView(p, len));
        }
        else if constexpr (sizeof(V) == 4) { // non-contiguous iterator, feed data piecemeal
            resize(0);
            if constexpr (IsFwdIt) {
                const sizetype requiredCapacity = 2 * std::distance(first, last);
                reserve(requiredCapacity);
            }
            while (first != last) {
                append(Char::fromUcs4(*first));
                ++first;
            }
            return *this;
        }
        else if constexpr (IsCompatibleChar8Type<V>::value) {
            assign_helper_char8(first, last);
            d.data()[d.size] = u'\0';
            return *this;
        }
        else {
            d.assign(first, last, [](Char ch) -> char16_t { return ch.unicode(); });
            d.data()[d.size] = u'\0';
            return *this;
        }
    }

    inline String& operator+=(Char c) { return append(c); }

    inline String& operator+=(const String& s) { return append(s); }
    inline String& operator+=(StringView v) { return append(v); }
    inline String& operator+=(Latin1StringView s) { return append(s); }
    String& operator+=(QUtf8StringView s) { return append(s); }

#if defined(QT_RESTRICTED_CAST_FROM_ASCII)
    template <sizetype N>
    String& insert(sizetype i, const char(&ch)[N]) { return insert(i, QUtf8StringView(ch)); }
    template <sizetype N>
    String& append(const char(&ch)[N]) { return append(QUtf8StringView(ch)); }
    template <sizetype N>
    String& prepend(const char(&ch)[N]) { return prepend(QUtf8StringView(ch)); }
    template <sizetype N>
    String& operator+=(const char(&ch)[N]) { return append(QUtf8StringView(ch)); }
#endif

    String& remove(sizetype i, sizetype len);
    String& remove(Char c, CaseSensitivity cs = CaseSensitive);
    String& remove(Latin1StringView s, CaseSensitivity cs = CaseSensitive);
    String& remove(const String& s, CaseSensitivity cs = CaseSensitive);

    String& removeAt(sizetype pos)
    {
        return size_t(pos) < size_t(size()) ? remove(pos, 1) : *this;
    }
    String& removeFirst() { return !isEmpty() ? remove(0, 1) : *this; }
    String& removeLast() { return !isEmpty() ? remove(size() - 1, 1) : *this; }

    template <typename Predicate>
    String& removeIf(Predicate pred)
    {
        removeIf_helper(pred);
        return *this;
    }

    String& replace(sizetype i, sizetype len, Char after);
    String& replace(sizetype i, sizetype len, const Char* s, sizetype slen);
    String& replace(sizetype i, sizetype len, const String& after);
    String& replace(Char before, Char after, CaseSensitivity cs = CaseSensitive);
    String& replace(const Char* before, sizetype blen, const Char* after, sizetype alen, CaseSensitivity cs = CaseSensitive);
    String& replace(Latin1StringView before, Latin1StringView after, CaseSensitivity cs = CaseSensitive);
    String& replace(Latin1StringView before, const String& after, CaseSensitivity cs = CaseSensitive);
    String& replace(const String& before, Latin1StringView after, CaseSensitivity cs = CaseSensitive);
    String& replace(const String& before, const String& after,
        CaseSensitivity cs = CaseSensitive);
    String& replace(Char c, const String& after, CaseSensitivity cs = CaseSensitive);
    String& replace(Char c, Latin1StringView after, CaseSensitivity cs = CaseSensitive);
#if QT_CONFIG(regularexpression)
    String& replace(const RegularExpression& re, const String& after);
    inline String& remove(const RegularExpression& re)
    {
        return replace(re, String());
    }
#endif

public:
    [[nodiscard]]
    StringList split(const String& sep, Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
        CaseSensitivity cs = CaseSensitive) const;
    [[nodiscard]]
    StringList split(Char sep, Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
        CaseSensitivity cs = CaseSensitive) const;
#ifndef QT_NO_REGULAREXPRESSION
    [[nodiscard]]
    StringList split(const RegularExpression& sep,
        Qt::SplitBehavior behavior = Qt::KeepEmptyParts) const;
#endif

    template <typename Needle, typename...Flags>
    [[nodiscard]] inline auto tokenize(Needle&& needle, Flags...flags) const&
        noexcept(noexcept(qTokenize(std::declval<const String&>(), std::forward<Needle>(needle), flags...)))
        -> decltype(qTokenize(*this, std::forward<Needle>(needle), flags...))
    {
        return qTokenize(qToStringViewIgnoringNull(*this), std::forward<Needle>(needle), flags...);
    }

    template <typename Needle, typename...Flags>
    [[nodiscard]] inline auto tokenize(Needle&& needle, Flags...flags) const&&
        noexcept(noexcept(qTokenize(std::declval<const String>(), std::forward<Needle>(needle), flags...)))
        -> decltype(qTokenize(std::move(*this), std::forward<Needle>(needle), flags...))
    {
        return qTokenize(std::move(*this), std::forward<Needle>(needle), flags...);
    }

    template <typename Needle, typename...Flags>
    [[nodiscard]] inline auto tokenize(Needle&& needle, Flags...flags) &&
        noexcept(noexcept(qTokenize(std::declval<String>(), std::forward<Needle>(needle), flags...)))
        -> decltype(qTokenize(std::move(*this), std::forward<Needle>(needle), flags...))
    {
        return qTokenize(std::move(*this), std::forward<Needle>(needle), flags...);
    }


    enum NormalizationForm {
        NormalizationForm_D,
        NormalizationForm_C,
        NormalizationForm_KD,
        NormalizationForm_KC
    };
    [[nodiscard]] String normalized(NormalizationForm mode, Char::UnicodeVersion version = Char::Unicode_Unassigned) const;

    [[nodiscard]] String repeated(sizetype times) const;

    const ushort* utf16() const; // ### Qt 7 char16_t

#if !defined(Q_QDOC)
    [[nodiscard]] QByteArray toLatin1() const&
    {
        return toLatin1_helper(*this);
    }
    [[nodiscard]] QByteArray toLatin1()&&
    {
        return toLatin1_helper_inplace(*this);
    }
    [[nodiscard]] QByteArray toUtf8() const&
    {
        return toUtf8_helper(*this);
    }
    [[nodiscard]] QByteArray toUtf8()&&
    {
        return toUtf8_helper(*this);
    }
    [[nodiscard]] QByteArray toLocal8Bit() const&
    {
        return toLocal8Bit_helper(isNull() ? nullptr : constData(), size());
    }
    [[nodiscard]] QByteArray toLocal8Bit()&&
    {
        return toLocal8Bit_helper(isNull() ? nullptr : constData(), size());
    }
#else
    [[nodiscard]] QByteArray toLatin1() const;
    [[nodiscard]] QByteArray toUtf8() const;
    [[nodiscard]] QByteArray toLocal8Bit() const;
#endif
    [[nodiscard]] QList<uint> toUcs4() const; // ### Qt 7 char32_t

    // note - this are all inline so we can benefit from strlen() compile time optimizations
    static String fromLatin1(QByteArrayView ba);
    Q_WEAK_OVERLOAD
        static inline String fromLatin1(const QByteArray& ba) { return fromLatin1(QByteArrayView(ba)); }
    static inline String fromLatin1(const char* str, sizetype size)
    {
        return fromLatin1(QByteArrayView(str, !str || size < 0 ? qstrlen(str) : size));
    }
    static String fromUtf8(QByteArrayView utf8);
    Q_WEAK_OVERLOAD
        static inline String fromUtf8(const QByteArray& ba) { return fromUtf8(QByteArrayView(ba)); }
    static inline String fromUtf8(const char* utf8, sizetype size)
    {
        return fromUtf8(QByteArrayView(utf8, !utf8 || size < 0 ? qstrlen(utf8) : size));
    }
#if defined(__cpp_char8_t) || defined(Q_QDOC)
    Q_WEAK_OVERLOAD
        static inline String fromUtf8(const char8_t* str)
    {
        return fromUtf8(reinterpret_cast<const char*>(str));
    }
    Q_WEAK_OVERLOAD
        static inline String fromUtf8(const char8_t* str, sizetype size)
    {
        return fromUtf8(reinterpret_cast<const char*>(str), size);
    }
#endif
    static String fromLocal8Bit(QByteArrayView ba);
    Q_WEAK_OVERLOAD
        static inline String fromLocal8Bit(const QByteArray& ba) { return fromLocal8Bit(QByteArrayView(ba)); }
    static inline String fromLocal8Bit(const char* str, sizetype size)
    {
        return fromLocal8Bit(QByteArrayView(str, !str || size < 0 ? qstrlen(str) : size));
    }
    static String fromUtf16(const char16_t*, sizetype size = -1);
    static String fromUcs4(const char32_t*, sizetype size = -1);
    static String fromRawData(const Char*, sizetype size);

#if QT_DEPRECATED_SINCE(6, 0)
    QT_DEPRECATED_VERSION_X_6_0("Use char16_t* overload.")
        static String fromUtf16(const ushort* str, sizetype size = -1)
    {
        return fromUtf16(reinterpret_cast<const char16_t*>(str), size);
    }
    QT_DEPRECATED_VERSION_X_6_0("Use char32_t* overload.")
        static String fromUcs4(const uint* str, sizetype size = -1)
    {
        return fromUcs4(reinterpret_cast<const char32_t*>(str), size);
    }
#endif

    inline sizetype toWCharArray(wchar_t* array) const;
    [[nodiscard]] static inline String fromWCharArray(const wchar_t* string, sizetype size = -1);

    String& setRawData(const Char* unicode, sizetype size);
    String& setUnicode(const Char* unicode, sizetype size);
    inline String& setUtf16(const ushort* utf16, sizetype size); // ### Qt 7 char16_t

    int compare(const String& s, CaseSensitivity cs = CaseSensitive) const noexcept;
    int compare(Latin1StringView other, CaseSensitivity cs = CaseSensitive) const noexcept;
    inline int compare(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept;
    int compare(Char ch, CaseSensitivity cs = CaseSensitive) const noexcept
    {
        return compare(StringView{ &ch, 1 }, cs);
    }

    static inline int compare(const String& s1, const String& s2,
        CaseSensitivity cs = CaseSensitive) noexcept
    {
        return s1.compare(s2, cs);
    }

    static inline int compare(const String& s1, Latin1StringView s2,
        CaseSensitivity cs = CaseSensitive) noexcept
    {
        return s1.compare(s2, cs);
    }
    static inline int compare(Latin1StringView s1, const String& s2,
        CaseSensitivity cs = CaseSensitive) noexcept
    {
        return -s2.compare(s1, cs);
    }
    static int compare(const String& s1, StringView s2, CaseSensitivity cs = CaseSensitive) noexcept
    {
        return s1.compare(s2, cs);
    }
    static int compare(StringView s1, const String& s2, CaseSensitivity cs = CaseSensitive) noexcept
    {
        return -s2.compare(s1, cs);
    }

    int localeAwareCompare(const String& s) const;
    inline int localeAwareCompare(StringView s) const;
    static int localeAwareCompare(const String& s1, const String& s2)
    {
        return s1.localeAwareCompare(s2);
    }

    static inline int localeAwareCompare(StringView s1, StringView s2);

    short toShort(bool* ok = nullptr, int base = 10) const
    {
        return toIntegral_helper<short>(*this, ok, base);
    }
    ushort toUShort(bool* ok = nullptr, int base = 10) const
    {
        return toIntegral_helper<ushort>(*this, ok, base);
    }
    int toInt(bool* ok = nullptr, int base = 10) const
    {
        return toIntegral_helper<int>(*this, ok, base);
    }
    uint toUInt(bool* ok = nullptr, int base = 10) const
    {
        return toIntegral_helper<uint>(*this, ok, base);
    }
    long toLong(bool* ok = nullptr, int base = 10) const
    {
        return toIntegral_helper<long>(*this, ok, base);
    }
    ulong toULong(bool* ok = nullptr, int base = 10) const
    {
        return toIntegral_helper<ulong>(*this, ok, base);
    }
    QT_CORE_INLINE_SINCE(6, 5)
        qlonglong toLongLong(bool* ok = nullptr, int base = 10) const;
    QT_CORE_INLINE_SINCE(6, 5)
        qulonglong toULongLong(bool* ok = nullptr, int base = 10) const;
    float toFloat(bool* ok = nullptr) const;
    double toDouble(bool* ok = nullptr) const;

    inline String& setNum(short, int base = 10);
    inline String& setNum(ushort, int base = 10);
    inline String& setNum(int, int base = 10);
    inline String& setNum(uint, int base = 10);
    inline String& setNum(long, int base = 10);
    inline String& setNum(ulong, int base = 10);
    String& setNum(qlonglong, int base = 10);
    String& setNum(qulonglong, int base = 10);
    inline String& setNum(float, char format = 'g', int precision = 6);
    String& setNum(double, char format = 'g', int precision = 6);

    static String number(int, int base = 10);
    static String number(uint, int base = 10);
    static String number(long, int base = 10);
    static String number(ulong, int base = 10);
    static String number(qlonglong, int base = 10);
    static String number(qulonglong, int base = 10);
    static String number(double, char format = 'g', int precision = 6);

    friend bool operator==(const String& s1, const String& s2) noexcept
    {
        return (s1.size() == s2.size()) && equalStrings(s1, s2);
    }
    friend bool operator< (const String& s1, const String& s2) noexcept
    {
        return compareStrings(s1, s2, CaseSensitive) < 0;
    }
    friend bool operator> (const String& s1, const String& s2) noexcept { return s2 < s1; }
    friend bool operator!=(const String& s1, const String& s2) noexcept { return !(s1 == s2); }
    friend bool operator<=(const String& s1, const String& s2) noexcept { return !(s1 > s2); }
    friend bool operator>=(const String& s1, const String& s2) noexcept { return !(s1 < s2); }

    friend bool operator==(const String& s1, Latin1StringView s2) noexcept
    {
        return (s1.size() == s2.size()) && equalStrings(s1, s2);
    }
    friend bool operator< (const String& s1, Latin1StringView s2) noexcept
    {
        return compareStrings(s1, s2, CaseSensitive) < 0;
    }
    friend bool operator> (const String& s1, Latin1StringView s2) noexcept
    {
        return compareStrings(s1, s2, CaseSensitive) > 0;
    }
    friend bool operator!=(const String& s1, Latin1StringView s2) noexcept { return !(s1 == s2); }
    friend bool operator<=(const String& s1, Latin1StringView s2) noexcept { return !(s1 > s2); }
    friend bool operator>=(const String& s1, Latin1StringView s2) noexcept { return !(s1 < s2); }

    friend bool operator==(Latin1StringView s1, const String& s2) noexcept { return s2 == s1; }
    friend bool operator< (Latin1StringView s1, const String& s2) noexcept { return s2 > s1; }
    friend bool operator> (Latin1StringView s1, const String& s2) noexcept { return s2 < s1; }
    friend bool operator!=(Latin1StringView s1, const String& s2) noexcept { return s2 != s1; }
    friend bool operator<=(Latin1StringView s1, const String& s2) noexcept { return s2 >= s1; }
    friend bool operator>=(Latin1StringView s1, const String& s2) noexcept { return s2 <= s1; }

    // Check isEmpty() instead of isNull() for backwards compatibility.
    friend bool operator==(const String& s1, std::nullptr_t) noexcept { return s1.isEmpty(); }
    friend bool operator!=(const String& s1, std::nullptr_t) noexcept { return !s1.isEmpty(); }
    friend bool operator< (const String&, std::nullptr_t) noexcept { return false; }
    friend bool operator> (const String& s1, std::nullptr_t) noexcept { return !s1.isEmpty(); }
    friend bool operator<=(const String& s1, std::nullptr_t) noexcept { return s1.isEmpty(); }
    friend bool operator>=(const String&, std::nullptr_t) noexcept { return true; }
    friend bool operator==(std::nullptr_t, const String& s2) noexcept { return s2 == nullptr; }
    friend bool operator!=(std::nullptr_t, const String& s2) noexcept { return s2 != nullptr; }
    friend bool operator< (std::nullptr_t, const String& s2) noexcept { return s2 > nullptr; }
    friend bool operator> (std::nullptr_t, const String& s2) noexcept { return s2 < nullptr; }
    friend bool operator<=(std::nullptr_t, const String& s2) noexcept { return s2 >= nullptr; }
    friend bool operator>=(std::nullptr_t, const String& s2) noexcept { return s2 <= nullptr; }

    friend bool operator==(const String& s1, const char16_t* s2) noexcept { return s1 == StringView(s2); }
    friend bool operator!=(const String& s1, const char16_t* s2) noexcept { return s1 != StringView(s2); }
    friend bool operator< (const String& s1, const char16_t* s2) noexcept { return s1 < StringView(s2); }
    friend bool operator> (const String& s1, const char16_t* s2) noexcept { return s1 > StringView(s2); }
    friend bool operator<=(const String& s1, const char16_t* s2) noexcept { return s1 <= StringView(s2); }
    friend bool operator>=(const String& s1, const char16_t* s2) noexcept { return s1 >= StringView(s2); }

    friend bool operator==(const char16_t* s1, const String& s2) noexcept { return s2 == s1; }
    friend bool operator!=(const char16_t* s1, const String& s2) noexcept { return s2 != s1; }
    friend bool operator< (const char16_t* s1, const String& s2) noexcept { return s2 > s1; }
    friend bool operator> (const char16_t* s1, const String& s2) noexcept { return s2 < s1; }
    friend bool operator<=(const char16_t* s1, const String& s2) noexcept { return s2 >= s1; }
    friend bool operator>=(const char16_t* s1, const String& s2) noexcept { return s2 <= s1; }

    // Char <> String
    friend inline bool operator==(Char lhs, const String& rhs) noexcept
    {
        return rhs.size() == 1 && lhs == rhs.front();
    }
    friend inline bool operator< (Char lhs, const String& rhs) noexcept
    {
        return compare_helper(&lhs, 1, rhs.data(), rhs.size()) < 0;
    }
    friend inline bool operator> (Char lhs, const String& rhs) noexcept
    {
        return compare_helper(&lhs, 1, rhs.data(), rhs.size()) > 0;
    }

    friend inline bool operator!=(Char lhs, const String& rhs) noexcept { return !(lhs == rhs); }
    friend inline bool operator<=(Char lhs, const String& rhs) noexcept { return !(lhs > rhs); }
    friend inline bool operator>=(Char lhs, const String& rhs) noexcept { return !(lhs < rhs); }

    friend inline bool operator==(const String& lhs, Char rhs) noexcept { return   rhs == lhs; }
    friend inline bool operator!=(const String& lhs, Char rhs) noexcept { return !(rhs == lhs); }
    friend inline bool operator< (const String& lhs, Char rhs) noexcept { return   rhs > lhs; }
    friend inline bool operator> (const String& lhs, Char rhs) noexcept { return   rhs < lhs; }
    friend inline bool operator<=(const String& lhs, Char rhs) noexcept { return !(rhs < lhs); }
    friend inline bool operator>=(const String& lhs, Char rhs) noexcept { return !(rhs > lhs); }

    // ASCII compatibility
#if defined(QT_RESTRICTED_CAST_FROM_ASCII)
    template <sizetype N>
    inline String(const char(&ch)[N])
        : String(fromUtf8(ch))
    {}
    template <sizetype N>
    String(char(&)[N]) = delete;
    template <sizetype N>
    inline String& operator=(const char(&ch)[N])
    {
        return (*this = fromUtf8(ch, N - 1));
    }
    template <sizetype N>
    String& operator=(char(&)[N]) = delete;
#endif
#if !defined(QT_NO_CAST_FROM_ASCII) && !defined(QT_RESTRICTED_CAST_FROM_ASCII)
    QT_ASCII_CAST_WARN inline String(const char* ch)
        : String(fromUtf8(ch))
    {}
    QT_ASCII_CAST_WARN inline String(const QByteArray& a)
        : String(fromUtf8(a))
    {}
    QT_ASCII_CAST_WARN inline String& operator=(const char* ch)
    {
        if (!ch) {
            clear();
            return *this;
        }
        return assign(ch);
    }
    QT_ASCII_CAST_WARN inline String& operator=(const QByteArray& a)
    {
        if (a.isNull()) {
            clear();
            return *this;
        }
        return assign(a);
    }
    // these are needed, so it compiles with STL support enabled
    QT_ASCII_CAST_WARN inline String& prepend(const char* s)
    {
        return prepend(QUtf8StringView(s));
    }
    QT_ASCII_CAST_WARN inline String& prepend(const QByteArray& s)
    {
        return prepend(QUtf8StringView(s));
    }
    QT_ASCII_CAST_WARN inline String& append(const char* s)
    {
        return append(QUtf8StringView(s));
    }
    QT_ASCII_CAST_WARN inline String& append(const QByteArray& s)
    {
        return append(QUtf8StringView(s));
    }
    QT_ASCII_CAST_WARN inline String& insert(sizetype i, const char* s)
    {
        return insert(i, QUtf8StringView(s));
    }
    QT_ASCII_CAST_WARN inline String& insert(sizetype i, const QByteArray& s)
    {
        return insert(i, QUtf8StringView(s));
    }
    QT_ASCII_CAST_WARN inline String& operator+=(const char* s)
    {
        return append(QUtf8StringView(s));
    }
    QT_ASCII_CAST_WARN inline String& operator+=(const QByteArray& s)
    {
        return append(QUtf8StringView(s));
    }

    QT_ASCII_CAST_WARN inline bool operator==(const char* s) const;
    QT_ASCII_CAST_WARN inline bool operator!=(const char* s) const;
    QT_ASCII_CAST_WARN inline bool operator<(const char* s) const;
    QT_ASCII_CAST_WARN inline bool operator<=(const char* s) const;
    QT_ASCII_CAST_WARN inline bool operator>(const char* s) const;
    QT_ASCII_CAST_WARN inline bool operator>=(const char* s) const;

    QT_ASCII_CAST_WARN inline bool operator==(const QByteArray& s) const;
    QT_ASCII_CAST_WARN inline bool operator!=(const QByteArray& s) const;
    QT_ASCII_CAST_WARN inline bool operator<(const QByteArray& s) const;
    QT_ASCII_CAST_WARN inline bool operator>(const QByteArray& s) const;
    QT_ASCII_CAST_WARN inline bool operator<=(const QByteArray& s) const;
    QT_ASCII_CAST_WARN inline bool operator>=(const QByteArray& s) const;

    QT_ASCII_CAST_WARN friend bool operator==(const char* s1, const String& s2)
    {
        return String::compare_helper(s2.constData(), s2.size(), s1, -1) == 0;
    }
    QT_ASCII_CAST_WARN friend bool operator!=(const char* s1, const String& s2)
    {
        return String::compare_helper(s2.constData(), s2.size(), s1, -1) != 0;
    }
    QT_ASCII_CAST_WARN friend bool operator< (const char* s1, const String& s2)
    {
        return String::compare_helper(s2.constData(), s2.size(), s1, -1) > 0;
    }
    QT_ASCII_CAST_WARN friend bool operator> (const char* s1, const String& s2)
    {
        return String::compare_helper(s2.constData(), s2.size(), s1, -1) < 0;
    }
    QT_ASCII_CAST_WARN friend bool operator<=(const char* s1, const String& s2)
    {
        return String::compare_helper(s2.constData(), s2.size(), s1, -1) >= 0;
    }
    QT_ASCII_CAST_WARN friend bool operator>=(const char* s1, const String& s2)
    {
        return String::compare_helper(s2.constData(), s2.size(), s1, -1) <= 0;
    }
#endif

    typedef Char* iterator;
    typedef const Char* const_iterator;
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    inline iterator begin();
    inline const_iterator begin() const;
    inline const_iterator cbegin() const;
    inline const_iterator constBegin() const;
    inline iterator end();
    inline const_iterator end() const;
    inline const_iterator cend() const;
    inline const_iterator constEnd() const;
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

    // STL compatibility
    typedef sizetype size_type;
    typedef qptrdiff difference_type;
    typedef const Char& const_reference;
    typedef Char& reference;
    typedef Char* pointer;
    typedef const Char* const_pointer;
    typedef Char value_type;
    inline void push_back(Char c) { append(c); }
    inline void push_back(const String& s) { append(s); }
    inline void push_front(Char c) { prepend(c); }
    inline void push_front(const String& s) { prepend(s); }
    void shrink_to_fit() { squeeze(); }
    iterator erase(const_iterator first, const_iterator last);
    inline iterator erase(const_iterator it) { return erase(it, it + 1); }

    static inline String fromStdString(const std::string& s);
    inline std::string toStdString() const;
    static inline String fromStdWString(const std::wstring& s);
    inline std::wstring toStdWString() const;

    static inline String fromStdU16String(const std::u16string& s);
    inline std::u16string toStdU16String() const;
    static inline String fromStdU32String(const std::u32string& s);
    inline std::u32string toStdU32String() const;

    Q_IMPLICIT inline operator std::u16string_view() const noexcept;

#if defined(Q_OS_DARWIN) || defined(Q_QDOC)
    static String fromCFString(CFStringRef string);
    CFStringRef toCFString() const Q_DECL_CF_RETURNS_RETAINED;
    static String fromNSString(const NSString* string);
    NSString* toNSString() const Q_DECL_NS_RETURNS_AUTORELEASED;
#endif

#if defined(Q_OS_WASM) || defined(Q_QDOC)
    static String fromEcmaString(emscripten::val jsString);
    emscripten::val toEcmaString() const;
#endif

    inline bool isNull() const { return d->isNull(); }

    bool isRightToLeft() const;
    [[nodiscard]] bool isValidUtf16() const noexcept
    {
        return StringView(*this).isValidUtf16();
    }

    String(sizetype size, Qt::Initialization);
    explicit String(DataPointer&& dd) : d(std::move(dd)) {}

private:
#if defined(QT_NO_CAST_FROM_ASCII)
    String& operator+=(const char* s);
    String& operator+=(const QByteArray& s);
    String(const char* ch);
    String(const QByteArray& a);
    String& operator=(const char* ch);
    String& operator=(const QByteArray& a);
#endif

    DataPointer d;
    static const char16_t _empty;

    void reallocData(sizetype alloc, QArrayData::AllocationOption option);
    void reallocGrowData(sizetype n);
    // ### remove once AnyStringView supports UTF-32:
    String& assign_helper(const char32_t* data, sizetype len);
    // Defined in Stringconverter.h
    template <typename InputIterator>
    void assign_helper_char8(InputIterator first, InputIterator last);
    static int compare_helper(const Char* data1, sizetype length1,
        const Char* data2, sizetype length2,
        CaseSensitivity cs = CaseSensitive) noexcept;
    static int compare_helper(const Char* data1, sizetype length1,
        const char* data2, sizetype length2,
        CaseSensitivity cs = CaseSensitive) noexcept;
    static int localeAwareCompare_helper(const Char* data1, sizetype length1,
        const Char* data2, sizetype length2);
    static String sliced_helper(String& str, sizetype pos, sizetype n);
    static String toLower_helper(const String& str);
    static String toLower_helper(String& str);
    static String toUpper_helper(const String& str);
    static String toUpper_helper(String& str);
    static String toCaseFolded_helper(const String& str);
    static String toCaseFolded_helper(String& str);
    static String trimmed_helper(const String& str);
    static String trimmed_helper(String& str);
    static String simplified_helper(const String& str);
    static String simplified_helper(String& str);
    static QByteArray toLatin1_helper(const String&);
    static QByteArray toLatin1_helper_inplace(String&);
    static QByteArray toUtf8_helper(const String&);
    static QByteArray toLocal8Bit_helper(const Char* data, sizetype size);
#if QT_CORE_REMOVED_SINCE(6, 6)
    static sizetype toUcs4_helper(const ushort* uc, sizetype length, uint* out);
#endif
    static sizetype toUcs4_helper(const char16_t* uc, sizetype length, char32_t* out);
    static qlonglong toIntegral_helper(StringView string, bool* ok, int base);
    static qulonglong toIntegral_helper(StringView string, bool* ok, uint base);
    template <typename Predicate>
    sizetype removeIf_helper(Predicate pred)
    {
        const sizetype result = d->eraseIf(pred);
        if (result > 0)
            d.data()[d.size] = u'\0';
        return result;
    }

    friend class StringView;
    friend class QByteArray;
    friend struct QAbstractConcatenable;
    template <typename T> friend sizetype erase(String& s, const T& t);
    template <typename Predicate> friend sizetype erase_if(String& s, Predicate pred);

    template <typename T> static
        T toIntegral_helper(StringView string, bool* ok, int base)
    {
        using Int64 = typename std::conditional<std::is_unsigned<T>::value, qulonglong, qlonglong>::type;
        using Int32 = typename std::conditional<std::is_unsigned<T>::value, uint, int>::type;

        // we select the right overload by casting base to int or uint
        Int64 val = toIntegral_helper(string, ok, Int32(base));
        if (T(val) != val) {
            if (ok)
                *ok = false;
            val = 0;
        }
        return T(val);
    }

     constexpr void verify([[maybe_unused]] sizetype pos = 0,
        [[maybe_unused]] sizetype n = 1) const
    {
        Assert(pos >= 0);
        Assert(pos <= d.size);
        Assert(n >= 0);
        Assert(n <= d.size - pos);
    }

public:
    inline DataPointer& data_ptr() { return d; }
    inline const DataPointer& data_ptr() const { return d; }
};

//
// Latin1StringView inline members that require QUtf8StringView:
//

int Latin1StringView::compare(QUtf8StringView other, CaseSensitivity cs) const noexcept
{
    return compareStrings(*this, other, cs);
}

//
// Latin1StringView inline members that require String:
//

String Latin1StringView::toString() const { return *this; }

//
// StringView inline members that require QUtf8StringView:
//

int StringView::compare(QUtf8StringView other, CaseSensitivity cs) const noexcept
{
    return compareStrings(*this, other, cs);
}

//
// StringView inline members that require String:
//

String StringView::toString() const
{
    return String(data(), size());
}

qint64 StringView::toLongLong(bool* ok, int base) const
{
    return String::toIntegral_helper<qint64>(*this, ok, base);
}
quint64 StringView::toULongLong(bool* ok, int base) const
{
    return String::toIntegral_helper<quint64>(*this, ok, base);
}
long StringView::toLong(bool* ok, int base) const
{
    return String::toIntegral_helper<long>(*this, ok, base);
}
ulong StringView::toULong(bool* ok, int base) const
{
    return String::toIntegral_helper<ulong>(*this, ok, base);
}
int StringView::toInt(bool* ok, int base) const
{
    return String::toIntegral_helper<int>(*this, ok, base);
}
uint StringView::toUInt(bool* ok, int base) const
{
    return String::toIntegral_helper<uint>(*this, ok, base);
}
short StringView::toShort(bool* ok, int base) const
{
    return String::toIntegral_helper<short>(*this, ok, base);
}
ushort StringView::toUShort(bool* ok, int base) const
{
    return String::toIntegral_helper<ushort>(*this, ok, base);
}

//
// QUtf8StringView inline members that require StringView:
//

template <bool UseChar8T>
int QBasicUtf8StringView<UseChar8T>::compare(StringView other, CaseSensitivity cs) const noexcept
{
    return compareStrings(*this, other, cs);
}


//
// QUtf8StringView inline members that require String:
//

template <bool UseChar8T>
String QBasicUtf8StringView<UseChar8T>::toString() const
{
    return String::fromUtf8(data(), size());
}

template<bool UseChar8T>
[[nodiscard]] int QBasicUtf8StringView<UseChar8T>::compare(Latin1StringView other,
    CaseSensitivity cs) const noexcept
{
    return compareStrings(*this, other, cs);
}

//
// AnyStringView inline members that require String:
//

AnyStringView::AnyStringView(const QByteArray& str) noexcept
    : AnyStringView{ str.isNull() ? nullptr : str.data(), str.size() } {}
AnyStringView::AnyStringView(const String& str) noexcept
    : AnyStringView{ str.isNull() ? nullptr : str.data(), str.size() } {}

String AnyStringView::toString() const
{
    return convertToString(*this);
}

//
// String inline members
//
String::String(Latin1StringView latin1)
{
    *this = String::fromLatin1(latin1.data(), latin1.size());
}
const Char String::at(sizetype i) const
{
    verify(i, 1); return Char(d.data()[i]);
}
const Char String::operator[](sizetype i) const
{
    verify(i, 1); return Char(d.data()[i]);
}
const Char* String::unicode() const
{
    return data();
}
const Char* String::data() const
{
#if QT5_NULL_STRINGS == 1
    return reinterpret_cast<const Char*>(d.data() ? d.data() : &_empty);
#else
    return reinterpret_cast<const Char*>(d.data());
#endif
}
Char* String::data()
{
    detach();
    Assert(d.data());
    return reinterpret_cast<Char*>(d.data());
}
const Char* String::constData() const
{
    return data();
}
void String::detach()
{
    if (d->needsDetach()) reallocData(d.size, QArrayData::KeepSize);
}
bool String::isDetached() const
{
    return !d->isShared();
}
void String::clear()
{
    if (!isNull()) *this = String();
}
String::String(const String& other) noexcept : d(other.d)
{ }
sizetype String::capacity() const { return sizetype(d->constAllocatedCapacity()); }
String& String::setNum(short n, int base)
{
    return setNum(qlonglong(n), base);
}
String& String::setNum(ushort n, int base)
{
    return setNum(qulonglong(n), base);
}
String& String::setNum(int n, int base)
{
    return setNum(qlonglong(n), base);
}
String& String::setNum(uint n, int base)
{
    return setNum(qulonglong(n), base);
}
String& String::setNum(long n, int base)
{
    return setNum(qlonglong(n), base);
}
String& String::setNum(ulong n, int base)
{
    return setNum(qulonglong(n), base);
}
String& String::setNum(float n, char f, int prec)
{
    return setNum(double(n), f, prec);
}
String String::arg(int a, int fieldWidth, int base, Char fillChar) const
{
    return arg(qlonglong(a), fieldWidth, base, fillChar);
}
String String::arg(uint a, int fieldWidth, int base, Char fillChar) const
{
    return arg(qulonglong(a), fieldWidth, base, fillChar);
}
String String::arg(long a, int fieldWidth, int base, Char fillChar) const
{
    return arg(qlonglong(a), fieldWidth, base, fillChar);
}
String String::arg(ulong a, int fieldWidth, int base, Char fillChar) const
{
    return arg(qulonglong(a), fieldWidth, base, fillChar);
}
String String::arg(short a, int fieldWidth, int base, Char fillChar) const
{
    return arg(qlonglong(a), fieldWidth, base, fillChar);
}
String String::arg(ushort a, int fieldWidth, int base, Char fillChar) const
{
    return arg(qulonglong(a), fieldWidth, base, fillChar);
}

String String::section(Char asep, sizetype astart, sizetype aend, SectionFlags aflags) const
{
    return section(String(asep), astart, aend, aflags);
}

QT_WARNING_PUSH
QT_WARNING_DISABLE_MSVC(4127)   // "conditional expression is constant"
QT_WARNING_DISABLE_INTEL(111)   // "statement is unreachable"

sizetype String::toWCharArray(wchar_t* array) const
{
    return qToStringViewIgnoringNull(*this).toWCharArray(array);
}

sizetype StringView::toWCharArray(wchar_t* array) const
{
    if (sizeof(wchar_t) == sizeof(Char)) {
        if (auto src = data())
            memcpy(array, src, sizeof(Char) * size());
        return size();
    }
    else {
        return String::toUcs4_helper(utf16(), size(), reinterpret_cast<char32_t*>(array));
    }
}

QT_WARNING_POP

String String::fromWCharArray(const wchar_t* string, sizetype size)
{
    return sizeof(wchar_t) == sizeof(Char) ? fromUtf16(reinterpret_cast<const char16_t*>(string), size)
        : fromUcs4(reinterpret_cast<const char32_t*>(string), size);
}

constexpr String::String() noexcept {}
String::~String() {}

void String::reserve(sizetype asize)
{
    if (d->needsDetach() || asize >= capacity() - d.freeSpaceAtBegin())
        reallocData(qMax(asize, size()), QArrayData::KeepSize);
    if (d->constAllocatedCapacity())
        d->setFlag(Data::CapacityReserved);
}

void String::squeeze()
{
    if (!d.isMutable())
        return;
    if (d->needsDetach() || size() < capacity())
        reallocData(d.size, QArrayData::KeepSize);
    if (d->constAllocatedCapacity())
        d->clearFlag(Data::CapacityReserved);
}

String& String::setUtf16(const ushort* autf16, sizetype asize)
{
    return setUnicode(reinterpret_cast<const Char*>(autf16), asize);
}
Char& String::operator[](sizetype i)
{
    verify(i, 1); return data()[i];
}
Char& String::front() { return operator[](0); }
Char& String::back() { return operator[](size() - 1); }
String::iterator String::begin()
{
    detach(); return reinterpret_cast<Char*>(d.data());
}
String::const_iterator String::begin() const
{
    return reinterpret_cast<const Char*>(d.data());
}
String::const_iterator String::cbegin() const
{
    return reinterpret_cast<const Char*>(d.data());
}
String::const_iterator String::constBegin() const
{
    return reinterpret_cast<const Char*>(d.data());
}
String::iterator String::end()
{
    detach(); return reinterpret_cast<Char*>(d.data() + d.size);
}
String::const_iterator String::end() const
{
    return reinterpret_cast<const Char*>(d.data() + d.size);
}
String::const_iterator String::cend() const
{
    return reinterpret_cast<const Char*>(d.data() + d.size);
}
String::const_iterator String::constEnd() const
{
    return reinterpret_cast<const Char*>(d.data() + d.size);
}
bool String::contains(const String& s, CaseSensitivity cs) const
{
    return indexOf(s, 0, cs) != -1;
}
bool String::contains(Latin1StringView s, CaseSensitivity cs) const
{
    return indexOf(s, 0, cs) != -1;
}
bool String::contains(Char c, CaseSensitivity cs) const
{
    return indexOf(c, 0, cs) != -1;
}
bool String::contains(StringView s, CaseSensitivity cs) const noexcept
{
    return indexOf(s, 0, cs) != -1;
}

#if !defined(QT_NO_CAST_FROM_ASCII) && !defined(QT_RESTRICTED_CAST_FROM_ASCII)
bool String::operator==(const char* s) const
{
    return String::compare_helper(constData(), size(), s, -1) == 0;
}
bool String::operator!=(const char* s) const
{
    return String::compare_helper(constData(), size(), s, -1) != 0;
}
bool String::operator<(const char* s) const
{
    return String::compare_helper(constData(), size(), s, -1) < 0;
}
bool String::operator>(const char* s) const
{
    return String::compare_helper(constData(), size(), s, -1) > 0;
}
bool String::operator<=(const char* s) const
{
    return String::compare_helper(constData(), size(), s, -1) <= 0;
}
bool String::operator>=(const char* s) const
{
    return String::compare_helper(constData(), size(), s, -1) >= 0;
}

//
// Latin1StringView inline members that require String:
//
QT_ASCII_CAST_WARN bool Latin1StringView::operator==(const char* s) const
{
    return String::fromUtf8(s) == *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator!=(const char* s) const
{
    return String::fromUtf8(s) != *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator<(const char* s) const
{
    return String::fromUtf8(s) > *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator>(const char* s) const
{
    return String::fromUtf8(s) < *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator<=(const char* s) const
{
    return String::fromUtf8(s) >= *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator>=(const char* s) const
{
    return String::fromUtf8(s) <= *this;
}

QT_ASCII_CAST_WARN bool Latin1StringView::operator==(const QByteArray& s) const
{
    return String::fromUtf8(s) == *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator!=(const QByteArray& s) const
{
    return String::fromUtf8(s) != *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator<(const QByteArray& s) const
{
    return String::fromUtf8(s) > *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator>(const QByteArray& s) const
{
    return String::fromUtf8(s) < *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator<=(const QByteArray& s) const
{
    return String::fromUtf8(s) >= *this;
}
QT_ASCII_CAST_WARN bool Latin1StringView::operator>=(const QByteArray& s) const
{
    return String::fromUtf8(s) <= *this;
}

QT_ASCII_CAST_WARN bool String::operator==(const QByteArray& s) const
{
    return String::compare_helper(constData(), size(), s.constData(), s.size()) == 0;
}
QT_ASCII_CAST_WARN bool String::operator!=(const QByteArray& s) const
{
    return String::compare_helper(constData(), size(), s.constData(), s.size()) != 0;
}
QT_ASCII_CAST_WARN bool String::operator<(const QByteArray& s) const
{
    return String::compare_helper(constData(), size(), s.constData(), s.size()) < 0;
}
QT_ASCII_CAST_WARN bool String::operator>(const QByteArray& s) const
{
    return String::compare_helper(constData(), size(), s.constData(), s.size()) > 0;
}
QT_ASCII_CAST_WARN bool String::operator<=(const QByteArray& s) const
{
    return String::compare_helper(constData(), size(), s.constData(), s.size()) <= 0;
}
QT_ASCII_CAST_WARN bool String::operator>=(const QByteArray& s) const
{
    return String::compare_helper(constData(), size(), s.constData(), s.size()) >= 0;
}

bool QByteArray::operator==(const String& s) const
{
    return String::compare_helper(s.constData(), s.size(), constData(), size()) == 0;
}
bool QByteArray::operator!=(const String& s) const
{
    return String::compare_helper(s.constData(), s.size(), constData(), size()) != 0;
}
bool QByteArray::operator<(const String& s) const
{
    return String::compare_helper(s.constData(), s.size(), constData(), size()) > 0;
}
bool QByteArray::operator>(const String& s) const
{
    return String::compare_helper(s.constData(), s.size(), constData(), size()) < 0;
}
bool QByteArray::operator<=(const String& s) const
{
    return String::compare_helper(s.constData(), s.size(), constData(), size()) >= 0;
}
bool QByteArray::operator>=(const String& s) const
{
    return String::compare_helper(s.constData(), s.size(), constData(), size()) <= 0;
}
#endif // !defined(QT_NO_CAST_FROM_ASCII) && !defined(QT_RESTRICTED_CAST_FROM_ASCII)

#if !defined(QT_USE_FAST_OPERATOR_PLUS) && !defined(QT_USE_StringBUILDER)
inline String operator+(const String& s1, const String& s2)
{
    String t(s1); t += s2; return t;
}
inline String operator+(String&& lhs, const String& rhs)
{
    return std::move(lhs += rhs);
}
inline String operator+(const String& s1, Char s2)
{
    String t(s1); t += s2; return t;
}
inline String operator+(String&& lhs, Char rhs)
{
    return std::move(lhs += rhs);
}
inline String operator+(Char s1, const String& s2)
{
    String t(s1); t += s2; return t;
}
#  if !defined(QT_NO_CAST_FROM_ASCII) && !defined(QT_RESTRICTED_CAST_FROM_ASCII)
QT_ASCII_CAST_WARN inline String operator+(const String& s1, const char* s2)
{
    String t(s1); t += QUtf8StringView(s2); return t;
}
QT_ASCII_CAST_WARN inline String operator+(String&& lhs, const char* rhs)
{
    QT_IGNORE_DEPRECATIONS(return std::move(lhs += rhs);)
}
QT_ASCII_CAST_WARN inline String operator+(const char* s1, const String& s2)
{
    String t = String::fromUtf8(s1); t += s2; return t;
}
QT_ASCII_CAST_WARN inline String operator+(const QByteArray& ba, const String& s)
{
    String t = String::fromUtf8(ba); t += s; return t;
}
QT_ASCII_CAST_WARN inline String operator+(const String& s, const QByteArray& ba)
{
    String t(s); t += QUtf8StringView(ba); return t;
}
QT_ASCII_CAST_WARN inline String operator+(String&& lhs, const QByteArray& rhs)
{
    QT_IGNORE_DEPRECATIONS(return std::move(lhs += rhs);)
}
#  endif // QT_NO_CAST_FROM_ASCII
#endif // QT_USE_StringBUILDER

std::string String::toStdString() const
{
    return toUtf8().toStdString();
}

String String::fromStdString(const std::string& s)
{
    return fromUtf8(s.data(), sizetype(s.size()));
}

std::wstring String::toStdWString() const
{
    std::wstring str;
    str.resize(size());
    str.resize(toWCharArray(str.data()));
    return str;
}

String String::fromStdWString(const std::wstring& s)
{
    return fromWCharArray(s.data(), sizetype(s.size()));
}

String String::fromStdU16String(const std::u16string& s)
{
    return fromUtf16(s.data(), sizetype(s.size()));
}

std::u16string String::toStdU16String() const
{
    return std::u16string(reinterpret_cast<const char16_t*>(data()), size());
}

String String::fromStdU32String(const std::u32string& s)
{
    return fromUcs4(s.data(), sizetype(s.size()));
}

std::u32string String::toStdU32String() const
{
    std::u32string u32str(size(), char32_t(0));
    const sizetype len = toUcs4_helper(reinterpret_cast<const char16_t*>(data()),
        size(), u32str.data());
    u32str.resize(len);
    return u32str;
}

String::operator std::u16string_view() const noexcept
{
    return std::u16string_view(d.data(), size_t(d.size));
}

#if !defined(QT_NO_DATASTREAM) || defined(QT_BOOTSTRAPPED)
 QDataStream& operator<<(QDataStream&, const String&);
 QDataStream& operator>>(QDataStream&, String&);
#endif

Q_DECLARE_SHARED(String)
Q_DECLARE_OPERATORS_FOR_FLAGS(String::SectionFlags)

int String::compare(StringView s, CaseSensitivity cs) const noexcept
{
    return -s.compare(*this, cs);
}

int String::localeAwareCompare(StringView s) const
{
    return localeAwareCompare_helper(constData(), size(), s.constData(), s.size());
}
int String::localeAwareCompare(StringView s1, StringView s2)
{
    return localeAwareCompare_helper(s1.constData(), s1.size(), s2.constData(), s2.size());
}
int StringView::localeAwareCompare(StringView other) const
{
    return String::localeAwareCompare(*this, other);
}

#if QT_CORE_INLINE_IMPL_SINCE(6, 5)
qint64 String::toLongLong(bool* ok, int base) const
{
    return toIntegral_helper<qlonglong>(*this, ok, base);
}

quint64 String::toULongLong(bool* ok, int base) const
{
    return toIntegral_helper<qulonglong>(*this, ok, base);
}
#endif

namespace QtPrivate {
    // used by qPrintable() and qUtf8Printable() macros
    inline const String& asString(const String& s) { return s; }
    inline String&& asString(String&& s) { return std::move(s); }
}

#ifndef qPrintable
#  define qPrintable(string) asString(string).toLocal8Bit().constData()
#endif

#ifndef qUtf8Printable
#  define qUtf8Printable(string) asString(string).toUtf8().constData()
#endif

/*
    Wrap String::utf16() with enough casts to allow passing it
    to String::asprintf("%ls") without warnings.
*/
#ifndef qUtf16Printable
#  define qUtf16Printable(string) \
    static_cast<const wchar_t*>(static_cast<const void*>(asString(string).utf16()))
#endif

//
// StringView::arg() implementation
//

namespace QtPrivate {

    struct ArgBase {
        enum Tag : uchar { L1, U8, U16 } tag;
    };

    struct StringViewArg : ArgBase {
        StringView string;
        StringViewArg() = default;
        constexpr explicit StringViewArg(StringView v) noexcept : ArgBase{ U16 }, string{ v } {}
    };

    struct QLatin1StringArg : ArgBase {
        Latin1StringView string;
        QLatin1StringArg() = default;
        constexpr explicit QLatin1StringArg(Latin1StringView v) noexcept : ArgBase{ L1 }, string{ v } {}
    };

    [[nodiscard]]  String argToString(StringView pattern, size_t n, const ArgBase** args);
    [[nodiscard]]  String argToString(Latin1StringView pattern, size_t n, const ArgBase** args);

    template <typename StringView, typename...Args>
    [[nodiscard]] always_inline String argToStringDispatch(StringView pattern, const Args &...args)
    {
        const ArgBase* argBases[] = { &args..., /* avoid zero-sized array */ nullptr };
        return argToString(pattern, sizeof...(Args), argBases);
    }

    inline StringViewArg   StringLikeToArg(const String& s) noexcept { return StringViewArg{ qToStringViewIgnoringNull(s) }; }
    constexpr inline StringViewArg   StringLikeToArg(StringView s) noexcept { return StringViewArg{ s }; }
    inline StringViewArg   StringLikeToArg(const Char& c) noexcept { return StringViewArg{ StringView{&c, 1} }; }
    constexpr inline QLatin1StringArg StringLikeToArg(Latin1StringView s) noexcept { return QLatin1StringArg{ s }; }

} // namespace QtPrivate

template <typename...Args>
always_inline
String StringView::arg(Args &&...args) const
{
    return argToStringDispatch(*this, StringLikeToArg(args)...);
}

template <typename...Args>
always_inline
String Latin1StringView::arg(Args &&...args) const
{
    return argToStringDispatch(*this, StringLikeToArg(args)...);
}

template <typename T>
sizetype erase(String& s, const T& t)
{
    return s.removeIf_helper([&t](const auto& e) { return t == e; });
}

template <typename Predicate>
sizetype erase_if(String& s, Predicate pred)
{
    return s.removeIf_helper(pred);
}

namespace Qt {
    inline namespace Literals {
        inline namespace StringLiterals {
            inline String operator""_s(const char16_t* str, size_t size) noexcept
            {
                return String(StringPrivate(nullptr, const_cast<char16_t*>(str), sizetype(size)));
            }

        } // StringLiterals
    } // Literals
} // Qt

inline namespace QtLiterals {
#if QT_DEPRECATED_SINCE(6, 8)

    QT_DEPRECATED_VERSION_X_6_8("Use _s from Qt::StringLiterals namespace instead.")
        inline String operator""_qs(const char16_t* str, size_t size) noexcept
    {
        return Qt::StringLiterals::operator""_s(str, size);
    }

#endif // QT_DEPRECATED_SINCE(6, 8)
} // QtLiterals

QT_END_NAMESPACE

#include <QtCore/Stringbuilder.h>
#include <QtCore/Stringconverter.h>

#ifdef Q_L1S_VIEW_IS_PRIMARY
#    undef Q_L1S_VIEW_IS_PRIMARY
#endif

#endif // String_H

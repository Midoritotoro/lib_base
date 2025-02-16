#pragma once

#include <base/string/Char.h>
#include <base/io/ByteArray.h>

#include <string>
#include <string_view>

#include <base/utility/TypeTraits.h>



namespace base::string {
    enum SplitBehaviorFlags {
        KeepEmptyParts = 0,
        SkipEmptyParts = 0x1,
    };

    enum CaseSensitivity {
        CaseInsensitive,
        CaseSensitive
    };

    DECLARE_FLAGS(SplitBehavior, SplitBehaviorFlags)

    class String;
    class StringView;

    class RegularExpression;
    class RegularExpressionMatch;

    template <typename Char>
    struct IsCompatibleCharTypeHelper
        : std::integral_constant<bool,
        std::is_same<Char, Char>::value ||
        std::is_same<Char, ushort>::value ||
        std::is_same<Char, char16_t>::value ||
        (std::is_same<Char, wchar_t>::value && sizeof(wchar_t) == sizeof(Char))> {};

    template <typename Char>
    struct IsCompatibleCharType
        : IsCompatibleCharTypeHelper<std::remove_cvref_t<Char>> {};

    template <typename Pointer>
    struct IsCompatiblePointerHelper : std::false_type {};
    template <typename Char>

    struct IsCompatiblePointerHelper<Char*>
        : IsCompatibleCharType<Char> {};

    template <typename Pointer>
    struct IsCompatiblePointer
        : IsCompatiblePointerHelper<std::remove_cvref_t<Pointer>> {};

    template <typename T, typename Enable = void>
    struct IsContainerCompatibleWithStringView : std::false_type {};

    template <typename T>
    struct IsContainerCompatibleWithStringView<T, std::enable_if_t<std::conjunction_v<

        IsCompatiblePointer<decltype(std::data(std::declval<const T&>()))>,

        std::is_convertible<decltype(std::size(std::declval<const T&>())), sizetype>,
        IsCompatibleCharType<typename std::iterator_traits<decltype(std::begin(std::declval<const T&>()))>::value_type>,
        std::is_convertible<
        decltype(std::begin(std::declval<const T&>()) != std::end(std::declval<const T&>())),
        bool>,

        // These need to be treated specially due to the empty vs null distinction
        std::negation<std::is_same<std::decay_t<T>, String>>,

        // Don't make an accidental copy constructor
        std::negation<std::is_same<std::decay_t<T>, StringView>>
        >>> : std::true_type{};


    class StringView
    {
    public:
        typedef char16_t storage_type;
        typedef const Char value_type;

        typedef std::ptrdiff_t difference_type;
        typedef sizetype size_type;
        typedef value_type& reference;

        typedef value_type& const_reference;
        typedef value_type* pointer;
        typedef value_type* const_pointer;

        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        template <typename Char>
        using if_compatible_char = typename std::enable_if<IsCompatibleCharType<Char>::value, bool>::type;

        template <typename Pointer>
        using if_compatible_pointer = typename std::enable_if<IsCompatiblePointer<Pointer>::value, bool>::type;

        template <typename T>
        using if_compatible_qstring_like = typename std::enable_if<std::is_same<T, String>::value, bool>::type;

        template <typename T>
        using if_compatible_container = typename std::enable_if<IsContainerCompatibleWithStringView<T>::value, bool>::type;

        template <typename Char>
        static constexpr sizetype lengthHelperPointer(const Char* str) noexcept
        {
            if (is_constant_evaluated())
                return std::char_traits<Char>::length(str);

            return qustrlen(reinterpret_cast<const char16_t*>(str));
        }

        static sizetype lengthHelperPointer(const Char* str) noexcept
        {
            return qustrlen(reinterpret_cast<const char16_t*>(str));
        }

        template <typename Char>
        static const storage_type* castHelper(const Char* str) noexcept
        {
            return reinterpret_cast<const storage_type*>(str);
        }
        static constexpr const storage_type* castHelper(const storage_type* str) noexcept
        {
            return str;
        }

    public:
        constexpr StringView() noexcept {}
        constexpr StringView(std::nullptr_t) noexcept
            : StringView() {}

        template <typename Char, if_compatible_char<Char> = true>
        constexpr StringView(const Char* str, sizetype len)
            : m_size((Assert(len >= 0), Assert(str || !len), len)),
            m_data(castHelper(str))
        {}

        template <typename Char, if_compatible_char<Char> = true>
        constexpr StringView(const Char* f, const Char* l)
            : StringView(f, l - f) {}


        template <typename Pointer, if_compatible_pointer<Pointer> = true>
        constexpr StringView(const Pointer& str) noexcept
            : StringView(str, str ? lengthHelperPointer(str) : 0) {}


        template <typename String, if_compatible_qstring_like<String> = true>
        StringView(const String& str) noexcept
            : StringView(str.isNull() ? nullptr : str.data(), sizetype(str.size())) {}

        template <typename Container, if_compatible_container<Container> = true>
        constexpr always_inline StringView(const Container& c) noexcept
            : StringView(std::data(c), lengthHelperContainer(c)) {}

        template <typename Char, size_t Size, if_compatible_char<Char> = true>
        [[nodiscard]] constexpr static StringView fromArray(const Char(&string)[Size]) noexcept
        {
            return StringView(string, Size);
        }

        [[nodiscard]] inline String toString() const; // defined in qstring.h

        [[nodiscard]] constexpr sizetype size() const noexcept { return m_size; }
        [[nodiscard]] const_pointer data() const noexcept { return reinterpret_cast<const_pointer>(m_data); }

        [[nodiscard]] const_pointer constData() const noexcept { return data(); }
        [[nodiscard]] constexpr const storage_type* utf16() const noexcept { return m_data; }

        [[nodiscard]] constexpr Char operator[](sizetype n) const
        {
            verify(n, 1); return Char(m_data[n]);
        }

        template <typename...Args>
        [[nodiscard]] inline String arg(Args &&...args) const; // defined in qstring.h

        [[nodiscard]] io::ByteArray toLatin1() const { return convertToLatin1(*this); }
        [[nodiscard]] io::ByteArray toUtf8() const { return convertToUtf8(*this); }

        [[nodiscard]] io::ByteArray toLocal8Bit() const { return convertToLocal8Bit(*this); }
        [[nodiscard]] inline QList<uint> toUcs4() const; // defined in qlist.h ### Qt 7 char32_t

        [[nodiscard]] constexpr Char at(sizetype n) const noexcept { return (*this)[n]; }

        [[nodiscard]] constexpr StringView mid(sizetype pos, sizetype n = -1) const noexcept
        {
            auto result = ContainerImplHelper::mid(size(), &pos, &n);
            return result == ContainerImplHelper::Null ? StringView() : StringView(m_data + pos, n);
        }
        [[nodiscard]] constexpr StringView left(sizetype n) const noexcept
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return StringView(m_data, n);
        }
        [[nodiscard]] constexpr StringView right(sizetype n) const noexcept
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return StringView(m_data + m_size - n, n);
        }

        [[nodiscard]] constexpr StringView first(sizetype n) const noexcept
        {
            verify(0, n); return sliced(0, n);
        }
        [[nodiscard]] constexpr StringView last(sizetype n) const noexcept
        {
            verify(0, n); return sliced(size() - n, n);
        }
        [[nodiscard]] constexpr StringView sliced(sizetype pos) const noexcept
        {
            verify(pos, 0); return StringView(m_data + pos, size() - pos);
        }
        [[nodiscard]] constexpr StringView sliced(sizetype pos, sizetype n) const noexcept
        {
            verify(pos, n); return StringView(m_data + pos, n);
        }
        [[nodiscard]] constexpr StringView chopped(sizetype n) const noexcept
        {
            verify(0, n); return sliced(0, m_size - n);
        }

        constexpr void truncate(sizetype n) noexcept
        {
            verify(0, n); m_size = n;
        }
        constexpr void chop(sizetype n) noexcept
        {
            verify(0, n); m_size -= n;
        }

        [[nodiscard]] StringView trimmed() const noexcept { return trimmed(*this); }

        template <typename Needle, typename...Flags>
        [[nodiscard]] constexpr inline auto tokenize(Needle&& needle, Flags...flags) const
            noexcept(noexcept(qTokenize(std::declval<const StringView&>(), std::forward<Needle>(needle), flags...)))
            -> decltype(qTokenize(*this, std::forward<Needle>(needle), flags...))
        {
            return qTokenize(*this, std::forward<Needle>(needle), flags...);
        }

        [[nodiscard]] int compare(StringView other, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return compareStrings(*this, other, cs);
        }

        [[nodiscard]] inline int compare(Latin1StringView other, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;
        [[nodiscard]] inline int compare(QUtf8StringView other, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;

        [[nodiscard]] constexpr int compare(Char c) const noexcept
        {
            return size() >= 1 ? compare_single_char_helper(*utf16() - c.unicode()) : -1;
        }

        [[nodiscard]] int compare(Char c, Qt::CaseSensitivity cs) const noexcept
        {
            return compareStrings(*this, StringView(&c, 1), cs);
        }

        [[nodiscard]] inline int localeAwareCompare(StringView other) const;

        [[nodiscard]] bool startsWith(StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return startsWith(*this, s, cs);
        }
        [[nodiscard]] inline bool startsWith(Latin1StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;
        [[nodiscard]] bool startsWith(Char c) const noexcept
        {
            return !empty() && front() == c;
        }
        [[nodiscard]] bool startsWith(Char c, Qt::CaseSensitivity cs) const noexcept
        {
            return startsWith(*this, StringView(&c, 1), cs);
        }

        [[nodiscard]] bool endsWith(StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return endsWith(*this, s, cs);
        }
        [[nodiscard]] inline bool endsWith(Latin1StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;
        [[nodiscard]] bool endsWith(Char c) const noexcept
        {
            return !empty() && back() == c;
        }
        [[nodiscard]] bool endsWith(Char c, Qt::CaseSensitivity cs) const noexcept
        {
            return endsWith(*this, StringView(&c, 1), cs);
        }

        [[nodiscard]] sizetype indexOf(Char c, sizetype from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return findString(*this, from, StringView(&c, 1), cs);
        }
        [[nodiscard]] sizetype indexOf(StringView s, sizetype from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return findString(*this, from, s, cs);
        }
        [[nodiscard]] inline sizetype indexOf(Latin1StringView s, sizetype from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;

        [[nodiscard]] bool contains(Char c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return indexOf(StringView(&c, 1), 0, cs) != sizetype(-1);
        }
        [[nodiscard]] bool contains(StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return indexOf(s, 0, cs) != sizetype(-1);
        }
        [[nodiscard]] inline bool contains(Latin1StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;

        [[nodiscard]] sizetype count(Char c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return count(*this, c, cs);
        }
        [[nodiscard]] sizetype count(StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return count(*this, s, cs);
        }
        [[nodiscard]] inline sizetype count(Latin1StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

        [[nodiscard]] sizetype lastIndexOf(Char c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return lastIndexOf(c, -1, cs);
        }
        [[nodiscard]] sizetype lastIndexOf(Char c, sizetype from, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return lastIndexOf(*this, from, StringView(&c, 1), cs);
        }
        [[nodiscard]] sizetype lastIndexOf(StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return lastIndexOf(s, size(), cs);
        }
        [[nodiscard]] sizetype lastIndexOf(StringView s, sizetype from, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return lastIndexOf(*this, from, s, cs);
        }
        [[nodiscard]] inline sizetype lastIndexOf(Latin1StringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;
        [[nodiscard]] inline sizetype lastIndexOf(Latin1StringView s, sizetype from, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;

        [[nodiscard]] sizetype indexOf(const QRegularExpression& re, sizetype from = 0, QRegularExpressionMatch* rmatch = nullptr) const
        {
            return indexOf(*this, re, from, rmatch);
        }

        [[nodiscard]] sizetype lastIndexOf(const QRegularExpression& re, sizetype from, QRegularExpressionMatch* rmatch = nullptr) const
        {
            return lastIndexOf(*this, re, from, rmatch);
        }

        [[nodiscard]] bool contains(const QRegularExpression& re, QRegularExpressionMatch* rmatch = nullptr) const
        {
            return contains(*this, re, rmatch);
        }

        [[nodiscard]] sizetype count(const QRegularExpression& re) const
        {
            return count(*this, re);
        }

        [[nodiscard]] bool isRightToLeft() const noexcept
        {
            return isRightToLeft(*this);
        }

        [[nodiscard]] bool isValidUtf16() const noexcept
        {
            return isValidUtf16(*this);
        }

        [[nodiscard]] bool isUpper() const noexcept
        {
            return isUpper(*this);
        }

        [[nodiscard]] bool isLower() const noexcept
        {
            return isLower(*this);
        }

        [[nodiscard]] inline short toShort(bool* ok = nullptr, int base = 10) const;
        [[nodiscard]] inline ushort toUShort(bool* ok = nullptr, int base = 10) const;

        [[nodiscard]] inline int toInt(bool* ok = nullptr, int base = 10) const;
        [[nodiscard]] inline uint toUInt(bool* ok = nullptr, int base = 10) const;

        [[nodiscard]] inline long toLong(bool* ok = nullptr, int base = 10) const;
        [[nodiscard]] inline ulong toULong(bool* ok = nullptr, int base = 10) const;

        [[nodiscard]] inline longlong toLongLong(bool* ok = nullptr, int base = 10) const;
        [[nodiscard]] inline ulonglong toULongLong(bool* ok = nullptr, int base = 10) const;

        [[nodiscard]] float toFloat(bool* ok = nullptr) const;
        [[nodiscard]] double toDouble(bool* ok = nullptr) const;

        [[nodiscard]] inline sizetype toWCharArray(wchar_t* array) const; // defined in qstring.h


        [[nodiscard]]
            QList<StringView> split(StringView sep,
                Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
                Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
        [[nodiscard]]
            QList<StringView> split(Char sep, Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
                Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

        [[nodiscard]]
            QList<StringView> split(const RegularExpression& sep,
                Qt::SplitBehavior behavior = Qt::KeepEmptyParts) const;

        // StringView <> StringView
        friend bool operator==(StringView lhs, StringView rhs) noexcept { return lhs.size() == rhs.size() && equalStrings(lhs, rhs); }
        friend bool operator!=(StringView lhs, StringView rhs) noexcept { return !(lhs == rhs); }

        friend bool operator< (StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) < 0; }
        friend bool operator<=(StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) <= 0; }

        friend bool operator> (StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) > 0; }
        friend bool operator>=(StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) >= 0; }

        // StringView <> Char
        friend bool operator==(StringView lhs, Char rhs) noexcept { return lhs == StringView(&rhs, 1); }
        friend bool operator!=(StringView lhs, Char rhs) noexcept { return lhs != StringView(&rhs, 1); }

        friend bool operator< (StringView lhs, Char rhs) noexcept { return lhs < StringView(&rhs, 1); }
        friend bool operator<=(StringView lhs, Char rhs) noexcept { return lhs <= StringView(&rhs, 1); }

        friend bool operator> (StringView lhs, Char rhs) noexcept { return lhs > StringView(&rhs, 1); }
        friend bool operator>=(StringView lhs, Char rhs) noexcept { return lhs >= StringView(&rhs, 1); }

        friend bool operator==(Char lhs, StringView rhs) noexcept { return StringView(&lhs, 1) == rhs; }
        friend bool operator!=(Char lhs, StringView rhs) noexcept { return StringView(&lhs, 1) != rhs; }

        friend bool operator< (Char lhs, StringView rhs) noexcept { return StringView(&lhs, 1) < rhs; }
        friend bool operator<=(Char lhs, StringView rhs) noexcept { return StringView(&lhs, 1) <= rhs; }

        friend bool operator> (Char lhs, StringView rhs) noexcept { return StringView(&lhs, 1) > rhs; }
        friend bool operator>=(Char lhs, StringView rhs) noexcept { return StringView(&lhs, 1) >= rhs; }

        //
        // STL compatibility API:
        //
        [[nodiscard]] const_iterator begin()   const noexcept { return data(); }
        [[nodiscard]] const_iterator end()     const noexcept { return data() + size(); }

        [[nodiscard]] const_iterator cbegin()  const noexcept { return begin(); }
        [[nodiscard]] const_iterator cend()    const noexcept { return end(); }

        [[nodiscard]] const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(end()); }
        [[nodiscard]] const_reverse_iterator rend()    const noexcept { return const_reverse_iterator(begin()); }

        [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        [[nodiscard]] const_reverse_iterator crend()   const noexcept { return rend(); }

        [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }

        [[nodiscard]] constexpr Char front() const { return Q_ASSERT(!empty()), Char(m_data[0]); }
        [[nodiscard]] constexpr Char back()  const { return Q_ASSERT(!empty()), Char(m_data[m_size - 1]); }

        [[nodiscard]] IMPLICIT operator std::u16string_view() const noexcept
        {
            return std::u16string_view(m_data, size_t(m_size));
        }

        //
        // Qt compatibility API:
        //
        [[nodiscard]] const_iterator constBegin() const noexcept { return begin(); }

        [[nodiscard]] const_iterator constEnd() const noexcept { return end(); }
        [[nodiscard]] constexpr bool isNull() const noexcept { return !m_data; }

        [[nodiscard]] constexpr bool isEmpty() const noexcept { return empty(); }
        [[nodiscard]] constexpr sizetype length() const noexcept
        {
            return size();
        }
        [[nodiscard]] constexpr Char first() const { return front(); }
        [[nodiscard]] constexpr Char last()  const { return back(); }
    private:
        sizetype m_size = 0;
        const storage_type* m_data = nullptr;

        always_inline constexpr void verify([[maybe_unused]] sizetype pos = 0,
            [[maybe_unused]] sizetype n = 1) const
        {
            Assert(pos >= 0);
            Assert(pos <= size());
            Assert(n >= 0);
            Assert(n <= size() - pos);
        }

        constexpr int compare_single_char_helper(int diff) const noexcept
        {
            return diff ? diff : size() > 1 ? 1 : 0;
        }
    };

    DECLARE_TYPEINFO(StringView, PRIMITIVE_TYPE);

    template <typename StringLike, typename std::enable_if<
        std::is_same<StringLike, String>::value,
        bool>::type = true>
    inline StringView qToStringViewIgnoringNull(const StringLike& s) noexcept
    {
        return StringView(s.data(), s.size());
    }

    [[nodiscard]] constexpr auto Char::fromUcs4(char32_t c) noexcept
    {
        struct R {
            char16_t chars[2];
            [[nodiscard]] constexpr operator StringView() const noexcept { return { begin(), end() }; }
            [[nodiscard]] constexpr sizetype size() const noexcept { return chars[1] ? 2 : 1; }
            [[nodiscard]] constexpr const char16_t* begin() const noexcept { return chars; }
            [[nodiscard]] constexpr const char16_t* end() const noexcept { return begin() + size(); }
        };
        return requiresSurrogates(c) ? R{ {Char::highSurrogate(c),
                                          Char::lowSurrogate(c)} } :
            R{ {char16_t(c), u'\0'} };
    }
} // namespace base::strings
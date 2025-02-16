#pragma once 

#include <base/string/StringAlgorithms.h>
#include <base/string/StringFwd.h>

#include <base/io/ArrayData.h>
#include <base/io/ByteArrayView.h>

#include <base/io/ByteArray.h>

#include <string>
#include <string_view>

#include <base/utility/TypeTraits.h>
#include <base/string/Utf8StringView.h>

#include <base/string/Latin1StringView.h>


namespace base::string {
    class BasicUtf8StringView;
        template <typename _Char>
        using IsCompatibleChar8TypeHelper = std::disjunction<
#ifdef __cpp_char8_t
            std::is_same<_Char, char8_t>,
#endif
            std::is_same<_Char, char>,
            std::is_same<_Char, uchar>,
            std::is_same<_Char, signed char>
        >;
        template <typename _Char>
        using IsCompatibleChar8Type
            = IsCompatibleChar8TypeHelper<remove_cvref_t<_Char>>;

        template <typename Pointer>

        struct IsCompatiblePointer8Helper : std::false_type {};
        template <typename _Char>

        struct IsCompatiblePointer8Helper<_Char*>
            : IsCompatibleChar8Type<_Char> {};

        template <typename Pointer>
        using IsCompatiblePointer8
            = IsCompatiblePointer8Helper<remove_cvref_t<Pointer>>;

        template <typename T, typename Enable = void>
        struct IsContainerCompatibleWithUtf8StringView : std::false_type {};

        template <typename T>
        struct IsContainerCompatibleWithUtf8StringView < T, std::enable_if_t < std::conjunction_v <
            // lacking concepts and ranges, we accept any T whose std::data yields a suitable pointer ...
            IsCompatiblePointer8<decltype(std::data(std::declval<const T&>()))>,
            // ... and that has a suitable size ...
            std::is_convertible<
            decltype(std::size(std::declval<const T&>())),
            sizetype
            >,
            // ... and it's a range as it defines an iterator-like API
            IsCompatibleChar8Type<typename std::iterator_traits<
            decltype(std::begin(std::declval<const T&>()))>::value_type
            >,
            std::is_convertible<
            decltype(std::begin(std::declval<const T&>()) != std::end(std::declval<const T&>())),
            bool
            >,

            // This needs to be treated specially due to the empty vs null distinction
            std::negation<std::is_same<std::decay_t<T>, io::ByteArray>>,

            // This has a compatible value_type, but explicitly a different encoding
            std::negation<std::is_same<std::decay_t<T>, Latin1StringView>>,

            // Don't make an accidental copy constructor
            std::negation<std::disjunction<
            std::is_same<std::decay_t<T>, BasicUtf8StringView<true>>,
            std::is_same<std::decay_t<T>, BasicUtf8StringView<false>>
            >>
            >> > : std::true_type{};

        struct hide_char8_t {
#ifdef __cpp_char8_t
            using type = char8_t;
#endif
        };

        struct wrap_char { using type = char; };


    class BasicUtf8StringView
    {
    public:

        using storage_type = typename std::conditional<true,
            hide_char8_t,
            wrap_char
        >::type::type;

        typedef const storage_type value_type;
        typedef ptrdiff difference_type;

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
        template <typename _Char>
        using if_compatible_char = std::enable_if_t<IsCompatibleChar8Type<_Char>::value, bool>;

        template <typename Pointer>
        using if_compatible_pointer = std::enable_if_t<IsCompatiblePointer8<Pointer>::value, bool>;

        template <typename T>
        using if_compatible_qstring_like = std::enable_if_t<std::is_same_v<T, io::ByteArray>, bool>;

        template <typename T>
        using if_compatible_container = std::enable_if_t<IsContainerCompatibleWithUtf8StringView<T>::value, bool>;

        template <typename Container>
        static constexpr sizetype lengthHelperContainer(const Container& c) noexcept
        {
            return sizetype(std::size(c));
        }

        // Note: Do not replace with std::size(const Char (&)[N]), cause the result
        // will be of by one.
        template <typename _Char, size_t N>
        static constexpr sizetype lengthHelperContainer(const _Char(&str)[N]) noexcept
        {
            const auto it = std::char_traits<Char>::find(str, N, Char(0));
            const auto end = it ? it : std::next(str, N);
            return sizetype(std::distance(str, end));
        }

        template <typename _Char>
        static const storage_type* castHelper(const _Char* str) noexcept
        {
            return reinterpret_cast<const storage_type*>(str);
        }
        static constexpr const storage_type* castHelper(const storage_type* str) noexcept
        {
            return str;
        }

    public:
        constexpr BasicUtf8StringView() noexcept
            : m_data(nullptr), m_size(0) {}
        constexpr BasicUtf8StringView(std::nullptr_t) noexcept
            : BasicUtf8StringView() {}

        template <typename _Char, if_compatible_char<_Char> = true>
        constexpr BasicUtf8StringView(const _Char* str, qsizetype len)
            : m_data(castHelper(str)),
            m_size((Assert(len >= 0), Assert(str || !len), len)) {}

        template <typename _Char, if_compatible_char<_Char> = true>
        constexpr BasicUtf8StringView(const _Char* f, const _Char* l)
            : BasicUtf8StringView(f, l - f) {}

        template <typename Pointer, if_compatible_pointer<Pointer> = true>
        constexpr BasicUtf8StringView(const Pointer& str) noexcept
            : BasicUtf8StringView(str,
                str ? std::char_traits<std::remove_cv_t<std::remove_pointer_t<Pointer>>>::length(str) : 0) {}

        template <typename _String, if_compatible_qstring_like<_String> = true>
        BasicUtf8StringView(const _String& str) noexcept
            : BasicUtf8StringView(str.isNull() ? nullptr : str.data(), sizetype(str.size())) {}

        template <typename Container, if_compatible_container<Container> = true>
        constexpr BasicUtf8StringView(const Container& c) noexcept
            : BasicUtf8StringView(std::data(c), lengthHelperContainer(c)) {}

#if defined(__cpp_char8_t)
        constexpr BasicUtf8StringView(BasicUtf8StringView<!UseChar8T> other)
            : BasicUtf8StringView(other.data(), other.size()) {}
#endif

        template <typename _Char, size_t Size, if_compatible_char<_Char> = true>
        [[nodiscard]] constexpr static BasicUtf8StringView fromArray(const _Char(&string)[Size]) noexcept
        {
            return BasicUtf8StringView(string, Size);
        }

        [[nodiscard]] inline String toString() const; // defined in String.h

        [[nodiscard]] constexpr sizetype size() const noexcept { return m_size; }
        [[nodiscard]] constexpr const_pointer data() const noexcept { return m_data; }
#ifdef __cpp_char8_t
        [[nodiscard]] const char8_t* utf8() const noexcept { return reinterpret_cast<const char8_t*>(m_data); }
#endif

        [[nodiscard]] constexpr storage_type operator[](qsizetype n) const
        {
            verify(n, 1); return m_data[n];
        }

        //
        // String API
        //

        [[nodiscard]] constexpr storage_type at(qsizetype n) const { return (*this)[n]; }

        [[nodiscard]]
        constexpr BasicUtf8StringView mid(qsizetype pos, qsizetype n = -1) const
        {
            using namespace QtPrivate;
            auto result = QContainerImplHelper::mid(size(), &pos, &n);
            return result == QContainerImplHelper::Null ? BasicUtf8StringView() : BasicUtf8StringView(m_data + pos, n);
        }
        [[nodiscard]]
        constexpr BasicUtf8StringView left(qsizetype n) const
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return BasicUtf8StringView(m_data, n);
        }
        [[nodiscard]]
        constexpr BasicUtf8StringView right(qsizetype n) const
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return BasicUtf8StringView(m_data + m_size - n, n);
        }

        [[nodiscard]] constexpr BasicUtf8StringView sliced(qsizetype pos) const
        {
            verify(pos, 0); return BasicUtf8StringView{ m_data + pos, m_size - pos };
        }
        [[nodiscard]] constexpr BasicUtf8StringView sliced(qsizetype pos, qsizetype n) const
        {
            verify(pos, n); return BasicUtf8StringView(m_data + pos, n);
        }
        [[nodiscard]] constexpr BasicUtf8StringView first(qsizetype n) const
        {
            verify(0, n); return sliced(0, n);
        }
        [[nodiscard]] constexpr BasicUtf8StringView last(qsizetype n) const
        {
            verify(0, n); return sliced(m_size - n, n);
        }
        [[nodiscard]] constexpr BasicUtf8StringView chopped(qsizetype n) const
        {
            verify(0, n); return sliced(0, m_size - n);
        }

        constexpr void truncate(qsizetype n)
        {
            verify(0, n); m_size = n;
        }
        constexpr void chop(qsizetype n)
        {
            verify(0, n); m_size -= n;
        }

        [[nodiscard]] inline bool isValidUtf8() const noexcept
        {
            return QByteArrayView(reinterpret_cast<const char*>(data()), size()).isValidUtf8();
        }

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
        [[nodiscard]] constexpr storage_type front() const { return Q_ASSERT(!empty()), m_data[0]; }
        [[nodiscard]] constexpr storage_type back()  const { return Q_ASSERT(!empty()), m_data[m_size - 1]; }

        [[nodiscard]] Q_IMPLICIT operator std::basic_string_view<storage_type>() const noexcept
        {
            return std::basic_string_view<storage_type>(data(), size_t(size()));
        }

        //
        // Qt compatibility API:
        //
        [[nodiscard]] constexpr bool isNull() const noexcept { return !m_data; }
        [[nodiscard]] constexpr bool isEmpty() const noexcept { return empty(); }
        [[nodiscard]] constexpr qsizetype length() const noexcept
        {
            return size();
        }

        [[nodiscard]] int compare(BasicUtf8StringView other,
            Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept
        {
            return QtPrivate::compareStrings(*this, other, cs);
        }

        [[nodiscard]] int compare(QStringView other,
            Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;
        [[nodiscard]] int compare(QLatin1StringView other,
            Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;

    private:
        [[nodiscard]] static inline int compare(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return QtPrivate::compareStrings(BasicUtf8StringView<false>(lhs.data(), lhs.size()),
                BasicUtf8StringView<false>(rhs.data(), rhs.size()));
        }

        [[nodiscard]] friend inline bool operator==(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return lhs.size() == rhs.size()
                && QtPrivate::equalStrings(BasicUtf8StringView<false>(lhs.data(), lhs.size()),
                    BasicUtf8StringView<false>(rhs.data(), rhs.size()));
        }
        [[nodiscard]] friend inline bool operator!=(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return !operator==(lhs, rhs);
        }

#ifdef __cpp_impl_three_way_comparison
        [[nodiscard]] friend inline auto operator<=>(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return BasicUtf8StringView::compare(lhs, rhs) <=> 0;
        }
#else
        [[nodiscard]] friend inline bool operator<=(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return BasicUtf8StringView::compare(lhs, rhs) <= 0;
        }
        [[nodiscard]] friend inline bool operator>=(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return BasicUtf8StringView::compare(lhs, rhs) >= 0;
        }
        [[nodiscard]] friend inline bool operator<(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return BasicUtf8StringView::compare(lhs, rhs) < 0;
        }
        [[nodiscard]] friend inline bool operator>(BasicUtf8StringView lhs, BasicUtf8StringView rhs) noexcept
        {
            return BasicUtf8StringView::compare(lhs, rhs) > 0;
        }
#endif

        Q_ALWAYS_INLINE constexpr void verify([[maybe_unused]] qsizetype pos = 0,
            [[maybe_unused]] qsizetype n = 1) const
        {
            Q_ASSERT(pos >= 0);
            Q_ASSERT(pos <= size());
            Q_ASSERT(n >= 0);
            Q_ASSERT(n <= size() - pos);
        }
        const storage_type* m_data;
        qsizetype m_size;
    };

#ifdef Q_QDOC
#undef BasicUtf8StringView
#else
    template <bool UseChar8T>
    DECLARE_TYPEINFO_BODY(BasicUtf8StringView<UseChar8T>, Q_PRIMITIVE_TYPE);

    template <typename QStringLike, std::enable_if_t<std::is_same_v<QStringLike, QByteArray>, bool> = true>
    [[nodiscard]] inline q_no_char8_t::QUtf8StringView qToUtf8StringViewIgnoringNull(const QStringLike& s) noexcept
    {
        return q_no_char8_t::QUtf8StringView(s.data(), s.size());
    }
#endif // Q_QDOC
}
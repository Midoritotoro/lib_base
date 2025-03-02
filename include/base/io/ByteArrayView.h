#pragma once

#include <base/io/ArrayData.h>
#include <base/io/ByteArrayAlgorithms.h>

#include <base/utility/TypeInfo.h>

#include <string>
#include <string_view>


namespace base::io {
    class ByteArray;

    template <typename Byte>
    struct IsCompatibleByteTypeHelper
        : std::integral_constant<bool,
        std::is_same_v<Byte, char> ||
        std::is_same_v<Byte, uchar> ||
        std::is_same_v<Byte, signed char> ||
        std::is_same_v<Byte, std::byte>> {};

    template <typename Byte>
    struct IsCompatibleByteType
        : IsCompatibleByteTypeHelper<std::remove_cvref_t<Byte>> {};

    template <typename Pointer>
    struct IsCompatibleByteArrayPointerHelper : std::false_type {};

    template <typename Byte>
    struct IsCompatibleByteArrayPointerHelper<Byte*>
        : IsCompatibleByteType<Byte> {};

    template<typename Pointer>
    struct IsCompatibleByteArrayPointer
        : IsCompatibleByteArrayPointerHelper<q20::remove_cvref_t<Pointer>> {};

    template <typename T, typename Enable = void>
    struct IsContainerCompatibleWithByteArrayView : std::false_type {};

    template <typename T>
    struct IsContainerCompatibleWithByteArrayView < T, std::enable_if_t <
        std::conjunction_v<
        // lacking concepts and ranges, we accept any T whose std::data yields a suitable
        // pointer ...
        IsCompatibleByteArrayPointer<decltype(std::data(std::declval<const T&>()))>,
        // ... and that has a suitable size ...
        std::is_convertible<decltype(std::size(std::declval<const T&>())), sizetype>,
        // ... and it's a range as it defines an iterator-like API
        IsCompatibleByteType<typename std::iterator_traits<decltype(
            std::begin(std::declval<const T&>()))>::value_type>,
        std::is_convertible<decltype(std::begin(std::declval<const T&>())
            != std::end(std::declval<const T&>())),
        bool>,

        // This needs to be treated specially due to the empty vs null distinction
        std::negation<std::is_same<std::decay_t<T>, ByteArray>>,

        // We handle array literals specially for source compat reasons
        std::negation<std::is_array<T>>,

        // Don't make an accidental copy constructor
        std::negation<std::is_same<std::decay_t<T>, ByteArrayView>> >> > : std::true_type{};

    // Used by QLatin1StringView too

    struct ContainerImplHelper
    {
        enum CutResult { Null, Empty, Full, Subset };
        static constexpr CutResult mid(sizetype originalLength, sizetype* _position, sizetype* _length)
        {
            sizetype& position = *_position;
            sizetype& length = *_length;
            if (position > originalLength) {
                position = 0;
                length = 0;
                return Null;
            }

            if (position < 0) {
                if (length < 0 || length + position >= originalLength) {
                    position = 0;
                    length = originalLength;
                    return Full;
                }
                if (length + position <= 0) {
                    position = length = 0;
                    return Null;
                }
                length += position;
                position = 0;
            }
            else if (size_t(length) > size_t(originalLength - position)) {
                length = originalLength - position;
            }

            if (position == 0 && length == originalLength)
                return Full;

            return length > 0 ? Subset : Empty;
        }
    };

    template <typename Char>
    static constexpr sizetype lengthHelperPointer(const Char* data) noexcept
    {
        return sizetype(std::char_traits<Char>::length(data));
    }

    class ByteArrayView
    {
    public:
        typedef char storage_type;
        typedef const char value_type;

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
        template <typename Byte>
        using if_compatible_byte =
            typename std::enable_if_t<IsCompatibleByteType<Byte>::value, bool>;

        template <typename Pointer>
        using if_compatible_pointer =
            typename std::enable_if_t<IsCompatibleByteArrayPointer<Pointer>::value,
            bool>;

        template <typename T>
        using if_compatible_bytearray_like =
            typename std::enable_if_t<std::is_same_v<T, ByteArray>, bool>;

        template <typename T>
        using if_compatible_container =
            typename std::enable_if_t<IsContainerCompatibleWithByteArrayView<T>::value,
            bool>;

        template <typename Container>
        static constexpr sizetype lengthHelperContainer(const Container& c) noexcept
        {
            return sizetype(std::size(c));
        }

        static constexpr sizetype lengthHelperCharArray(const char* data, size_t size) noexcept
        {
            const auto it = std::char_traits<char>::find(data, size, '\0');
            const auto end = it ? it : std::next(data, size);
            return sizetype(std::distance(data, end));
        }

        template <typename Byte>
        static const storage_type* castHelper(const Byte* data) noexcept
        {
            return reinterpret_cast<const storage_type*>(data);
        }
        static constexpr const storage_type* castHelper(const storage_type* data) noexcept
        {
            return data;
        }

    public:
        constexpr ByteArrayView() noexcept
            : m_size(0), m_data(nullptr) {}
        constexpr ByteArrayView(std::nullptr_t) noexcept
            : ByteArrayView() {}

        template <typename Byte, if_compatible_byte<Byte> = true>
        constexpr ByteArrayView(const Byte* data, sizetype len)
            : m_size((Assert(len >= 0), Assert(data || !len), len)),
            m_data(castHelper(data)) {}

        template <typename Byte, if_compatible_byte<Byte> = true>
        constexpr ByteArrayView(const Byte* first, const Byte* last)
            : ByteArrayView(first, last - first) {}

        template <typename Pointer, if_compatible_pointer<Pointer> = true>
        constexpr ByteArrayView(const Pointer& data) noexcept
            : ByteArrayView(
                data, data ? lengthHelperPointer(data) : 0) {}

        template <typename ByteArray, if_compatible_bytearray_like<ByteArray> = true>
        ByteArrayView(const ByteArray& ba) noexcept
            : ByteArrayView(ba.isNull() ? nullptr : ba.data(), sizetype(ba.size())) {}

        template <typename Container, if_compatible_container<Container> = true>
        constexpr ByteArrayView(const Container& c) noexcept
            : ByteArrayView(std::data(c), lengthHelperContainer(c)) {}
        template <size_t Size>
        constexpr ByteArrayView(const char(&data)[Size]) noexcept
            : ByteArrayView(data, lengthHelperCharArray(data, Size)) {}

        template <typename Byte, size_t Size, if_compatible_byte<Byte> = true>

        [[nodiscard]] constexpr static ByteArrayView fromArray(const Byte(&data)[Size]) noexcept
        {
            return ByteArrayView(data, Size);
        }
        [[nodiscard]] inline ByteArray toByteArray() const; // defined in bytearray.h

        [[nodiscard]] constexpr sizetype size() const noexcept { return m_size; }
        [[nodiscard]] constexpr const_pointer data() const noexcept { return m_data; }
        [[nodiscard]] constexpr const_pointer constData() const noexcept { return data(); }

        [[nodiscard]] constexpr char operator[](sizetype n) const
        {
            verify(n, 1); return m_data[n];
        }

        [[nodiscard]] constexpr char at(sizetype n) const { return (*this)[n]; }

        [[nodiscard]] constexpr ByteArrayView first(sizetype n) const
        {
            verify(0, n); return sliced(0, n);
        }

        [[nodiscard]] constexpr ByteArrayView last(sizetype n) const
        {
            verify(0, n); return sliced(size() - n, n);
        }

        [[nodiscard]] constexpr ByteArrayView sliced(sizetype pos) const
        {
            verify(pos, 0); return ByteArrayView(data() + pos, size() - pos);
        }

        [[nodiscard]] constexpr ByteArrayView sliced(sizetype pos, sizetype n) const
        {
            verify(pos, n); return ByteArrayView(data() + pos, n);
        }

        [[nodiscard]] constexpr ByteArrayView chopped(sizetype len) const
        {
            verify(0, len); return sliced(0, size() - len);
        }

        [[nodiscard]] constexpr ByteArrayView left(sizetype n) const
        {
            if (n < 0 || n > size()) n = size(); return ByteArrayView(data(), n);
        }
        [[nodiscard]] constexpr ByteArrayView right(sizetype n) const
        {
            if (n < 0 || n > size()) n = size(); if (n < 0) n = 0; return ByteArrayView(data() + size() - n, n);
        }
        [[nodiscard]] constexpr ByteArrayView mid(sizetype pos, sizetype n = -1) const
        {
            auto result = ContainerImplHelper::mid(size(), &pos, &n);
            return result == ContainerImplHelper::Null ? ByteArrayView()
                : ByteArrayView(m_data + pos, n);
        }

        constexpr void truncate(sizetype n)
        {
            verify(0, n); m_size = n;
        }
        constexpr void chop(sizetype n)
        {
            verify(0, n); m_size -= n;
        }


        [[nodiscard]] ByteArrayView trimmed() const noexcept
        {
            return algorithms::trimmed(*this);
        }
        [[nodiscard]] short toShort(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<short>(*this, ok, base);
        }
        [[nodiscard]] ushort toUShort(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<ushort>(*this, ok, base);
        }
        [[nodiscard]] int toInt(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<int>(*this, ok, base);
        }
        [[nodiscard]] uint toUInt(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<uint>(*this, ok, base);
        }
        [[nodiscard]] long toLong(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<long>(*this, ok, base);
        }
        [[nodiscard]] ulong toULong(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<ulong>(*this, ok, base);
        }
        [[nodiscard]] longlong toLongLong(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<longlong>(*this, ok, base);
        }
        [[nodiscard]] ulonglong toULongLong(bool* ok = nullptr, int base = 10) const
        {
            return algorithms::toIntegral<ulonglong>(*this, ok, base);
        }
        [[nodiscard]] float toFloat(bool* ok = nullptr) const
        {
            const auto r = algorithms::toFloat(*this);
            if (ok)
                *ok = bool(r);
            return r.value_or(0.0f);
        }

        [[nodiscard]] double toDouble(bool* ok = nullptr) const
        {
            const auto r = algorithms::toDouble(*this);
            if (ok)
                *ok = bool(r);
            return r.value_or(0.0);
        }

        [[nodiscard]] bool startsWith(ByteArrayView other) const noexcept
        {
            return algorithms::startsWith(*this, other);
        }
        [[nodiscard]] bool startsWith(char c) const noexcept
        {
            return !empty() && front() == c;
        }

        [[nodiscard]] bool endsWith(ByteArrayView other) const noexcept
        {
            return algorithms::endsWith(*this, other);
        }
        [[nodiscard]] bool endsWith(char c) const noexcept
        {
            return !empty() && back() == c;
        }

        [[nodiscard]] sizetype indexOf(ByteArrayView a, sizetype from = 0) const noexcept
        {
            return algorithms::findByteArray(*this, from, a);
        }
        [[nodiscard]] sizetype indexOf(char ch, sizetype from = 0) const noexcept
        {
            return algorithms::findByteArray(*this, from, ByteArrayView(&ch, 1));
        }

        [[nodiscard]] bool contains(ByteArrayView a) const noexcept
        {
            return indexOf(a) != sizetype(-1);
        }
        [[nodiscard]] bool contains(char c) const noexcept
        {
            return indexOf(c) != sizetype(-1);
        }

        [[nodiscard]] sizetype lastIndexOf(ByteArrayView a) const noexcept
        {
            return lastIndexOf(a, size());
        }
        [[nodiscard]] sizetype lastIndexOf(ByteArrayView a, sizetype from) const noexcept
        {
            return algorithms::lastIndexOf(*this, from, a);
        }
        [[nodiscard]] sizetype lastIndexOf(char ch, sizetype from = -1) const noexcept
        {
            return algorithms::lastIndexOf(*this, from, ByteArrayView(&ch, 1));
        }

        [[nodiscard]] sizetype count(ByteArrayView a) const noexcept
        {
            return algorithms::count(*this, a);
        }
        [[nodiscard]] sizetype count(char ch) const noexcept
        {
            return algorithms::count(*this, ByteArrayView(&ch, 1));
        }


        inline int compare(ByteArrayView a, CaseSensitivity cs = CaseSensitive) const noexcept;

        [[nodiscard]] inline bool isValidUtf8() const noexcept { return algorithms::isValidUtf8(*this); }


        [[nodiscard]] constexpr const_iterator begin()   const noexcept { return data(); }
        [[nodiscard]] constexpr const_iterator end()     const noexcept { return data() + size(); }
        [[nodiscard]] constexpr const_iterator cbegin()  const noexcept { return begin(); }
        [[nodiscard]] constexpr const_iterator cend()    const noexcept { return end(); }
        [[nodiscard]] constexpr const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(end()); }
        [[nodiscard]] constexpr const_reverse_iterator rend()    const noexcept { return const_reverse_iterator(begin()); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        [[nodiscard]] constexpr const_reverse_iterator crend()   const noexcept { return rend(); }

        [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
        [[nodiscard]] constexpr char front() const { Assert(!empty()); return m_data[0]; }
        [[nodiscard]] constexpr char back()  const { Assert(!empty()); return m_data[m_size - 1]; }

        [[nodiscard]] constexpr explicit(false) operator std::string_view() const noexcept
        {
            return std::string_view(m_data, size_t(m_size));
        }

        [[nodiscard]] constexpr bool isNull() const noexcept { return !m_data; }
        [[nodiscard]] constexpr bool isEmpty() const noexcept { return empty(); }
        [[nodiscard]] constexpr sizetype length() const noexcept
        {
            return size();
        }
        [[nodiscard]] constexpr char first() const { return front(); }
        [[nodiscard]] constexpr char last()  const { return back(); }

        friend inline bool operator==(ByteArrayView lhs, ByteArrayView rhs) noexcept
        {
            return lhs.size() == rhs.size() && (!lhs.size() || memcmp(lhs.data(), rhs.data(), lhs.size()) == 0);
        }
        friend inline bool operator!=(ByteArrayView lhs, ByteArrayView rhs) noexcept
        {
            return !(lhs == rhs);
        }
        friend inline bool operator< (ByteArrayView lhs, ByteArrayView rhs) noexcept
        {
            return algorithms::compareMemory(lhs, rhs) < 0;
        }
        friend inline bool operator<=(ByteArrayView lhs, ByteArrayView rhs) noexcept
        {
            return algorithms::compareMemory(lhs, rhs) <= 0;
        }
        friend inline bool operator> (ByteArrayView lhs, ByteArrayView rhs) noexcept
        {
            return !(lhs <= rhs);
        }
        friend inline bool operator>=(ByteArrayView lhs, ByteArrayView rhs) noexcept
        {
            return !(lhs < rhs);
        }

        private:
            always_inline constexpr void verify([[maybe_unused]] sizetype pos = 0,
                [[maybe_unused]] sizetype n = 1) const
            {
                Assert(pos >= 0);
                Assert(pos <= size());

                Assert(n >= 0);
                Assert(n <= size() - pos);
            }

            sizetype m_size;
            const storage_type* m_data;
        };

    DECLARE_TYPEINFO(ByteArrayView, PRIMITIVE_TYPE);

    template<typename ByteArrayLike,
        std::enable_if_t<std::is_same_v<ByteArrayLike, ByteArray>, bool> = true>
    [[nodiscard]] inline ByteArrayView ToByteArrayViewIgnoringNull(const ByteArrayLike& b) noexcept
    {
        return ByteArrayView(b.data(), b.size());
    }

    inline int ByteArrayView::compare(ByteArrayView a, CaseSensitivity cs) const noexcept
    {
        return cs == CaseSensitive ? QtPrivate::compareMemory(*this, a) :
            qstrnicmp(data(), size(), a.data(), a.size());
    }

} // namespace base::io
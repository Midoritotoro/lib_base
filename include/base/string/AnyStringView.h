#pragma once 

#include <base/string/Latin1StringView.h>
#include <base/string/StringView.h>

#include <base/utility/TypeInfo.h>
#include <base/string/Utf8StringView.h>

#include <base/string/StringAlgorithms.h>
#include <limits>


namespace base::string {

    template <typename Tag, typename Result>
    struct wrapped { using type = Result; };

    template <typename Tag, typename Result>
    using wrapped_t = typename wrapped<Tag, Result>::type;

    class AnyStringView
    {
    public:
        typedef ptrdiff difference_type;
        typedef sizetype size_type;
    private:
        static constexpr size_t SizeMask = (std::numeric_limits<size_t>::max)() / 4;
        static constexpr int SizeShift = 0;

        static constexpr size_t Latin1Flag = SizeMask + 1;

        static constexpr size_t TwoByteCodePointFlag = Latin1Flag << 1;
        static constexpr size_t TypeMask = ~(SizeMask << SizeShift);

        static_assert(TypeMask == (Latin1Flag | TwoByteCodePointFlag));

        // Tag bits
        //  0  0   Utf8
        //  0  1   Latin1
        //  1  0   Utf16
        //  1  1   Unused
        //  ^  ^ latin1
        //  | sizeof code-point == 2
        enum Tag : size_t {
            Utf8 = 0,
            Latin1 = Latin1Flag,
            Utf16 = TwoByteCodePointFlag,
            Unused = TypeMask,
        };

        template <typename Char>
        using if_compatible_char = std::enable_if_t<std::disjunction_v<
            IsCompatibleCharType<Char>,
            IsCompatibleChar8Type<Char>
        >, bool>;

        template <typename Pointer>
        using if_compatible_pointer = std::enable_if_t<std::disjunction_v<
            IsCompatiblePointer<Pointer>,
            IsCompatiblePointer8<Pointer>
        >, bool>;


        template <typename T>
        using if_compatible_container = std::enable_if_t<std::disjunction_v<
            IsContainerCompatibleWithStringView<T>,
            IsContainerCompatibleWithUtf8StringView<T>
        >, bool>;

        template <typename StringOrByteArray, typename T>
        using if_convertible_to = std::enable_if_t<std::conjunction_v<
            // need to exclude a bunch of stuff, because we take by universal reference:
            std::negation<std::disjunction<
            std::is_same<remove_cvref_t<T>, AnyStringView::Tag>,
            std::is_same<remove_cvref_t<T>, AnyStringView>, // don't make a copy/move ctor
            std::is_pointer<std::decay_t<T>>, // const char*, etc
            std::is_same<remove_cvref_t<T>, ByteArray>,
            std::is_same<remove_cvref_t<T>, String>
            >>,
            // this is what we're really after:
            std::is_convertible<T, StringOrByteArray>
            >, bool>;

        // confirm we don't make an accidental copy constructor:
        static_assert(IsContainerCompatibleWithStringView<AnyStringView>::value == false);
        static_assert(IsContainerCompatibleWithUtf8StringView<AnyStringView>::value == false);

        template<typename _Char>
        static constexpr bool isAsciiOnlyCharsAtCompileTime(_Char* str, sizetype sz) noexcept
        {
            // do not perform check if not at compile time
            if (!is_constant_evaluated())
                return false;
            if constexpr (sizeof(_Char) != sizeof(char)) {
                unused(str);
                unused(sz);
                return false;
            }
            else {
                for (sizetype i = 0; i < sz; ++i) {
                    if (uchar(str[i]) > 0x7f)
                        return false;
                }
                return true;
            }
        }

        template<typename _Char>
        static constexpr std::size_t encodeType(const _Char* str, sizetype sz) noexcept
        {
            // Utf16 if 16 bit, Latin1 if ASCII, else Utf8
            Assert(sz >= 0);
            Assert(sz <= sizetype(SizeMask));
            Assert(str || !sz);
            return (std::size_t(sz) << SizeShift)
                | uint(sizeof(_Char) == sizeof(char16_t)) * Tag::Utf16
                | uint(isAsciiOnlyCharsAtCompileTime(str, sz)) * Tag::Latin1;
        }

        template <typename _Char>
        static constexpr sizetype lengthHelperPointer(const _Char* str) noexcept
        {
            if (is_constant_evaluated())
                return sizetype(std::char_traits<_Char>::length(str));
            if constexpr (sizeof(_Char) == sizeof(char16_t))
                return qustrlen(reinterpret_cast<const char16_t*>(str));
            else
                return sizetype(strlen(reinterpret_cast<const char*>(str)));
        }

        static Char toChar(char ch) noexcept { return toChar(Latin1Char{ ch }); } // we don't handle UTF-8 multibytes
        static Char toChar(Char ch) noexcept { return ch; }
        static Char toChar(Latin1Char ch) noexcept { return ch; }

        explicit constexpr AnyStringView(const void* d, sizetype n, std::size_t sizeAndType) noexcept
            : m_data{ d }, m_size{ std::size_t(n) | (sizeAndType & TypeMask) } {}
    public:
        constexpr AnyStringView() noexcept
            : m_data{ nullptr }, m_size{ 0 } {}
        constexpr AnyStringView(std::nullptr_t) noexcept
            : AnyStringView() {}

        template <typename _Char, if_compatible_char<_Char> = true>
        constexpr AnyStringView(const _Char* str, sizetype len)
            : m_data{ str }, m_size{ encodeType<_Char>(str, len) }
        {
        }

        template <typename _Char, if_compatible_char<_Char> = true>
        constexpr AnyStringView(const _Char* f, const _Char* l)
            : AnyStringView(f, l - f) {}

        template <typename Pointer, if_compatible_pointer<Pointer> = true>
        constexpr AnyStringView(const Pointer& str) noexcept
            : AnyStringView{ str, str ? lengthHelperPointer(str) : 0 } {}

        // defined in qstring.h
        inline AnyStringView(const ByteArray& str) noexcept; // TODO: Should we have this at all? Remove?
        inline AnyStringView(const String& str) noexcept;
        inline constexpr AnyStringView(Latin1StringView str) noexcept;

        template <typename Container, if_compatible_container<Container> = true>
        constexpr always_inline AnyStringView(const Container& c) noexcept
            : AnyStringView(std::data(c), lengthHelperContainer(c)) {}

        template <typename Container, if_convertible_to<String, Container> = true>
        constexpr AnyStringView(Container&& c, wrapped_t<Container, String>&& capacity = {})
            //noexcept(std::is_nothrow_constructible_v<QString, Container>)
            : AnyStringView(capacity = std::forward<Container>(c)) {}

        template <typename Container, if_convertible_to<ByteArray, Container> = true>
        constexpr AnyStringView(Container&& c, wrapped_t<Container, ByteArray>&& capacity = {})
            //noexcept(std::is_nothrow_constructible_v<QByteArray, Container>)
            : AnyStringView(capacity = std::forward<Container>(c)) {}

        template <typename _Char, if_compatible_char<_Char> = true>
        constexpr AnyStringView(const _Char& c) noexcept
            : AnyStringView{ &c, 1 } {}
        constexpr AnyStringView(const Char& c) noexcept
            : AnyStringView{ &c, 1 } {}

        template <typename _Char, typename Container = decltype(QChar::fromUcs4(U'x')),
            std::enable_if_t<std::is_same_v<_Char, char32_t>, bool> = true>
        constexpr AnyStringView(_Char c, Container&& capacity = {})
            : AnyStringView(capacity = QChar::fromUcs4(c)) {}

        constexpr AnyStringView(StringView v) noexcept
            : AnyStringView(std::data(v), lengthHelperContainer(v)) {}

        template <bool UseChar8T>
        constexpr AnyStringView(BasicUtf8StringView<UseChar8T> v) noexcept
            : AnyStringView(std::data(v), lengthHelperContainer(v)) {}

        template <typename _Char, size_t Size, if_compatible_char<_Char> = true>
        [[nodiscard]] constexpr static AnyStringView fromArray(const _Char(&string)[Size]) noexcept
        {
            return AnyStringView(string, Size);
        }

        // defined in qstring.h:
        template <typename Visitor>
        inline constexpr decltype(auto) visit(Visitor&& v) const;

        [[nodiscard]]
        constexpr AnyStringView mid(sizetype pos, sizetype n = -1) const
        {
            auto result = ContainerImplHelper::mid(size(), &pos, &n);
            return result == ContainerImplHelper::Null ? AnyStringView() : sliced(pos, n);
        }
        [[nodiscard]]
        constexpr AnyStringView left(sizetype n) const
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return sliced(0, n);
        }
        [[nodiscard]]
        constexpr AnyStringView right(sizetype n) const
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return sliced(size() - n, n);
        }

        [[nodiscard]] constexpr AnyStringView sliced(sizetype pos) const
        {
            verify(pos, 0); auto r = *this; r.advanceData(pos); r.setSize(size() - pos); return r;
        }
        [[nodiscard]] constexpr AnyStringView sliced(sizetype pos, sizetype n) const
        {
            verify(pos, n); auto r = *this; r.advanceData(pos); r.setSize(n); return r;
        }
        [[nodiscard]] constexpr AnyStringView first(sizetype n) const
        {
            verify(0, n); return sliced(0, n);
        }
        [[nodiscard]] constexpr AnyStringView last(sizetype n) const
        {
            verify(0, n); return sliced(size() - n, n);
        }
        [[nodiscard]] constexpr AnyStringView chopped(sizetype n) const
        {
            verify(0, n); return sliced(0, size() - n);
        }

        constexpr void truncate(sizetype n)
        {
            verify(0, n); setSize(n);
        }
        constexpr void chop(sizetype n)
        {
            verify(0, n); setSize(size() - n);
        }


        [[nodiscard]] inline String toString() const; // defined in qstring.h

        [[nodiscard]] constexpr sizetype size() const noexcept
        {
            return sizetype((m_size >> SizeShift) & SizeMask);
        }
        [[nodiscard]] constexpr const void* data() const noexcept { return m_data; }

        [[nodiscard]] static int compare(AnyStringView lhs, AnyStringView rhs, CaseSensitivity cs = CaseSensitive) noexcept;
        [[nodiscard]] static bool equal(AnyStringView lhs, AnyStringView rhs) noexcept;

        static constexpr inline bool detects_US_ASCII_at_compile_time =
#ifdef SUPPORTS_IS_CONSTANT_EVALUATED
            true
#else
            false
#endif
            ;

        //
        // STL compatibility API:
        //
        [[nodiscard]] constexpr Char front() const; // NOT noexcept!
        [[nodiscard]] constexpr Char back() const; // NOT noexcept!
        [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
        [[nodiscard]] constexpr sizetype size_bytes() const noexcept
        {
            return size() * charSize();
        }

        //
        // Qt compatibility API:
        //
        [[nodiscard]] constexpr bool isNull() const noexcept { return !m_data; }
        [[nodiscard]] constexpr bool isEmpty() const noexcept { return empty(); }
        [[nodiscard]] constexpr sizetype length() const noexcept
        {
            return size();
        }

    private:
        [[nodiscard]] friend inline bool operator==(AnyStringView lhs, AnyStringView rhs) noexcept
        {
            return AnyStringView::equal(lhs, rhs);
        }
        [[nodiscard]] friend inline bool operator!=(AnyStringView lhs, AnyStringView rhs) noexcept
        {
            return !AnyStringView::equal(lhs, rhs);
        }

        [[nodiscard]] friend inline bool operator<=(AnyStringView lhs, AnyStringView rhs) noexcept
        {
            return AnyStringView::compare(lhs, rhs) <= 0;
        }
        [[nodiscard]] friend inline bool operator>=(AnyStringView lhs, AnyStringView rhs) noexcept
        {
            return AnyStringView::compare(lhs, rhs) >= 0;
        }
        [[nodiscard]] friend inline bool operator<(AnyStringView lhs, AnyStringView rhs) noexcept
        {
            return AnyStringView::compare(lhs, rhs) < 0;
        }
        [[nodiscard]] friend inline bool operator>(AnyStringView lhs, AnyStringView rhs) noexcept
        {
            return AnyStringView::compare(lhs, rhs) > 0;
        }

        [[nodiscard]] constexpr Tag tag() const noexcept { return Tag{ m_size & TypeMask }; }
        [[nodiscard]] constexpr bool isUtf16() const noexcept { return tag() == Tag::Utf16; }

        [[nodiscard]] constexpr bool isUtf8() const noexcept { return tag() == Tag::Utf8; }
        [[nodiscard]] constexpr bool isLatin1() const noexcept { return tag() == Tag::Latin1; }

        [[nodiscard]] constexpr StringView asStringView() const
        {
            return Assert(isUtf16()), StringView{ m_data_utf16, size() };
        }
        [[nodiscard]] constexpr Utf8StringView asUtf8StringView() const
        {
            return Assert(isUtf8()), Utf8StringView{ m_data_utf8, size() };
        }
        [[nodiscard]] inline constexpr Latin1StringView asLatin1StringView() const;

        [[nodiscard]] constexpr size_t charSize() const noexcept { return isUtf16() ? 2 : 1; }
        constexpr void setSize(sizetype sz) noexcept { m_size = size_t(sz) | tag(); }

        constexpr void advanceData(sizetype delta) noexcept
        {
            m_data_utf8 += delta * charSize();
        }

        always_inline constexpr void verify([[maybe_unused]] sizetype pos = 0,
            [[maybe_unused]] sizetype n = 1) const
        {
            Assert(pos >= 0);
            Assert(pos <= size());
            Assert(n >= 0);
            Assert(n <= size() - pos);
        }
        union {
            const void* m_data;
            const char* m_data_utf8;
            const char16_t* m_data_utf16;
        };
        size_t m_size;
    };
    DECLARE_TYPEINFO(AnyStringView, PRIMITIVE_TYPE);

    template <typename StringLike, std::enable_if_t<std::disjunction_v<
        std::is_same<StringLike, String>,
        std::is_same<StringLike, ByteArray>
    >, bool> = true>
    [[nodiscard]] inline AnyStringView qToAnyStringViewIgnoringNull(const StringLike& s) noexcept
    {
        return AnyStringView(s.data(), s.size());
    }

} // namespace base::string

#pragma once 

#include <base/string/Char.h>
#include <base/string/StringView.h>

#include <base/string/StringFwd.h>


namespace base::string {

    class String;

    class Latin1String
    {
    public:
        constexpr Latin1String() noexcept : m_size(0), m_data(nullptr) {}
        template <typename = void>
            constexpr Latin1String(std::nullptr_t) noexcept : Latin1String() {}
        constexpr explicit Latin1String(const char* s) noexcept
            : m_size(s ? sizetype(lengthHelperPointer(s)) : 0), m_data(s) {}
        constexpr Latin1String(const char* f, const char* l)
            : Latin1String(f, sizetype(l - f)) {}
        constexpr Latin1String(const char* s, sizetype sz) noexcept : m_size(sz), m_data(s) {}
        explicit Latin1String(const ByteArray& s) noexcept : m_size(s.size()), m_data(s.constData()) {}
        constexpr explicit Latin1String(ByteArrayView s) noexcept : m_size(s.size()), m_data(s.data()) {}

        inline String toString() const;

        constexpr const char* latin1() const noexcept { return m_data; }
        constexpr sizetype size() const noexcept { return m_size; }
        constexpr const char* data() const noexcept { return m_data; }
        [[nodiscard]] constexpr const char* constData() const noexcept { return data(); }
        [[nodiscard]] constexpr const char* constBegin() const noexcept { return begin(); }
        [[nodiscard]] constexpr const char* constEnd() const noexcept { return end(); }

        [[nodiscard]] constexpr Latin1Char first() const { return front(); }
        [[nodiscard]] constexpr Latin1Char last() const { return back(); }

        [[nodiscard]] constexpr sizetype length() const noexcept { return size(); }

        constexpr bool isNull() const noexcept { return !data(); }
        constexpr bool isEmpty() const noexcept { return !size(); }

        [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }

        template <typename...Args>
        [[nodiscard]] inline String arg(Args &&...args) const;

        [[nodiscard]] constexpr Latin1Char at(sizetype i) const
        {
            Assert(i >= 0);
            Assert(i < size());
            return Latin1Char(m_data[i]);
        }
        [[nodiscard]] constexpr Latin1Char operator[](sizetype i) const { return at(i); }

        [[nodiscard]] constexpr Latin1Char front() const { return at(0); }
        [[nodiscard]] constexpr Latin1Char back() const { return at(size() - 1); }

        [[nodiscard]] int compare(StringView other, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return compareStrings(*this, other, cs);
        }
        [[nodiscard]] int compare(Latin1StringView other, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return compareStrings(*this, other, cs);
        }
        [[nodiscard]] inline int compare(QUtf8StringView other, CaseSensitivity cs = CaseSensitive) const noexcept;
        [[nodiscard]] constexpr int compare(Char c) const noexcept
        {
            return isEmpty() ? -1 : front() == c ? int(size() > 1) : uchar(m_data[0]) - c.unicode();
        }
        [[nodiscard]] int compare(Char c, CaseSensitivity cs) const noexcept
        {
            return compareStrings(*this, StringView(&c, 1), cs);
        }

        [[nodiscard]] bool startsWith(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return startsWith(*this, s, cs);
        }
        [[nodiscard]] bool startsWith(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return startsWith(*this, s, cs);
        }
        [[nodiscard]] constexpr bool startsWith(Char c) const noexcept
        {
            return !isEmpty() && front() == c;
        }
        [[nodiscard]] bool startsWith(Char c, CaseSensitivity cs) const noexcept
        {
            return startsWith(*this, StringView(&c, 1), cs);
        }

        [[nodiscard]] bool endsWith(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return endsWith(*this, s, cs);
        }
        [[nodiscard]] bool endsWith(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return endsWith(*this, s, cs);
        }
        [[nodiscard]] constexpr bool endsWith(Char c) const noexcept
        {
            return !isEmpty() && back() == c;
        }
        [[nodiscard]] bool endsWith(Char c, CaseSensitivity cs) const noexcept
        {
            return endsWith(*this, StringView(&c, 1), cs);
        }

        [[nodiscard]] sizetype indexOf(StringView s, sizetype from = 0, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return findString(*this, from, s, cs);
        }
        [[nodiscard]] sizetype indexOf(Latin1StringView s, sizetype from = 0, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return findString(*this, from, s, cs);
        }
        [[nodiscard]] sizetype indexOf(Char c, sizetype from = 0, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return findString(*this, from, StringView(&c, 1), cs);
        }

        [[nodiscard]] bool contains(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return indexOf(s, 0, cs) != -1;
        }
        [[nodiscard]] bool contains(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return indexOf(s, 0, cs) != -1;
        }
        [[nodiscard]] bool contains(Char c, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return indexOf(StringView(&c, 1), 0, cs) != -1;
        }

        [[nodiscard]] sizetype lastIndexOf(StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return lastIndexOf(s, size(), cs);
        }
        [[nodiscard]] sizetype lastIndexOf(StringView s, sizetype from, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return lastIndexOf(*this, from, s, cs);
        }
        [[nodiscard]] sizetype lastIndexOf(Latin1StringView s, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return lastIndexOf(s, size(), cs);
        }
        [[nodiscard]] sizetype lastIndexOf(Latin1StringView s, sizetype from, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return lastIndexOf(*this, from, s, cs);
        }
        [[nodiscard]] sizetype lastIndexOf(Char c, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return lastIndexOf(c, -1, cs);
        }
        [[nodiscard]] sizetype lastIndexOf(Char c, sizetype from, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return lastIndexOf(*this, from, StringView(&c, 1), cs);
        }

        [[nodiscard]] sizetype count(StringView str, CaseSensitivity cs = CaseSensitive) const
        {
            return count(*this, str, cs);
        }
        [[nodiscard]] sizetype count(Latin1StringView str, CaseSensitivity cs = CaseSensitive) const
        {
            return count(*this, str, cs);
        }
        [[nodiscard]] sizetype count(Char ch, CaseSensitivity cs = CaseSensitive) const noexcept
        {
            return count(*this, ch, cs);
        }

        [[nodiscard]] short toShort(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<short>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] ushort toUShort(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<ushort>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] int toInt(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<int>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] uint toUInt(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<uint>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] long toLong(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<long>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] ulong toULong(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<ulong>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] qlonglong toLongLong(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<qlonglong>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] qulonglong toULongLong(bool* ok = nullptr, int base = 10) const
        {
            return toIntegral<qulonglong>(ByteArrayView(*this), ok, base);
        }
        [[nodiscard]] float toFloat(bool* ok = nullptr) const
        {
            const auto r = toFloat(*this);
            if (ok)
                *ok = bool(r);
            return r.value_or(0.0f);
        }
        [[nodiscard]] double toDouble(bool* ok = nullptr) const
        {
            const auto r = toDouble(*this);
            if (ok)
                *ok = bool(r);
            return r.value_or(0.0);
        }

        using value_type = const char;
        using pointer = value_type*;

        using const_pointer = pointer;
        using reference = value_type&;

        using const_reference = reference;
        using iterator = value_type*;

        using const_iterator = iterator;

        using difference_type = sizetype; // violates Container concept requirements
        using size_type = sizetype;       // violates Container concept requirements

        constexpr const_iterator begin() const noexcept { return data(); }
        constexpr const_iterator cbegin() const noexcept { return data(); }

        constexpr const_iterator end() const noexcept { return data() + size(); }
        constexpr const_iterator cend() const noexcept { return data() + size(); }

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = reverse_iterator;

        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
        
        [[nodiscard]] constexpr Latin1StringView mid(sizetype pos, sizetype n = -1) const
        {
            using namespace QtPrivate;
            auto result = QContainerImplHelper::mid(size(), &pos, &n);
            return result == QContainerImplHelper::Null ? Latin1StringView()
                : Latin1StringView(m_data + pos, n);
        }
        [[nodiscard]] constexpr Latin1StringView left(sizetype n) const
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return { m_data, n };
        }
        [[nodiscard]] constexpr Latin1StringView right(sizetype n) const
        {
            if (size_t(n) >= size_t(size()))
                n = size();
            return { m_data + m_size - n, n };
        }

        [[nodiscard]] constexpr Latin1StringView sliced(sizetype pos) const
        {
            verify(pos, 0); return { m_data + pos, m_size - pos };
        }
        [[nodiscard]] constexpr Latin1StringView sliced(sizetype pos, sizetype n) const
        {
            verify(pos, n); return { m_data + pos, n };
        }
        [[nodiscard]] constexpr Latin1StringView first(sizetype n) const
        {
            verify(0, n); return sliced(0, n);
        }
        [[nodiscard]] constexpr Latin1StringView last(sizetype n) const
        {
            verify(0, n); return sliced(size() - n, n);
        }
        [[nodiscard]] constexpr Latin1StringView chopped(sizetype n) const
        {
            verify(0, n); return sliced(0, size() - n);
        }

        constexpr void chop(sizetype n)
        {
            verify(0, n); m_size -= n;
        }
        constexpr void truncate(sizetype n)
        {
            verify(0, n); m_size = n;
        }

        [[nodiscard]] Latin1StringView trimmed() const noexcept { return trimmed(*this); }

        template <typename Needle, typename...Flags>
        [[nodiscard]] constexpr auto tokenize(Needle&& needle, Flags...flags) const
            noexcept(noexcept(qTokenize(std::declval<const Latin1StringView&>(),
                std::forward<Needle>(needle), flags...)))
            -> decltype(qTokenize(*this, std::forward<Needle>(needle), flags...))
        {
            return qTokenize(*this, std::forward<Needle>(needle), flags...);
        }

        friend bool operator==(Latin1StringView s1, Latin1StringView s2) noexcept
        {
            return ByteArrayView(s1) == ByteArrayView(s2);
        }
        friend bool operator!=(Latin1StringView s1, Latin1StringView s2) noexcept
        {
            return !(s1 == s2);
        }
        friend bool operator<(Latin1StringView s1, Latin1StringView s2) noexcept
        {
            const sizetype len = qMin(s1.size(), s2.size());
            const int r = len ? memcmp(s1.latin1(), s2.latin1(), len) : 0;
            return r < 0 || (r == 0 && s1.size() < s2.size());
        }
        friend bool operator>(Latin1StringView s1, Latin1StringView s2) noexcept
        {
            return s2 < s1;
        }
        friend bool operator<=(Latin1StringView s1, Latin1StringView s2) noexcept
        {
            return !(s1 > s2);
        }
        friend bool operator>=(Latin1StringView s1, Latin1StringView s2) noexcept
        {
            return !(s1 < s2);
        }

        // Char <> Latin1StringView
        friend bool operator==(Char lhs, Latin1StringView rhs) noexcept { return rhs.size() == 1 && lhs == rhs.front(); }
        friend bool operator< (Char lhs, Latin1StringView rhs) noexcept { return compare_helper(&lhs, 1, rhs) < 0; }
        friend bool operator> (Char lhs, Latin1StringView rhs) noexcept { return compare_helper(&lhs, 1, rhs) > 0; }
        friend bool operator!=(Char lhs, Latin1StringView rhs) noexcept { return !(lhs == rhs); }
        friend bool operator<=(Char lhs, Latin1StringView rhs) noexcept { return !(lhs > rhs); }
        friend bool operator>=(Char lhs, Latin1StringView rhs) noexcept { return !(lhs < rhs); }

        friend bool operator==(Latin1StringView lhs, Char rhs) noexcept { return   rhs == lhs; }
        friend bool operator!=(Latin1StringView lhs, Char rhs) noexcept { return !(rhs == lhs); }
        friend bool operator< (Latin1StringView lhs, Char rhs) noexcept { return   rhs > lhs; }
        friend bool operator> (Latin1StringView lhs, Char rhs) noexcept { return   rhs < lhs; }
        friend bool operator<=(Latin1StringView lhs, Char rhs) noexcept { return !(rhs < lhs); }
        friend bool operator>=(Latin1StringView lhs, Char rhs) noexcept { return !(rhs > lhs); }

        // StringView <> Latin1StringView
        friend bool operator==(StringView lhs, Latin1StringView rhs) noexcept
        {
            return lhs.size() == rhs.size() && equalStrings(lhs, rhs);
        }
        friend bool operator!=(StringView lhs, Latin1StringView rhs) noexcept { return !(lhs == rhs); }
        friend bool operator< (StringView lhs, Latin1StringView rhs) noexcept { return compareStrings(lhs, rhs) < 0; }
        friend bool operator<=(StringView lhs, Latin1StringView rhs) noexcept { return compareStrings(lhs, rhs) <= 0; }
        friend bool operator> (StringView lhs, Latin1StringView rhs) noexcept { return compareStrings(lhs, rhs) > 0; }
        friend bool operator>=(StringView lhs, Latin1StringView rhs) noexcept { return compareStrings(lhs, rhs) >= 0; }

        friend bool operator==(Latin1StringView lhs, StringView rhs) noexcept
        {
            return lhs.size() == rhs.size() && equalStrings(lhs, rhs);
        }
        friend bool operator!=(Latin1StringView lhs, StringView rhs) noexcept { return !(lhs == rhs); }
        friend bool operator< (Latin1StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) < 0; }
        friend bool operator<=(Latin1StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) <= 0; }
        friend bool operator> (Latin1StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) > 0; }
        friend bool operator>=(Latin1StringView lhs, StringView rhs) noexcept { return compareStrings(lhs, rhs) >= 0; }

         inline bool operator==(const char* s) const;
         inline bool operator!=(const char* s) const;
         inline bool operator<(const char* s) const;
         inline bool operator>(const char* s) const;
         inline bool operator<=(const char* s) const;
         inline bool operator>=(const char* s) const;

         inline bool operator==(const ByteArray& s) const;
         inline bool operator!=(const ByteArray& s) const;
         inline bool operator<(const ByteArray& s) const;
         inline bool operator>(const ByteArray& s) const;
         inline bool operator<=(const ByteArray& s) const;
         inline bool operator>=(const ByteArray& s) const;

         friend bool operator==(const char* s1, Latin1StringView s2) { return compare_helper(s2, s1) == 0; }
         friend bool operator!=(const char* s1, Latin1StringView s2) { return compare_helper(s2, s1) != 0; }
         friend bool operator< (const char* s1, Latin1StringView s2) { return compare_helper(s2, s1) > 0; }
         friend bool operator> (const char* s1, Latin1StringView s2) { return compare_helper(s2, s1) < 0; }
         friend bool operator<=(const char* s1, Latin1StringView s2) { return compare_helper(s2, s1) >= 0; }
         friend bool operator>=(const char* s1, Latin1StringView s2) { return compare_helper(s2, s1) <= 0; }

    private:
        always_inline constexpr void verify([[maybe_unused]] sizetype pos,
            [[maybe_unused]] sizetype n = 1) const
        {
            Assert(pos >= 0);
            Assert(pos <= size());
            Assert(n >= 0);
            Assert(n <= size() - pos);
        }
        static int compare_helper(const Latin1StringView& s1, const char* s2) noexcept
        {
            return compare_helper(s1, s2, qstrlen(s2));
        }
         static int compare_helper(const Latin1StringView& s1, const char* s2, sizetype len) noexcept;
         static int compare_helper(const Char* data1, sizetype length1,
            Latin1StringView s2,
            CaseSensitivity cs = CaseSensitive) noexcept;

        sizetype m_size;
        const char* m_data;

    };

    DECLARE_TYPEINFO(Latin1String, RELOCATABLE_TYPE);

    namespace Literals {
        inline namespace StringLiterals {

            constexpr inline Latin1StringView operator""_L1(const char* str, size_t size) noexcept
            {
                return { str, sizetype(size) };
            }

        } // StringLiterals
    } // Literals
} // namespace base::string
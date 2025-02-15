#pragma once 

#include <stdlib.h>
#include <string.h>

#include <string>
#include <iterator>

#include <base/utility/Assert.h>

#include <base/io/ArrayDataPointer.h>
#include <base/io/ArrayData.h>

#include <base/io/ByteArrayView.h>

#include <atomic>


namespace base::io {
    using ByteArrayData = ArrayDataPointer<char>;

#  define ByteArrayLiteral(str) \
    (ByteArray(ByteArrayData(nullptr, const_cast<char *>(str), sizeof(str) - 1))) \
    /**/

    class ByteArray
    {
    public:
        using DataPointer = ByteArrayData;
    private:
        typedef QTypedArrayData<char> Data;

        DataPointer d;
        static const char _empty;

        template <typename InputIterator>
        using if_input_iterator = QtPrivate::IfIsInputIterator<InputIterator>;
    public:

        enum Base64Option {
            Base64Encoding = 0,
            Base64UrlEncoding = 1,

            KeepTrailingEquals = 0,
            OmitTrailingEquals = 2,

            IgnoreBase64DecodingErrors = 0,
            AbortOnBase64DecodingErrors = 4,
        };
        DECLARE_FLAGS(Base64Options, Base64Option)

            enum class Base64DecodingStatus {
            Ok,
            IllegalInputLength,
            IllegalCharacter,
            IllegalPadding,
        };

        inline constexpr ByteArray() noexcept;
        ByteArray(const char*, sizetype size = -1);
        ByteArray(sizetype size, char c);

        inline ByteArray(const ByteArray&) noexcept;
        inline ~ByteArray();

        ByteArray& operator=(const ByteArray&) noexcept;
        ByteArray& operator=(const char* str);
        inline ByteArray(ByteArray&& other) noexcept
            = default;

        MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(ByteArray)
            inline void swap(ByteArray& other) noexcept
        {
            d.swap(other.d);
        }

        bool isEmpty() const noexcept { return size() == 0; }
        void resize(sizetype size);
        void resize(sizetype size, char c);

        ByteArray& fill(char c, sizetype size = -1);

        inline sizetype capacity() const;
        inline void reserve(sizetype size);
        inline void squeeze();

#ifndef QT_NO_CAST_FROM_BYTEARRAY
        inline operator const char* () const;
        inline operator const void* () const;
#endif
        inline char* data();
        inline const char* data() const noexcept;
        const char* constData() const noexcept { return data(); }
        inline void detach();
        inline bool isDetached() const;
        inline bool isSharedWith(const ByteArray& other) const noexcept
        {
            return data() == other.data() && size() == other.size();
        }
        void clear();

        inline char at(sizetype i) const;
        inline char operator[](sizetype i) const;
        [[nodiscard]] inline char& operator[](sizetype i);
        [[nodiscard]] char front() const { return at(0); }
        [[nodiscard]] inline char& front();
        [[nodiscard]] char back() const { return at(size() - 1); }
        [[nodiscard]] inline char& back();

        sizetype indexOf(char c, sizetype from = 0) const;
        sizetype indexOf(ByteArrayView bv, sizetype from = 0) const
        {
            return QtPrivate::findByteArray(ToByteArrayViewIgnoringNull(*this), from, bv);
        }

        sizetype lastIndexOf(char c, sizetype from = -1) const;
        sizetype lastIndexOf(ByteArrayView bv) const
        {
            return lastIndexOf(bv, size());
        }
        sizetype lastIndexOf(ByteArrayView bv, sizetype from) const
        {
            return QtPrivate::lastIndexOf(ToByteArrayViewIgnoringNull(*this), from, bv);
        }

        inline bool contains(char c) const;
        inline bool contains(ByteArrayView bv) const;
        sizetype count(char c) const;
        sizetype count(ByteArrayView bv) const
        {
            return QtPrivate::count(ToByteArrayViewIgnoringNull(*this), bv);
        }

        inline int compare(ByteArrayView a, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;

        [[nodiscard]] ByteArray left(sizetype n) const&
        {
            if (n >= size())
                return *this;
            return first(qMax(n, 0));
        }
        [[nodiscard]] ByteArray left(sizetype n)&&
        {
            if (n >= size())
                return std::move(*this);
            return std::move(*this).first(qMax(n, 0));
        }
        [[nodiscard]] ByteArray right(sizetype n) const&
        {
            if (n >= size())
                return *this;
            return last(qMax(n, 0));
        }
        [[nodiscard]] ByteArray right(sizetype n)&&
        {
            if (n >= size())
                return std::move(*this);
            return std::move(*this).last(qMax(n, 0));
        }
        [[nodiscard]] ByteArray mid(sizetype index, sizetype len = -1) const&;
        [[nodiscard]] ByteArray mid(sizetype index, sizetype len = -1)&&;

        [[nodiscard]] ByteArray first(sizetype n) const&
        {
            verify(0, n); return sliced(0, n);
        }
        [[nodiscard]] ByteArray last(sizetype n) const&
        {
            verify(0, n); return sliced(size() - n, n);
        }
        [[nodiscard]] ByteArray sliced(sizetype pos) const&
        {
            verify(pos, 0); return sliced(pos, size() - pos);
        }
        [[nodiscard]] ByteArray sliced(sizetype pos, sizetype n) const&
        {
            verify(pos, n); return ByteArray(d.data() + pos, n);
        }
        [[nodiscard]] ByteArray chopped(sizetype len) const&
        {
            verify(0, len); return sliced(0, size() - len);
        }

        [[nodiscard]] ByteArray first(sizetype n)&&
        {
            verify(0, n);
            resize(n);      // may detach and allocate memory
            return std::move(*this);
        }
        [[nodiscard]] ByteArray last(sizetype n)&&
        {
            verify(0, n); return sliced_helper(*this, size() - n, n);
        }
        [[nodiscard]] ByteArray sliced(sizetype pos)&&
        {
            verify(pos, 0); return sliced_helper(*this, pos, size() - pos);
        }
        [[nodiscard]] ByteArray sliced(sizetype pos, sizetype n)&&
        {
            verify(pos, n); return sliced_helper(*this, pos, n);
        }
        [[nodiscard]] ByteArray chopped(sizetype len)&&
        {
            verify(0, len); return std::move(*this).first(size() - len);
        }
#endif

        bool startsWith(ByteArrayView bv) const
        {
            return QtPrivate::startsWith(qToByteArrayViewIgnoringNull(*this), bv);
        }
        bool startsWith(char c) const { return size() > 0 && front() == c; }

        bool endsWith(char c) const { return size() > 0 && back() == c; }
        bool endsWith(ByteArrayView bv) const
        {
            return QtPrivate::endsWith(qToByteArrayViewIgnoringNull(*this), bv);
        }

        bool isUpper() const;
        bool isLower() const;

        [[nodiscard]] bool isValidUtf8() const noexcept
        {
            return QtPrivate::isValidUtf8(qToByteArrayViewIgnoringNull(*this));
        }

        void truncate(sizetype pos);
        void chop(sizetype n);

#if !defined(Q_QDOC)
        [[nodiscard]] ByteArray toLower() const&
        {
            return toLower_helper(*this);
        }
        [[nodiscard]] ByteArray toLower()&&
        {
            return toLower_helper(*this);
        }
        [[nodiscard]] ByteArray toUpper() const&
        {
            return toUpper_helper(*this);
        }
        [[nodiscard]] ByteArray toUpper()&&
        {
            return toUpper_helper(*this);
        }
        [[nodiscard]] ByteArray trimmed() const&
        {
            return trimmed_helper(*this);
        }
        [[nodiscard]] ByteArray trimmed()&&
        {
            return trimmed_helper(*this);
        }
        [[nodiscard]] ByteArray simplified() const&
        {
            return simplified_helper(*this);
        }
        [[nodiscard]] ByteArray simplified()&&
        {
            return simplified_helper(*this);
        }
#else
        [[nodiscard]] ByteArray toLower() const;
        [[nodiscard]] ByteArray toUpper() const;
        [[nodiscard]] ByteArray trimmed() const;
        [[nodiscard]] ByteArray simplified() const;
#endif

        [[nodiscard]] ByteArray leftJustified(sizetype width, char fill = ' ', bool truncate = false) const;
        [[nodiscard]] ByteArray rightJustified(sizetype width, char fill = ' ', bool truncate = false) const;

        ByteArray& prepend(char c)
        {
            return insert(0, ByteArrayView(&c, 1));
        }
        inline ByteArray& prepend(sizetype count, char c);
        ByteArray& prepend(const char* s)
        {
            return insert(0, ByteArrayView(s, sizetype(qstrlen(s))));
        }
        ByteArray& prepend(const char* s, sizetype len)
        {
            return insert(0, ByteArrayView(s, len));
        }
        ByteArray& prepend(const ByteArray& a);
        ByteArray& prepend(ByteArrayView a)
        {
            return insert(0, a);
        }

        ByteArray& append(char c);
        inline ByteArray& append(sizetype count, char c);
        ByteArray& append(const char* s)
        {
            return append(s, -1);
        }
        ByteArray& append(const char* s, sizetype len)
        {
            return append(ByteArrayView(s, len < 0 ? sizetype(qstrlen(s)) : len));
        }
        ByteArray& append(const ByteArray& a);
        ByteArray& append(ByteArrayView a)
        {
            return insert(size(), a);
        }

        ByteArray& assign(ByteArrayView v);
        ByteArray& assign(sizetype n, char c)
        {
            Q_ASSERT(n >= 0);
            return fill(c, n);
        }
        template <typename InputIterator, if_input_iterator<InputIterator> = true>
        ByteArray& assign(InputIterator first, InputIterator last)
        {
            d.assign(first, last);
            d.data()[d.size] = '\0';
            return *this;
        }

        ByteArray& insert(sizetype i, ByteArrayView data);
        inline ByteArray& insert(sizetype i, const char* s)
        {
            return insert(i, ByteArrayView(s));
        }
        inline ByteArray& insert(sizetype i, const ByteArray& data)
        {
            return insert(i, ByteArrayView(data));
        }
        ByteArray& insert(sizetype i, sizetype count, char c);
        ByteArray& insert(sizetype i, char c)
        {
            return insert(i, ByteArrayView(&c, 1));
        }
        ByteArray& insert(sizetype i, const char* s, sizetype len)
        {
            return insert(i, ByteArrayView(s, len));
        }

        ByteArray& remove(sizetype index, sizetype len);
        ByteArray& removeAt(sizetype pos)
        {
            return size_t(pos) < size_t(size()) ? remove(pos, 1) : *this;
        }
        ByteArray& removeFirst() { return !isEmpty() ? remove(0, 1) : *this; }
        ByteArray& removeLast() { return !isEmpty() ? remove(size() - 1, 1) : *this; }

        template <typename Predicate>
        ByteArray& removeIf(Predicate pred)
        {
            removeIf_helper(pred);
            return *this;
        }

        ByteArray& replace(sizetype index, sizetype len, const char* s, sizetype alen)
        {
            return replace(index, len, ByteArrayView(s, alen));
        }
        ByteArray& replace(sizetype index, sizetype len, ByteArrayView s);
        ByteArray& replace(char before, ByteArrayView after)
        {
            return replace(ByteArrayView(&before, 1), after);
        }
        ByteArray& replace(const char* before, sizetype bsize, const char* after, sizetype asize)
        {
            return replace(ByteArrayView(before, bsize), ByteArrayView(after, asize));
        }
        ByteArray& replace(ByteArrayView before, ByteArrayView after);
        ByteArray& replace(char before, char after);

        ByteArray& operator+=(char c)
        {
            return append(c);
        }
        ByteArray& operator+=(const char* s)
        {
            return append(s);
        }
        ByteArray& operator+=(const ByteArray& a)
        {
            return append(a);
        }
        ByteArray& operator+=(ByteArrayView a)
        {
            return append(a);
        }

        QList<ByteArray> split(char sep) const;

        [[nodiscard]] ByteArray repeated(sizetype times) const;

#if !defined(QT_NO_CAST_FROM_ASCII) && !defined(QT_RESTRICTED_CAST_FROM_ASCII)
        QT_ASCII_CAST_WARN inline bool operator==(const QString& s2) const;
        QT_ASCII_CAST_WARN inline bool operator!=(const QString& s2) const;
        QT_ASCII_CAST_WARN inline bool operator<(const QString& s2) const;
        QT_ASCII_CAST_WARN inline bool operator>(const QString& s2) const;
        QT_ASCII_CAST_WARN inline bool operator<=(const QString& s2) const;
        QT_ASCII_CAST_WARN inline bool operator>=(const QString& s2) const;
#endif
        friend inline bool operator==(const ByteArray& a1, const ByteArray& a2) noexcept
        {
            return ByteArrayView(a1) == ByteArrayView(a2);
        }
        friend inline bool operator==(const ByteArray& a1, const char* a2) noexcept
        {
            return ByteArrayView(a1) == ByteArrayView(a2);
        }
        friend inline bool operator==(const char* a1, const ByteArray& a2) noexcept
        {
            return ByteArrayView(a1) == ByteArrayView(a2);
        }
        friend inline bool operator!=(const ByteArray& a1, const ByteArray& a2) noexcept
        {
            return !(a1 == a2);
        }
        friend inline bool operator!=(const ByteArray& a1, const char* a2) noexcept
        {
            return ByteArrayView(a1) != ByteArrayView(a2);
        }
        friend inline bool operator!=(const char* a1, const ByteArray& a2) noexcept
        {
            return ByteArrayView(a1) != ByteArrayView(a2);
        }
        friend inline bool operator<(const ByteArray& a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(ByteArrayView(a1), ByteArrayView(a2)) < 0;
        }
        friend inline bool operator<(const ByteArray& a1, const char* a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) < 0;
        }
        friend inline bool operator<(const char* a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) < 0;
        }
        friend inline bool operator<=(const ByteArray& a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(ByteArrayView(a1), ByteArrayView(a2)) <= 0;
        }
        friend inline bool operator<=(const ByteArray& a1, const char* a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) <= 0;
        }
        friend inline bool operator<=(const char* a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) <= 0;
        }
        friend inline bool operator>(const ByteArray& a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(ByteArrayView(a1), ByteArrayView(a2)) > 0;
        }
        friend inline bool operator>(const ByteArray& a1, const char* a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) > 0;
        }
        friend inline bool operator>(const char* a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) > 0;
        }
        friend inline bool operator>=(const ByteArray& a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(ByteArrayView(a1), ByteArrayView(a2)) >= 0;
        }
        friend inline bool operator>=(const ByteArray& a1, const char* a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) >= 0;
        }
        friend inline bool operator>=(const char* a1, const ByteArray& a2) noexcept
        {
            return QtPrivate::compareMemory(a1, a2) >= 0;
        }

        // Check isEmpty() instead of isNull() for backwards compatibility.
        friend inline bool operator==(const ByteArray& a1, std::nullptr_t) noexcept { return a1.isEmpty(); }
        friend inline bool operator!=(const ByteArray& a1, std::nullptr_t) noexcept { return !a1.isEmpty(); }
        friend inline bool operator< (const ByteArray&, std::nullptr_t) noexcept { return false; }
        friend inline bool operator> (const ByteArray& a1, std::nullptr_t) noexcept { return !a1.isEmpty(); }
        friend inline bool operator<=(const ByteArray& a1, std::nullptr_t) noexcept { return a1.isEmpty(); }
        friend inline bool operator>=(const ByteArray&, std::nullptr_t) noexcept { return true; }

        friend inline bool operator==(std::nullptr_t, const ByteArray& a2) noexcept { return a2 == nullptr; }
        friend inline bool operator!=(std::nullptr_t, const ByteArray& a2) noexcept { return a2 != nullptr; }
        friend inline bool operator< (std::nullptr_t, const ByteArray& a2) noexcept { return a2 > nullptr; }
        friend inline bool operator> (std::nullptr_t, const ByteArray& a2) noexcept { return a2 < nullptr; }
        friend inline bool operator<=(std::nullptr_t, const ByteArray& a2) noexcept { return a2 >= nullptr; }
        friend inline bool operator>=(std::nullptr_t, const ByteArray& a2) noexcept { return a2 <= nullptr; }

        short toShort(bool* ok = nullptr, int base = 10) const;
        ushort toUShort(bool* ok = nullptr, int base = 10) const;
        int toInt(bool* ok = nullptr, int base = 10) const;
        uint toUInt(bool* ok = nullptr, int base = 10) const;
        long toLong(bool* ok = nullptr, int base = 10) const;
        ulong toULong(bool* ok = nullptr, int base = 10) const;
        qlonglong toLongLong(bool* ok = nullptr, int base = 10) const;
        qulonglong toULongLong(bool* ok = nullptr, int base = 10) const;
        float toFloat(bool* ok = nullptr) const;
        double toDouble(bool* ok = nullptr) const;
        ByteArray toBase64(Base64Options options = Base64Encoding) const;
        ByteArray toHex(char separator = '\0') const;
        ByteArray toPercentEncoding(const ByteArray& exclude = ByteArray(),
            const ByteArray& include = ByteArray(),
            char percent = '%') const;
        [[nodiscard]] ByteArray percentDecoded(char percent = '%') const;

        inline ByteArray& setNum(short, int base = 10);
        inline ByteArray& setNum(ushort, int base = 10);
        inline ByteArray& setNum(int, int base = 10);
        inline ByteArray& setNum(uint, int base = 10);
        inline ByteArray& setNum(long, int base = 10);
        inline ByteArray& setNum(ulong, int base = 10);
        ByteArray& setNum(qlonglong, int base = 10);
        ByteArray& setNum(qulonglong, int base = 10);
        inline ByteArray& setNum(float, char format = 'g', int precision = 6);
        ByteArray& setNum(double, char format = 'g', int precision = 6);
        ByteArray& setRawData(const char* a, sizetype n);

        [[nodiscard]] static ByteArray number(int, int base = 10);
        [[nodiscard]] static ByteArray number(uint, int base = 10);
        [[nodiscard]] static ByteArray number(long, int base = 10);
        [[nodiscard]] static ByteArray number(ulong, int base = 10);
        [[nodiscard]] static ByteArray number(qlonglong, int base = 10);
        [[nodiscard]] static ByteArray number(qulonglong, int base = 10);
        [[nodiscard]] static ByteArray number(double, char format = 'g', int precision = 6);
        [[nodiscard]] static ByteArray fromRawData(const char* data, sizetype size)
        {
            return ByteArray(DataPointer(nullptr, const_cast<char*>(data), size));
        }

        class FromBase64Result;
        [[nodiscard]] static FromBase64Result fromBase64Encoding(ByteArray&& base64, Base64Options options = Base64Encoding);
        [[nodiscard]] static FromBase64Result fromBase64Encoding(const ByteArray& base64, Base64Options options = Base64Encoding);
        [[nodiscard]] static ByteArray fromBase64(const ByteArray& base64, Base64Options options = Base64Encoding);
        [[nodiscard]] static ByteArray fromHex(const ByteArray& hexEncoded);
        [[nodiscard]] static ByteArray fromPercentEncoding(const ByteArray& pctEncoded, char percent = '%');

#if defined(Q_OS_DARWIN) || defined(Q_QDOC)
        static ByteArray fromCFData(CFDataRef data);
        static ByteArray fromRawCFData(CFDataRef data);
        CFDataRef toCFData() const Q_DECL_CF_RETURNS_RETAINED;
        CFDataRef toRawCFData() const Q_DECL_CF_RETURNS_RETAINED;
        static ByteArray fromNSData(const NSData* data);
        static ByteArray fromRawNSData(const NSData* data);
        NSData* toNSData() const Q_DECL_NS_RETURNS_AUTORELEASED;
        NSData* toRawNSData() const Q_DECL_NS_RETURNS_AUTORELEASED;
#endif

#if defined(Q_OS_WASM) || defined(Q_QDOC)
        static ByteArray fromEcmaUint8Array(emscripten::val uint8array);
        emscripten::val toEcmaUint8Array();
#endif

        typedef char* iterator;
        typedef const char* const_iterator;
        typedef iterator Iterator;
        typedef const_iterator ConstIterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        iterator begin() { return data(); }
        const_iterator begin() const noexcept { return data(); }
        const_iterator cbegin() const noexcept { return begin(); }
        const_iterator constBegin() const noexcept { return begin(); }
        iterator end() { return begin() + size(); }
        const_iterator end() const noexcept { return begin() + size(); }
        const_iterator cend() const noexcept { return end(); }
        const_iterator constEnd() const noexcept { return end(); }
        reverse_iterator rbegin() { return reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        const_reverse_iterator crend() const noexcept { return rend(); }

        // stl compatibility
        typedef sizetype size_type;
        typedef qptrdiff difference_type;
        typedef const char& const_reference;
        typedef char& reference;
        typedef char* pointer;
        typedef const char* const_pointer;
        typedef char value_type;
        void push_back(char c)
        {
            append(c);
        }
        void push_back(const char* s)
        {
            append(s);
        }
        void push_back(const ByteArray& a)
        {
            append(a);
        }
        void push_back(ByteArrayView a)
        {
            append(a);
        }
        void push_front(char c)
        {
            prepend(c);
        }
        void push_front(const char* c)
        {
            prepend(c);
        }
        void push_front(const ByteArray& a)
        {
            prepend(a);
        }
        void push_front(ByteArrayView a)
        {
            prepend(a);
        }
        void shrink_to_fit() { squeeze(); }
        iterator erase(const_iterator first, const_iterator last);
        inline iterator erase(const_iterator it) { return erase(it, it + 1); }

        static ByteArray fromStdString(const std::string& s);
        std::string toStdString() const;

        inline sizetype size() const noexcept { return d->size; }
#if QT_DEPRECATED_SINCE(6, 4)
        QT_DEPRECATED_VERSION_X_6_4("Use size() or length() instead.")
            inline sizetype count() const noexcept { return size(); }
#endif
        inline sizetype length() const noexcept { return size(); }
        QT_CORE_INLINE_SINCE(6, 4)
            bool isNull() const noexcept;

        inline const DataPointer& data_ptr() const { return d; }
        inline DataPointer& data_ptr() { return d; }
#if QT_VERSION < QT_VERSION_CHECK(7, 0, 0)
        explicit inline ByteArray(const DataPointer& dd) : d(dd) {}
#endif
        explicit inline ByteArray(DataPointer&& dd) : d(std::move(dd)) {}

    private:
        void reallocData(sizetype alloc, QArrayData::AllocationOption option);
        void reallocGrowData(sizetype n);
        void expand(sizetype i);

        Q_ALWAYS_INLINE constexpr void verify([[maybe_unused]] sizetype pos = 0,
            [[maybe_unused]] sizetype n = 1) const
        {
            Q_ASSERT(pos >= 0);
            Q_ASSERT(pos <= d.size);
            Q_ASSERT(n >= 0);
            Q_ASSERT(n <= d.size - pos);
        }

        static ByteArray sliced_helper(ByteArray& a, sizetype pos, sizetype n);
        static ByteArray toLower_helper(const ByteArray& a);
        static ByteArray toLower_helper(ByteArray& a);
        static ByteArray toUpper_helper(const ByteArray& a);
        static ByteArray toUpper_helper(ByteArray& a);
        static ByteArray trimmed_helper(const ByteArray& a);
        static ByteArray trimmed_helper(ByteArray& a);
        static ByteArray simplified_helper(const ByteArray& a);
        static ByteArray simplified_helper(ByteArray& a);
        template <typename Predicate>
        sizetype removeIf_helper(Predicate pred)
        {
            const sizetype result = d->eraseIf(pred);
            if (result > 0)
                d.data()[d.size] = '\0';
            return result;
        }

        friend class QString;
        friend Q_CORE_EXPORT ByteArray Uncompress(const uchar* data, sizetype nbytes);

        template <typename T> friend sizetype erase(ByteArray& ba, const T& t);
        template <typename Predicate> friend sizetype erase_if(ByteArray& ba, Predicate pred);
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(ByteArray::Base64Options)

        inline constexpr ByteArray::ByteArray() noexcept {}
    inline ByteArray::~ByteArray() {}

    inline char ByteArray::at(sizetype i) const
    {
        verify(i, 1); return d.data()[i];
    }
    inline char ByteArray::operator[](sizetype i) const
    {
        verify(i, 1); return d.data()[i];
    }

#ifndef QT_NO_CAST_FROM_BYTEARRAY
    inline ByteArray::operator const char* () const
    {
        return data();
    }
    inline ByteArray::operator const void* () const
    {
        return data();
    }
#endif
    inline char* ByteArray::data()
    {
        detach();
        Q_ASSERT(d.data());
        return d.data();
    }
    inline const char* ByteArray::data() const noexcept
    {
#if QT5_NULL_STRINGS == 1
        return d.data() ? d.data() : &_empty;
#else
        return d.data();
#endif
    }
    inline void ByteArray::detach()
    {
        if (d->needsDetach()) reallocData(size(), QArrayData::KeepSize);
    }
    inline bool ByteArray::isDetached() const
    {
        return !d->isShared();
    }
    inline ByteArray::ByteArray(const ByteArray& a) noexcept : d(a.d)
    {}

    inline sizetype ByteArray::capacity() const { return sizetype(d->constAllocatedCapacity()); }

    inline void ByteArray::reserve(sizetype asize)
    {
        if (d->needsDetach() || asize > capacity() - d->freeSpaceAtBegin())
            reallocData(qMax(size(), asize), QArrayData::KeepSize);
        if (d->constAllocatedCapacity())
            d->setFlag(Data::CapacityReserved);
    }

    inline void ByteArray::squeeze()
    {
        if (!d.isMutable())
            return;
        if (d->needsDetach() || size() < capacity())
            reallocData(size(), QArrayData::KeepSize);
        if (d->constAllocatedCapacity())
            d->clearFlag(Data::CapacityReserved);
    }

    inline char& ByteArray::operator[](sizetype i)
    {
        verify(i, 1); return data()[i];
    }
    inline char& ByteArray::front() { return operator[](0); }
    inline char& ByteArray::back() { return operator[](size() - 1); }
    inline ByteArray& ByteArray::append(sizetype n, char ch)
    {
        return insert(size(), n, ch);
    }
    inline ByteArray& ByteArray::prepend(sizetype n, char ch)
    {
        return insert(0, n, ch);
    }
    inline bool ByteArray::contains(char c) const
    {
        return indexOf(c) != -1;
    }
    inline bool ByteArray::contains(ByteArrayView bv) const
    {
        return indexOf(bv) != -1;
    }
    inline int ByteArray::compare(ByteArrayView a, Qt::CaseSensitivity cs) const noexcept
    {
        return cs == Qt::CaseSensitive ? QtPrivate::compareMemory(*this, a) :
            qstrnicmp(data(), size(), a.data(), a.size());
    }
#if !defined(QT_USE_QSTRINGBUILDER)
    inline ByteArray operator+(const ByteArray& a1, const ByteArray& a2)
    {
        return ByteArray(a1) += a2;
    }
    inline ByteArray operator+(ByteArray&& lhs, const ByteArray& rhs)
    {
        return std::move(lhs += rhs);
    }
    inline ByteArray operator+(const ByteArray& a1, const char* a2)
    {
        return ByteArray(a1) += a2;
    }
    inline ByteArray operator+(ByteArray&& lhs, const char* rhs)
    {
        return std::move(lhs += rhs);
    }
    inline ByteArray operator+(const ByteArray& a1, char a2)
    {
        return ByteArray(a1) += a2;
    }
    inline ByteArray operator+(ByteArray&& lhs, char rhs)
    {
        return std::move(lhs += rhs);
    }
    inline ByteArray operator+(const char* a1, const ByteArray& a2)
    {
        return ByteArray(a1) += a2;
    }
    inline ByteArray operator+(char a1, const ByteArray& a2)
    {
        return ByteArray(&a1, 1) += a2;
    }
#endif // QT_USE_QSTRINGBUILDER


} // namespace base
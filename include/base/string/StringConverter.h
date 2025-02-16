#pragma once

#include <base/string/String.h>
#include <base/string/StringConverterBase.h>

namespace base::string {

    class StringEncoder : public StringConverter
    {
    protected:
        constexpr explicit StringEncoder(const Interface* i) noexcept
            : StringConverter(i)
        {}
    public:
        constexpr StringEncoder() noexcept
            : StringConverter()
        {}
        constexpr explicit StringEncoder(Encoding encoding, Flags flags = Flag::Default)
            : StringConverter(encoding, flags)
        {}
        explicit StringEncoder(const char* name, Flags flags = Flag::Default)
            : StringConverter(name, flags)
        {}

        template<typename T>
        struct DecodedData
        {
            StringEncoder* encoder;
            T data;
            operator QByteArray() const { return encoder->encodeAsByteArray(data); }
        };
        template <typename = void>
            DecodedData<const String&> operator()(const String& str)
        {
            return DecodedData<const String&>{this, str};
        }
        DecodedData<StringView> operator()(StringView in)
        {
            return DecodedData<StringView>{this, in};
        }
        template <typename = void>
            DecodedData<const String&> encode(const String& str)
        {
            return DecodedData<const String&>{this, str};
        }
        DecodedData<StringView> encode(StringView in)
        {
            return DecodedData<StringView>{this, in};
        }

        qsizetype requiredSpace(qsizetype inputLength) const
        {
            return iface ? iface->fromUtf16Len(inputLength) : 0;
        }
        char* appendToBuffer(char* out, StringView in)
        {
            if (!iface) {
                state.invalidChars = 1;
                return out;
            }
            return iface->fromUtf16(out, in, &state);
        }
    private:
        io::ByteArray encodeAsByteArray(StringView in)
        {
            if (!iface) {
                // ensure that hasError returns true
                state.invalidChars = 1;
                return {};
            }
            io::ByteArray result(iface->fromUtf16Len(in.size()), Qt::Uninitialized);
            char* out = result.data();
            out = iface->fromUtf16(out, in, &state);
            result.truncate(out - result.constData());
            return result;
        }

    };

    class StringDecoder : public StringConverter
    {
    protected:
        constexpr explicit StringDecoder(const Interface* i) noexcept
            : StringConverter(i)
        {}
    public:
        constexpr explicit StringDecoder(Encoding encoding, Flags flags = Flag::Default)
            : StringConverter(encoding, flags)
        {}
        constexpr StringDecoder() noexcept
            : StringConverter()
        {}
        explicit StringDecoder(const char* name, Flags f = Flag::Default)
            : StringConverter(name, f)
        {}

        template<typename T>
        struct EncodedData
        {
            StringDecoder* decoder;
            T data;
            operator String() const { return decoder->decodeAsString(data); }
        };
        template <typename = void>
            EncodedData<const io::ByteArray&> operator()(const io::ByteArray& ba)
        {
            return EncodedData<const io::ByteArray&>{this, ba};
        }
        EncodedData<io::ByteArrayView> operator()(io::ByteArrayView ba)
        {
            return EncodedData<io::ByteArrayView>{this, ba};
        }
        template <typename = void>
            EncodedData<const io::ByteArray&> decode(const io::ByteArray& ba)
        {
            return EncodedData<const io::ByteArray&>{this, ba};
        }
        EncodedData<io::ByteArrayView> decode(io::ByteArrayView ba)
        {
            return EncodedData<io::ByteArrayView>{this, ba};
        }

        sizetype requiredSpace(sizetype inputLength) const
        {
            return iface ? iface->toUtf16Len(inputLength) : 0;
        }

        Char* appendToBuffer(Char* out, io::ByteArrayView ba)
        {
            if (!iface) {
                state.invalidChars = 1;
                return out;
            }

            return iface->toUtf16(out, ba, &state);
        }
        char16_t* appendToBuffer(char16_t* out, io::ByteArrayView ba)
        {
            return reinterpret_cast<char16_t*>(appendToBuffer(reinterpret_cast<Char*>(out), ba));
        }

        static StringDecoder decoderForHtml(io::ByteArrayView data);

    private:
        String decodeAsString(io::ByteArrayView in)
        {
            if (!iface) {
                // ensure that hasError returns true
                state.invalidChars = 1;
                return {};
            }
            String result(iface->toUtf16Len(in.size()), Qt::Uninitialized);
            const QChar* out = iface->toUtf16(result.data(), in, &state);
            result.truncate(out - result.constData());
            return result;
        }
    };

    template <typename InputIterator>
    void String::assign_helper_char8(InputIterator first, InputIterator last)
    {
        static_assert(!String::is_contiguous_iterator_v<InputIterator>,
            "Internal error: Should have been handed over to the QAnyStringView overload."
            );

        using ValueType = typename std::iterator_traits<InputIterator>::value_type;
        constexpr bool IsFwdIt = std::is_convertible_v<
            typename std::iterator_traits<InputIterator>::iterator_category,
            std::forward_iterator_tag
        >;

        resize(0);
        // In case of not being shared, there is the possibility of having free space at begin
        // even after the resize to zero.
        if (const auto offset = d.freeSpaceAtBegin())
            d.setBegin(d.begin() - offset);

        if constexpr (IsFwdIt)
            reserve(static_cast<qsizetype>(std::distance(first, last)));

        auto toUtf16 = StringDecoder(StringDecoder::Utf8);
        auto availableCapacity = d.constAllocatedCapacity();
        auto* dst = d.data();
        auto* dend = d.data() + availableCapacity;

        while (true) {
            if (first == last) {                                    // ran out of input elements
                Q_ASSERT(!std::less<>{}(dend, dst));
                d.size = dst - d.begin();
                return;
            }
            const ValueType next = *first; // decays proxies, if any
            const auto chunk = QUtf8StringView(&next, 1);
            // UTF-8 characters can have a maximum size of 4 bytes and may result in a surrogate
            // pair of UTF-16 code units. In the input-iterator case, we don't know the size
            // and would need to always reserve space for 2 code units. To keep our promise
            // of 'not allocating if it fits', we have to pre-check this condition.
            //          We know that it fits in the forward-iterator case.
            if constexpr (!IsFwdIt) {
                constexpr qsizetype Pair = 2;
                char16_t buf[Pair];
                const qptrdiff n = toUtf16.appendToBuffer(buf, chunk) - buf;
                if (dend - dst < n) {                               // ran out of allocated memory
                    const auto offset = dst - d.begin();
                    reallocData(d.constAllocatedCapacity() + Pair, QArrayData::Grow);
                    // update the pointers since we've re-allocated
                    availableCapacity = d.constAllocatedCapacity();
                    dst = d.data() + offset;
                    dend = d.data() + availableCapacity;
                }
                dst = std::copy_n(buf, n, dst);
            }
            else {                                                // take the fast path
                dst = toUtf16.appendToBuffer(dst, chunk);
            }
            ++first;
        }
    }
} // namespace base::string
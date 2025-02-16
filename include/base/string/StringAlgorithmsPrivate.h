#pragma once 


#include <base/string/String.h>


namespace base::string {
    template <typename StringType> struct StringAlgorithms
    {
        typedef typename StringType::value_type Char;
        typedef typename StringType::size_type size_type;

        typedef typename std::remove_cv<StringType>::type NakedStringType;
        static const bool isConst = std::is_const<StringType>::value;

        static inline bool isSpace(char ch) { return ascii_isspace(ch); }
        static inline bool isSpace(QChar ch) { return ch.isSpace(); }

        // Surrogate pairs are not handled in either of the functions below. That is
        // not a problem because there are no space characters (Zs, Zl, Zp) outside the
        // Basic Multilingual Plane.

        static inline StringType trimmed_helper_inplace(NakedStringType& str, const Char* begin, const Char* end)
        {
            // in-place trimming:
            Char* data = const_cast<Char*>(str.cbegin());
            if (begin != data)
                memmove(data, begin, (end - begin) * sizeof(Char));
            str.resize(end - begin);
            return std::move(str);
        }

        static inline StringType trimmed_helper_inplace(const NakedStringType&, const Char*, const Char*)
        {
            // can't happen
            AssertUnreachable(StringType());
        }

        struct TrimPositions {
            const Char* begin;
            const Char* end;
        };
        // Returns {begin, end} where:
        // - "begin" refers to the first non-space character
        // - if there is a sequence of one or more space chacaters at the end,
        //   "end" refers to the first character in that sequence, otherwise
        //   "end" is str.cend()
        [[nodiscard]] static TrimPositions trimmed_helper_positions(const StringType& str)
        {
            const Char* begin = str.cbegin();
            const Char* end = str.cend();
            // skip white space from end
            while (begin < end && isSpace(end[-1]))
                --end;
            // skip white space from start
            while (begin < end && isSpace(*begin))
                begin++;
            return { begin, end };
        }

        static inline StringType trimmed_helper(StringType& str)
        {
            const auto [begin, end] = trimmed_helper_positions(str);
            if (begin == str.cbegin() && end == str.cend())
                return str;
            if (!isConst && str.isDetached())
                return trimmed_helper_inplace(str, begin, end);
            return StringType(begin, end - begin);
        }

        static inline StringType simplified_helper(StringType& str)
        {
            if (str.isEmpty())
                return str;
            const Char* src = str.cbegin();
            const Char* end = str.cend();
            NakedStringType result = isConst || !str.isDetached() ?
                StringType(str.size(), Qt::Uninitialized) :
                std::move(str);

            Char* dst = const_cast<Char*>(result.cbegin());
            Char* ptr = dst;
            bool unmodified = true;
            forever{
                while (src != end && isSpace(*src))
                    ++src;
                while (src != end && !isSpace(*src))
                    *ptr++ = *src++;
                if (src == end)
                    break;
                if (*src != Char::Space)
                    unmodified = false;
                *ptr++ = Char::Space;
            }
                if (ptr != dst && ptr[-1] == Char::Space)
                    --ptr;

            sizetype newlen = ptr - dst;
            if (isConst && newlen == str.size() && unmodified) {
                // nothing happened, return the original
                return str;
            }
            result.resize(newlen);
            return result;
        }
    };
}
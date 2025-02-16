#pragma once 

#include <base/string/String.h>
#include <base/string/StringView.h>

namespace base::string {
    class StringMatcherPrivate;

    class StringMatcher
    {
        void updateSkipTable();
    public:
        StringMatcher() = default;
        explicit StringMatcher(const QString& pattern,
            CaseSensitivity cs = CaseSensitive);
        StringMatcher(const QChar* uc, sizetype len,
            CaseSensitivity cs = CaseSensitive)
            : StringMatcher(StringView(uc, len), cs)
        {}
        StringMatcher(StringView pattern,
            CaseSensitivity cs = CaseSensitive);
        StringMatcher(const StringMatcher& other);
        ~StringMatcher();

        StringMatcher& operator=(const StringMatcher& other);

        void setPattern(const String& pattern);
        void setCaseSensitivity(CaseSensitivity cs);

        sizetype indexIn(const String& str, sizetype from = 0) const
        {
            return indexIn(StringView(str), from);
        }

        sizetype indexIn(const Char* str, sizetype length, sizetype from = 0) const
        {
            return indexIn(StringView(str, length), from);
        }

        sizetype indexIn(StringView str, sizetype from = 0) const;

        String pattern() const;

        StringView patternView() const noexcept
        {
            return q_sv;
        }

        inline CaseSensitivity caseSensitivity() const { return q_cs; }

    private:
        StringMatcherPrivate* d_ptr = nullptr;
        CaseSensitivity q_cs = CaseSensitive;
        String q_pattern;
        StringView q_sv;
        uchar q_skiptable[256] = {};
    };
} // namespace base::string
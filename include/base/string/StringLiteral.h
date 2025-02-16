#pragma once 

#include <base/io/ArrayData.h>
#include <base/io/ArrayDataPointer.h>

namespace base::string {

    // all our supported compilers support Unicode string literals,
    // even if their Q_COMPILER_UNICODE_STRING has been revoked due
    // to lacking stdlib support. But QStringLiteral only needs the
    // core language feature, so just use u"" here unconditionally:

    #define UNICODE_LITERAL(str) u"" str

    using StringPrivate = io::ArrayDataPointer<char16_t>;

    template <sizetype N>
    static always_inline StringPrivate MakeStringPrivate(const char16_t(&literal)[N])
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
        auto str = const_cast<char16_t*>(literal);
        return { nullptr, str, N - 1 };
    }

#define StringLiteral(str) \
    (String(MakeStringPrivate(UNICODE_LITERAL(str)))) \
    /**/

} // namespace base::string

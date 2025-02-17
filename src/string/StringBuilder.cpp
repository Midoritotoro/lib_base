#include <base/string/StringBuilder.h>
#include <base/string/StringConverterPrivate.h>

namespace base::string {
    void AbstractConcatenable::convertFromUtf8(ByteArrayView in, Char*& out) noexcept
    {
        out = convertToUnicode(out, in);
    }
}
#pragma once 

#include <base/core/string/Char.h>

#include <base/core/string/StringIterator.h>
#include <base/core/utility/TypeTraits.h>

#include <vector>
#include <src/core/string/BasicString.h>

__BASE_STRING_NAMESPACE_BEGIN

class StringView;

NODISCARD CONSTEXPR_CXX20 auto Char::fromUcs4(char32_t c) noexcept
{
	struct R {
		char16_t chars[2];
		//NODISCARD CONSTEXPR_CXX20 operator BasicString<char16_t, CharTraits<char16_t>, std::allocator<char16_t>>() const noexcept { return { begin(), end() }; }
		NODISCARD CONSTEXPR_CXX20 sizetype size() const noexcept { return chars[1] ? 2 : 1; }
		NODISCARD CONSTEXPR_CXX20 const char16_t* begin() const noexcept { return chars; }
		NODISCARD CONSTEXPR_CXX20 const char16_t* end() const noexcept { return begin() + size(); }
	};
	return requiresSurrogates(c) ? R{ {Char::highSurrogate(c),
									  Char::lowSurrogate(c)} } :
		R{ {char16_t(c), u'\0'} };
}

using String		= BasicString<char, std::char_traits<char>>;

#if __cpp_lib_char8_t

using Utf8String	= BasicString<char8_t, std::char_traits<char8_t>>;

#endif

using Utf16String	= BasicString<char16_t, std::char_traits<char16_t>>;
using WString		= BasicString<wchar_t, std::char_traits<wchar_t>>;

using Utf32String	= BasicString<char32_t, std::char_traits<char32_t>>;


__BASE_STRING_NAMESPACE_END


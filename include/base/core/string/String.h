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
		NODISCARD CONSTEXPR_CXX20 operator BasicString<char16_t, >() const noexcept { return { begin(), end() }; }
		NODISCARD CONSTEXPR_CXX20 sizetype size() const noexcept { return chars[1] ? 2 : 1; }
		NODISCARD CONSTEXPR_CXX20 const char16_t* begin() const noexcept { return chars; }
		NODISCARD CONSTEXPR_CXX20 const char16_t* end() const noexcept { return begin() + size(); }
	};
	return requiresSurrogates(c) ? R{ {Char::highSurrogate(c),
									  Char::lowSurrogate(c)} } :
		R{ {char16_t(c), u'\0'} };
}

__BASE_STRING_NAMESPACE_END


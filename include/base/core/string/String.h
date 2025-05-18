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

template <
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _Storage_				= BasicStringStorage<char, _SimdOptimization_,
		stringStorage::OptimizationSmallAndLarge>>
using String		= BasicString<
	char, std::char_traits<char>,
	std::allocator<char>, _SimdOptimization_, _Storage_>;


#if __cpp_lib_char8_t

template <
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _Storage_				= BasicStringStorage<char8_t, _SimdOptimization_,
		stringStorage::OptimizationSmallAndLarge>>
using Utf8String	= BasicString<
	char8_t, std::char_traits<char8_t>,
	std::allocator<char8_t>, _SimdOptimization_, _Storage_>;

#endif

template <
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _Storage_				= BasicStringStorage<char16_t, _SimdOptimization_,
		stringStorage::OptimizationSmallAndLarge>>
using Utf16String	= BasicString<
	char16_t, std::char_traits<char16_t>,
	std::allocator<char16_t>, _SimdOptimization_, _Storage_>;

template <
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _Storage_				= BasicStringStorage<wchar_t, _SimdOptimization_,
		stringStorage::OptimizationSmallAndLarge>>
using WString		= BasicString<
	wchar_t, std::char_traits<wchar_t>,
	std::allocator<wchar_t>, _SimdOptimization_, _Storage_>;

template <
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _Storage_				= BasicStringStorage<char32_t, _SimdOptimization_,
		stringStorage::OptimizationSmallAndLarge>>
using Utf32String	= BasicString<
	char32_t, std::char_traits<char32_t>,
	std::allocator<char32_t>, _SimdOptimization_, _Storage_>;

__BASE_STRING_NAMESPACE_END

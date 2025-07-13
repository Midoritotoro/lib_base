#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

#include <src/core/string/CharTypeCompatibility.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS int __CDECL __base_strcmp(
	const char* firstString, 
	const char* secondString) noexcept;

DECLARE_NOALIAS int __CDECL __base_wcscmp(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept;

DECLARE_NOALIAS int __CDECL __base_c32cmp(
	const char32_t* firstString,
	const char32_t* secondString) noexcept;

template <
	typename _Char_,
	typename = std::enable_if_t<string::IsCompatibleCharType<_Char_>::value>>
DECLARE_NOALIAS int __CDECL __base_any_strcmp(
	const _Char_* firstString,
	const _Char_* secondString) noexcept 
{
	if constexpr (IsAnyOf_v<_Char_, char, uchar, char8_t>)
		return __base_strcmp(
			reinterpret_cast<const char*>(firstString), 
			reinterpret_cast<const char32_t*>(secondString));

	else if constexpr (std::is_same_v<_Char_, char16_t> || (sizeof(wchar_t) == 2 && std::is_same_v<_Char_, wchar_t>)) // Windows 
		return __base_wcscmp(
			reinterpret_cast<const wchar_t*>(firstString), 
			reinterpret_cast<const char32_t*>(secondString));

	else if constexpr (std::is_same_v<_Char_, char32_t> || (sizeof(wchar_t) == 4 && std::is_same_v<_Char_, wchar_t>)) // Linux and etc.
		return __base_c32cmp(
			reinterpret_cast<const char32_t*>(firstString), 
			reinterpret_cast<const char32_t*>(secondString));

	AssertUnreachable();
	return -1;
}

__BASE_STRING_NAMESPACE_END

#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

#include <src/core/string/CharTypeCompatibility.h>

__BASE_STRING_NAMESPACE_BEGIN

extern "C" {
	DECLARE_NOALIAS std::size_t	__CDECL __base_strcmp(const char* string) noexcept;
	DECLARE_NOALIAS std::size_t __CDECL __base_wcscmp(const wchar_t* string) noexcept;
	DECLARE_NOALIAS std::size_t __CDECL __base_c32cmp(const char32_t* string) noexcept;
}

template <
	typename _Char_,
	typename = std::enable_if_t<string::IsCompatibleCharType<_Char_>::value>>
	DECLARE_NOALIAS std::size_t __CDECL __base_any_strcmp(const _Char_* string) noexcept {
	if constexpr (IsAnyOf_v<_Char_, char, uchar, char8_t>)
		return __base_strcmp(reinterpret_cast<const char*>(string));
	else if constexpr (std::is_same_v<_Char_, char16_t> || (sizeof(wchar_t) == 2 && std::is_same_v<_Char_, wchar_t>)) // Windows 
		return __base_wcscmp(reinterpret_cast<const wchar_t*>(string));
	else if constexpr (std::is_same_v<_Char_, char32_t> || (sizeof(wchar_t) == 4 && std::is_same_v<_Char_, wchar_t>)) // Linux and etc.
		return __base_c32cmp(reinterpret_cast<const char32_t*>(string));


	AssertUnreachable();
	return -1;
}

__BASE_STRING_NAMESPACE_END

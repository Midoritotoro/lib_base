#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS const char* CDECL __base_strstrn(
	const char*			mainString,
	const std::size_t	mainLength,
	const char*			subString,
	const std::size_t	subLength) noexcept;

DECLARE_NOALIAS const wchar_t* CDECL __base_wcsstrn(
	const wchar_t*		mainString,
	const std::size_t	mainLength,
	const wchar_t*		subString,
	const std::size_t	subLength) noexcept;

DECLARE_NOALIAS const wchar_t* CDECL __base_c32strn(
	const char32_t*		mainString,
	const std::size_t	mainLength,
	const char32_t*		subString,
	const std::size_t	subLength) noexcept;

__BASE_STRING_NAMESPACE_END

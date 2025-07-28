#pragma once 

#include <src/core/string/crt/cs/BaseStrstr.h>
#include <src/core/string/crt/c32/BaseC32str.h>
#include <src/core/string/crt/wcs/BaseWcsstr.h>

#include <base/core/utility/TypeTraits.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strstr(
	const char* string,
	const char* needle) noexcept
{
	return __base_strstr(string, needle);
}

DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strstr(
	const wchar_t* string,
	const wchar_t* needle) noexcept 
{
	return __base_wcsstr(string, needle);
}

DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strstr(
	const char32_t* string,
	const char32_t* needle) noexcept 
{
	return __base_c32str(string, needle);
}

__BASE_STRING_NAMESPACE_END
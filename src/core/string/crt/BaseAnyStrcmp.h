#pragma once 

#include <src/core/string/crt/cs/BaseStrcmp.h>
#include <src/core/string/crt/c32/BaseC32cmp.h>
#include <src/core/string/crt/wcs/BaseWcscmp.h>

#include <base/core/utility/TypeTraits.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS NODISCARD inline int __base_any_strcmp(
	const char* firstString,
	const char* secondString) noexcept
{
	return __base_strcmp(firstString, secondString);
}

DECLARE_NOALIAS NODISCARD inline int __base_any_strcmp(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	return __base_wcscmp(firstString, secondString);
}

DECLARE_NOALIAS NODISCARD inline int __base_any_strcmp(
	const char32_t* firstString,
	const char32_t* secondString) noexcept
{
	return __base_c32cmp(firstString, secondString);
}

__BASE_STRING_NAMESPACE_END

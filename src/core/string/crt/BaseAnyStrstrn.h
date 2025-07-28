#pragma once 

#include <src/core/string/crt/cs/BaseStrstrn.h>
#include <src/core/string/crt/c32/BaseC32strn.h>
#include <src/core/string/crt/wcs/BaseWcsstrn.h>

#include <base/core/utility/TypeTraits.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strstrn(
	const char* string,
	sizetype	stringLength,
	const char* needle,
	sizetype	needleLength) noexcept
{
	return __base_strstrn(string, stringLength, needle, needleLength);
}

DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strstrn(
	const whar_t*	string,
	sizetype		stringLength,
	const wchar_t*	needle,
	sizetype		needleLength) noexcept
{
	return __base_wcsstrn(string, stringLength, needle, needleLength);
}

DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strstrn(
	const char32_t* string,
	sizetype		stringLength,
	const char32_t* needle,
	sizetype		needleLength) noexcept
{
	return __base_c32strn(string, stringLength, needle, needleLength);
}

__BASE_STRING_NAMESPACE_END
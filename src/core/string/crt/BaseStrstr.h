#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

extern "C" {
	DECLARE_NOALIAS char* CDECL __base_strstr(
		const char* mainString,
		const char* subString)i noexcept;
	DECLARE_NOALIAS wchar_t* CDECL __base_wcsstr(
		const wchar_t* mainString,
		const wchar_t* subString) noexcept;
}

__BASE_STRING_NAMESPACE_END



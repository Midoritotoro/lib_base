#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS const wchar_t* CDECL __base_wcsstrn(
	const wchar_t*		mainString,
	const sizetype		mainLength,
	const wchar_t*		subString,
	const sizetype		subLength) noexcept;

__BASE_STRING_NAMESPACE_END

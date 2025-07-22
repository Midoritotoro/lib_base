#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS const char32_t* CDECL __base_c32strn(
	const char32_t*		mainString,
	const sizetype		mainLength,
	const char32_t*		subString,
	const sizetype		subLength) noexcept;

__BASE_STRING_NAMESPACE_END

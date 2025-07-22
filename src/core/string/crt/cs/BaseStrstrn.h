#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS const char* CDECL __base_strstrn(
	const char*			mainString,
	const sizetype		mainLength,
	const char*			subString,
	const sizetype		subLength) noexcept;

__BASE_STRING_NAMESPACE_END

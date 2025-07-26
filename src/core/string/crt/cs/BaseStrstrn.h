#pragma once 

#include <base/core/Types.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS const char* __base_strstrn(
	const char*			mainString,
	const sizetype		mainLength,
	const char*			subString,
	const sizetype		subLength) noexcept;

__BASE_STRING_NAMESPACE_END

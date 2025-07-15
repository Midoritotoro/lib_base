#pragma once

#include <base/core/Types.h>


__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS int __CDECL __base_wcscmp(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept;

__BASE_STRING_NAMESPACE_END

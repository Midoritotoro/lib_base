#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

#include <src/core/string/CharTypeCompatibility.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS int __CDECL __base_strcmp(
	const char* firstString, 
	const char* secondString) noexcept;

__BASE_STRING_NAMESPACE_END

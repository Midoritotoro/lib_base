#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

extern "C" {
	DECLARE_NOALIAS std::size_t	CDECL __base_strlen(const char* string) noexcept;
	DECLARE_NOALIAS std::size_t CDECL __base_wcslen(const wchar_t* string) noexcept;
}

__BASE_STRING_NAMESPACE_END

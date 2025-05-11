#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

extern "C" {
	DECLARE_NOALIAS std::size_t	CDECL __base_strlen(const char* str) noexcept;
	DECLARE_NOALIAS std::size_t CDECL __base_wcslen(const wchar_t* str) noexcept;
}

__BASE_STRING_NAMESPACE_END

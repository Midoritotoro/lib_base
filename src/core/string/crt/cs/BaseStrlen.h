#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

#include <src/core/string/CharTypeCompatibility.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t	__CDECL __base_strlen(const char* string) noexcept;

__BASE_STRING_NAMESPACE_END

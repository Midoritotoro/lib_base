#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS NODISCARD const char* __base_strstr(
    const char* string,
    const char* needle);


__BASE_STRING_NAMESPACE_END



#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS NODISCARD const char32_t* __base_c32str(
    const char32_t* string,
    const char32_t* needle);

__BASE_STRING_NAMESPACE_END



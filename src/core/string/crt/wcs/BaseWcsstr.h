#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS NODISCARD const wchar_t* __base_wcsstr(
    const wchar_t* string,
    const wchar_t* needle);

__BASE_STRING_NAMESPACE_END


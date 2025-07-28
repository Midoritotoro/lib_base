#pragma once 

#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/c32/BaseC32len.h>
#include <src/core/string/crt/wcs/BaseWcslen.h>

#include <base/core/utility/TypeTraits.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strlen(const char* string) noexcept {
	return __base_strlen(string);
}

DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strlen(const wchar_t* string) noexcept {
	return __base_wcslen(string);
}

DECLARE_NOALIAS NODISCARD inline sizetype __base_any_strlen(const char32_t* string) noexcept {
	return __base_c32len(string);
}

__BASE_STRING_NAMESPACE_END
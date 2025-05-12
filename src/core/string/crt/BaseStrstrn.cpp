#include <base/core/arch/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS char* CDECL __base_strstrn(
	const char* mainString,
	const std::size_t	mainNullTerminatedLength,
	const char* subString,
	const std::size_t	subNullTerminatedLength) noexcept
{

}

DECLARE_NOALIAS wchar_t* CDECL __base_wcsstrn(
	const wchar_t* mainString,
	const std::size_t	mainNullTerminatedLength,
	const wchar_t* subString,
	const std::size_t	subNullTerminatedLength) noexcept
{

}

__BASE_STRING_NAMESPACE_END

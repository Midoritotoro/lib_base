#include <src/core/string/crt/BaseStrlen.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS char* CDECL __base_strstr(
	const char* mainString,
	const char* subString) noexcept
{

}

DECLARE_NOALIAS wchar_t* CDECL __base_wcsstr(
	const wchar_t* mainString,
	const wchar_t* subString) noexcept
{
	
}

__BASE_STRING_NAMESPACE_END

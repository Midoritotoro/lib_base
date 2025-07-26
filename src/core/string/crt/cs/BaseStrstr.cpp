#include <src/core/string/crt/cs/BaseStrlen.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/string/crt/cs/BaseStrstrn.h>
#include <src/core/string/crt/cs/BaseStrlen.h>

__BASE_STRING_NAMESPACE_BEGIN



DECLARE_NOALIAS NODISCARD const char* CDECL __base_strstr(
	const char* string,
	const char* needle) noexcept
{
	return __base_strstrn(string, __base_strlen(string), needle, __base_strlen(needle));
}


__BASE_STRING_NAMESPACE_END

#include <src/core/string/crt/cs/BaseStrlen.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/string/crt/VectorizedSafeStringAlgorithm.h>
#include <src/core/memory/crt/FixedMemcmp.h>

__BASE_STRING_NAMESPACE_BEGIN



DECLARE_NOALIAS NODISCARD const char* CDECL __base_strstr(
	const char* string,
	const char* needle) noexcept
{
	/*if (ProcessorFeatures::AVX512F())
		return __base_strstrnAvx512F(
			string, __base_strlen(string), 
			needle, __base_strlen(needle));

	else if (ProcessorFeatures::AVX2())
		return __base_strstrnAvx2(
			string, __base_strlen(string),
			needle, __base_strlen(needle));

	else if (ProcessorFeatures::SSE2())
		return __base_strstrnSse2(
			string, __base_strlen(string),
			needle, __base_strlen(needle));

	return __base_strstrnScalar(
		string, __base_strlen(string),
		needle, __base_strlen(needle));*/
    return "";
}


__BASE_STRING_NAMESPACE_END

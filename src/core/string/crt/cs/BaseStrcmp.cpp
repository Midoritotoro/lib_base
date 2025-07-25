#include <src/core/string/crt/cs/BaseStrcmp.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/string/crt/cs/internal/BaseStrcmpInternal.h>

always_inline NODISCARD bool IsXmmZero(__m128i xmmRegister) noexcept {
	return _mm_movemask_epi8(_mm_cmpeq_epi64(xmmRegister, _mm_setzero_si128())) == 0xFFFFFFFFFFFFFFFF;
}

always_inline NODISCARD bool IsYmmZero(__m256i ymmRegister) noexcept {
	return _mm256_cmpeq_epi64_mask(ymmRegister, _mm256_setzero_si256()) == 0xFFFFFFFFFFFFFFFF;
}

always_inline NODISCARD bool IsZmmZero(__m512i zmmRegister) noexcept {
	return _mm512_cmpeq_epi64_mask(zmmRegister, _mm512_setzero_si512()) == 0xFFFFFFFFFFFFFFFF;
}

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS int	__CDECL __base_strcmp(
	const char* firstString,
	const char* secondString) noexcept 
{
	if (arch::ProcessorFeatures::AVX512BW())
		return __baseFeatureAwareStrcmp<arch::CpuFeature::AVX512BW>(firstString, secondString);
	else if (arch::ProcessorFeatures::AVX2())
		return __baseFeatureAwareStrcmp<arch::CpuFeature::AVX2>(firstString, secondString);
	else if (arch::ProcessorFeatures::SSE2())
		return __baseFeatureAwareStrcmp<arch::CpuFeature::SSE2>(firstString, secondString);

	return __baseFeatureAwareStrcmp<arch::CpuFeature::None>(firstString, secondString);
}

__BASE_STRING_NAMESPACE_END

#include <src/core/string/crt/cs/internal/BaseStrstrInternal.h>

__BASE_STRING_NAMESPACE_BEGIN

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::AVX512F>(
	const char* mainString,
	const char* subString) noexcept
{

}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::AVX2>(
	const char* mainString,
	const char* subString) noexcept
{

}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::SSE2>(
	const char* mainString,
	const char* subString) noexcept
{

}

__BASE_STRING_NAMESPACE_END
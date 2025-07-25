#pragma once 

#include <base/core/arch/CpuFeature.h>


__BASE_STRING_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
	BASE_ECHO(
		template <arch::CpuFeature feature>
		DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen(const char* string) noexcept
	),
	feature,
	"base::string",
	arch::CpuFeature::None, arch::CpuFeature::SSE2, arch::CpuFeature::AVX2, arch::CpuFeature::AVX512BW
)

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::None>(const char* string) noexcept;

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX512BW>(const char* string) noexcept;

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX2>(const char* string) noexcept;

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::SSE2>(const char* string) noexcept;

__BASE_STRING_NAMESPACE_END


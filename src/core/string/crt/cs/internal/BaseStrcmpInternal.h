#pragma once 

#include <base/core/arch/Platform.h>

__BASE_STRING_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
	BASE_ECHO(
		template <arch::CpuFeature feature>
		DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp(
			const char* firstString,
			const char* secondString) noexcept
	),
	feature, 
	"base::string",
	arch::CpuFeature::None, arch::CpuFeature::SSE2, arch::CpuFeature::AVX2, arch::CpuFeature::AVX512BW
)

template <>
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::None>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::SSE2>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::AVX2>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::AVX512BW>(
	const char* firstString,
	const char* secondString) noexcept;



__BASE_STRING_NAMESPACE_END

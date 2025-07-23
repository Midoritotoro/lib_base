#pragma once 

#include <base/core/arch/Platform.h>

__BASE_STRING_NAMESPACE_BEGIN

template <CpuFeature feature>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<CpuFeature::None>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<CpuFeature::SSE2>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<CpuFeature::AVX>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<CpuFeature::AVX512F>(
	const char* firstString,
	const char* secondString) noexcept;



__BASE_STRING_NAMESPACE_END

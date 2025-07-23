#pragma once 

#include <base/core/arch/Platform.h>

__BASE_STRING_NAMESPACE_BEGIN


template <arch::CpuFeature feature>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::None>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::SSE2>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::AVX>(
	const char* firstString,
	const char* secondString) noexcept;

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::AVX512F>(
	const char* firstString,
	const char* secondString) noexcept;



__BASE_STRING_NAMESPACE_END

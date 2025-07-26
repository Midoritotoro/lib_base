#pragma once 

#include <base/core/arch/Platform.h>

__BASE_STRING_NAMESPACE_BEGIN

//BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
//	BASE_ECHO(
//		template <arch::CpuFeature feature>
//		DECLARE_NOALIAS NODISCARD const char* BaseFeatureAwareStrstr(
//			const char* string,
//			const char*	needle) noexcept
//	),
//	feature,
//	"base::string",
//	arch::CpuFeature::AVX512F, arch::CpuFeature::AVX2, arch::CpuFeature::SSE2
//)
//
//template <>
//DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::AVX512F>(
//	const char*		mainString,
//	const char*		subString) noexcept;
//
//template <>
//DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::AVX2>(
//	const char*		mainString,
//	const char*		subString) noexcept;
//
//template <>
//DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::SSE2>(
//	const char*		mainString,
//	const char*		subString) noexcept;

__BASE_STRING_NAMESPACE_END

#pragma once 

#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
	BASE_ECHO(
		template <arch::CpuFeature feature>
		DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn(
			const char*		mainString,
			const sizetype	mainLength,
			const char*		subString,
			const sizetype	subLength) noexcept
	),
	feature,
	"base::string",
	arch::CpuFeature::None, arch::CpuFeature::AVX512F, arch::CpuFeature::AVX2, arch::CpuFeature::SSE2
)

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::None>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::AVX512F>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::AVX2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::SSE2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

__BASE_STRING_NAMESPACE_END

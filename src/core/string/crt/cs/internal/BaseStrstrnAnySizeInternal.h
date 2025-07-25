#pragma once 

#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

template <arch::CpuFeature feature>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnAnySize(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::AVX512F>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::AVX2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::SSE2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept;

__BASE_STRING_NAMESPACE_END

#pragma once 

#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	arch::CpuFeature feature,
	sizetype needleLength,
	typename _MemCmpLike_>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnMemcmp(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	_MemCmpLike_	memcmpLike) noexcept;

template <
	sizetype needleLength,
	typename _MemCmpLike_>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX512BW>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	_MemCmpLike_	memcmpLike) noexcept;

template <
	sizetype needleLength,
	typename _MemCmpLike_>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	_MemCmpLike_	memcmpLike) noexcept;

template <
	sizetype needleLength,
	typename _MemCmpLike_>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	_MemCmpLike_	memcmpLike) noexcept;

__BASE_STRING_NAMESPACE_END

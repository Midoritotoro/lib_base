#pragma once 

#include <base/core/arch/CpuFeature.h>


__BASE_STRING_NAMESPACE_BEGIN

template <arch::CpuFeature feature>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen(const char* string) noexcept;

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::None>(const char* string) noexcept;

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX512BW>(const char* string) noexcept;

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX2>(const char* string) noexcept;

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::SSE2>(const char* string) noexcept;

__BASE_STRING_NAMESPACE_END


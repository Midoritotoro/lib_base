#pragma once 

#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

struct ZeroComparison {
	uint64	mask = 0;
	uint64	trailingZeros = 0;
};

template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(const void* start) noexcept;

template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m128i vector) noexcept;
template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m256i vector) noexcept;
template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m512i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::SSE2, 1>(__m128i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::SSE2, 2>(__m128i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::SSE2, 4>(__m128i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX2, 1>(__m256i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX2, 2>(__m256i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX2, 4>(__m256i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX512BW, 1>(__m512i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX512BW, 2>(__m512i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX512BW, 4>(__m512i vector) noexcept;

__BASE_STRING_NAMESPACE_END

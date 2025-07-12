#pragma once 

#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

struct ZeroComparison {
	uint64	mask = 0;
	uint64	trailingZeros = 0;
};

template <CpuFeature feature, uint64 byteCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(const void* start) noexcept;

template <CpuFeature feature, uint64 byteCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m128i vector) noexcept;
template <CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m256i vector) noexcept;
template <CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m512i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::SSE, 1>(__m128i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::SSE, 2>(__m128i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::SSE, 4>(__m128i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX, 1>(__m256i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX, 2>(__m256i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX, 4>(__m256i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX512, 1>(__m512i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX512, 2>(__m512i vector) noexcept;

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX512, 4>(__m512i vector) noexcept;

__BASE_STRING_NAMESPACE_END

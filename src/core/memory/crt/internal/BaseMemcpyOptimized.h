#pragma once 

#include <base/core/arch/CpuFeature.h>


__BASE_MEMORY_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
	BASE_ECHO(
		template <arch::CpuFeature feature>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
	),
	feature,
	"base::string",
	arch::CpuFeature::None, arch::CpuFeature::SSE, arch::CpuFeature::AVX, arch::CpuFeature::AVX512F
)

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::None>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::SSE>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::AVX>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::AVX512F>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;


__BASE_MEMORY_NAMESPACE_END

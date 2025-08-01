#include <src/core/memory/crt/internal/BaseMemcpyOptimized.h>

#include <src/core/memory/crt/internal/BaseMemcpyImplementationInternal.h>


__BASE_MEMORY_NAMESPACE_BEGIN

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::None>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	return nullptr;
}

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::SSE>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	return nullptr;
}

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::AVX>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	return nullptr;
}

template <>
DECLARE_NOALIAS NODISCARD void* BaseMemcpyOptimized<arch::CpuFeature::AVX512F>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	return nullptr;
}

__BASE_MEMORY_NAMESPACE_END

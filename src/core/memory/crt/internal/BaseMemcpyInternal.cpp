#include <src/core/memory/crt/internal/BaseMemcpyInternal.h>
#include <src/core/memory/crt/internal/BaseMemcpyImplementationInternal.h>

__BASE_MEMORY_NAMESPACE_BEGIN


template <>
DECLARE_NOALIAS NODISCARD void* BaseFeatureAwareMemcpy<arch::CpuFeature::None>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	using _Implementation_ = BaseMemcpyImplementationInternal<arch::CpuFeature::None>;

#if defined(PROCESSOR_X86_64)
	if (size < 16)
		return _Implementation_::Memcpy<8, false>(destination, source, size);
	else 
#endif
	if (size < 8)
		return _Implementation_::Memcpy<4, false>(destination, source, size);
	else if (size < 4)
		return _Implementation_::Memcpy<2, false>(destination, source, size);

	return _Implementation_::Memcpy<1, false>(destination, source, size);
}

template <>
DECLARE_NOALIAS NODISCARD void* BaseFeatureAwareMemcpy<arch::CpuFeature::SSE>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	using _Implementation_ = BaseMemcpyImplementationInternal<arch::CpuFeature::SSE>;
}

template <>
DECLARE_NOALIAS NODISCARD void* BaseFeatureAwareMemcpy<arch::CpuFeature::AVX>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	using _Implementation_ = BaseMemcpyImplementationInternal<arch::CpuFeature::AVX>;
}

template <>
DECLARE_NOALIAS NODISCARD void* BaseFeatureAwareMemcpy<arch::CpuFeature::AVX512F>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept
{
	using _Implementation_ = BaseMemcpyImplementationInternal<arch::CpuFeature::AVX512F>;
}

__BASE_MEMORY_NAMESPACE_END

#pragma once 

#include <base/core/arch/CpuFeature.h>


__BASE_MEMORY_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
	BASE_ECHO(
		template <arch::CpuFeature feature>
		base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy(
			void*               destination,
			const void* const   source,
			sizetype            size) noexcept
	),
	feature,
	"base::string",
	arch::CpuFeature::None, arch::CpuFeature::SSE, arch::CpuFeature::AVX, arch::CpuFeature::AVX512F
)

template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::None>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;

template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::SSE>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;

template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::AVX>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;

template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::AVX512F>(
	void*				destination,
	const void* const   source,
	sizetype            size) noexcept;


__BASE_MEMORY_NAMESPACE_END

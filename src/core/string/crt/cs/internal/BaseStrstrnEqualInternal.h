#pragma once

#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
	BASE_ECHO(
		template <
			arch::CpuFeature feature,
			sizetype needleLength>
		DECLARE_NOALIAS NODISCARD always_inline const char* BaseFeatureAwareStrstrnEqual(
			const char* string,
			sizetype    stringLength,
			const char* needle) noexcept
	),
	feature,
	"base::string",
	arch::CpuFeature::AVX2
)

template <sizetype needleLength>
DECLARE_NOALIAS NODISCARD always_inline const char* BaseFeatureAwareStrstrnEqual<arch::CpuFeature::AVX2>(
	const char* string,
	sizetype    stringLength,
	const char* needle) noexcept;

__BASE_STRING_NAMESPACE_END

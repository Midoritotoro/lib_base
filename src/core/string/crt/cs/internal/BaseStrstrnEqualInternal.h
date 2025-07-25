#pragma once

#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(
	BASE_ECHO(
		template <
			sizetype needleLength,
			arch::CpuFeature feature>
		DECLARE_NOALIAS NODISCARD const char* BaseFeatureAwareStrstrnEqual(
			const char* string,
			sizetype    stringLength,
			const char* needle) noexcept
	),
	feature,
	"base::string",
	arch::CpuFeature::AVX2
)

template <sizetype needleLength>
DECLARE_NOALIAS NODISCARD const char* BaseFeatureAwareStrstrnEqual<needleLength, arch::CpuFeature::AVX2>(
	const char* string,
	sizetype    stringLength,
	const char* needle) noexcept;

__BASE_STRING_NAMESPACE_END

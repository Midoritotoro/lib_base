#pragma once 

#include <base/core/arch/CpuFeatureTag.h>
#include <src/core/string/StringConversionOptions.h> 

__BASE_STRING_NAMESPACE_BEGIN


template <
	CpuFeature _SimdType_,
	class _NarrowingConversionBehaviour_>
class StringConverterTraitsBase {
public:
	template <
		typename _FromChar_,
		typename _ToChar_>
	// Is data loss possible when converting from _FromChar_ to _ToChar_
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {
		return (MaximumIntegralLimit<_FromChar_>() < MaximumIntegralLimit<_ToChar_>());
	}
};


__BASE_STRING_NAMESPACE_END

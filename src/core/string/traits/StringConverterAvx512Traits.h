#pragma once 

#include <base/core/arch/CpuFeatureTag.h>
#include <src/core/string/StringConversionOptions.h> 

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_ = DefaultReplacementConversionMode>
class StringConverterTraits {
public:
	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsSupportedCharType<_FromChar_>&&
			IsSupportedCharType<_ToChar_>>>
	void convert() {
		
	}
};

__BASE_STRING_NAMESPACE_END
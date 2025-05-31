#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<
	CpuFeatureTag<CpuFeature::None>, 
	_NarrowingConversionBehaviour_>:
		public StringConverterTraitsBase<
			CpuFeatureTag<CpuFeature::None>,
			_NarrowingConversionBehaviour_> 
{
public:
	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		StringConversionParameters<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::None>>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
};

__BASE_STRING_NAMESPACE_END
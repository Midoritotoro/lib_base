#pragma once 

#include <base/core/arch/CpuFeatureTag.h>
#include <src/core/string/StringConversionOptions.h> 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<CpuFeatureTag<CpuFeature::None>, _NarrowingConversionBehaviour_> {
public:
	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value&&
			IsCompatibleCharType<_ToChar_>::value>>
	// Is data loss possible when converting from _FromChar_ to _ToChar_
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {
		return (MaximumIntegralLimit<_FromChar_>() < MaximumIntegralLimit<_ToChar_>());
	}

	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value	&&
			IsCompatibleCharType<_ToChar_>::value>>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const _FromChar_* const string,
		const size_t			stringLength,
		_ToChar_*				outputString) noexcept
	{
		
	}
};

__BASE_STRING_NAMESPACE_END
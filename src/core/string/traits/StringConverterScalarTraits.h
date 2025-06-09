#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<
	CpuFeature::None, 
	_NarrowingConversionBehaviour_>:
		public StringConverterTraitsBase<
			CpuFeature::None,
			_NarrowingConversionBehaviour_> 
{
public:
	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	// Is data loss possible when converting from _FromChar_ to _ToChar_
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {
		return StringConverterTraitsBase<
			CpuFeature::None,
			_NarrowingConversionBehaviour_>::template maybeNarrowingConversion<_FromChar_, _ToChar_>();
	}

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		StringConversionParameters<_FromChar_, _ToChar_, CpuFeature::None>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
};

__BASE_STRING_NAMESPACE_END
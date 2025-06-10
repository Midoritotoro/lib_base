#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<
	CpuFeature::AVX, 
	_NarrowingConversionBehaviour_>:
		public StringConverterTraitsBase<
			CpuFeature::AVX,
			_NarrowingConversionBehaviour_> 
{
	using WCharAvxMaskIntegerType = std::conditional_t<sizeof(wchar_t) == 2,
		uint16,
		std::conditional_t<sizeof(wchar_t) == 4, uint8, void>>;

	template <size_t conversionToLimit>
	static constexpr NODISCARD base_vec512i_t wcharAvxLessThenCompareVector() noexcept {
		if constexpr (sizeof(wchar_t) == 2)
			return base_constexpr_mm256_set1_epu16(
				static_cast<uint16>(conversionToLimit));
		else if constexpr (sizeof(wchar_t) == 4)
			return base_constexpr_mm256_set1_epu32(
				static_cast<uint32>(conversionToLimit));
	}

	using FallbackTraits = StringConverterTraits<
		CpuFeature::SSE,
		_NarrowingConversionBehaviour_>;
public:
	template <typename _FromChar_>
	// A vector for finding characters of type _FromChar_ that exceed the _ToChar_ limit
	static constexpr NODISCARD base_vec256i_t narrowingLimitVector() noexcept {
		if constexpr (std::is_same_v<_FromChar_, char>)
			return base_constexpr_mm256_set1_epi8(MaximumIntegralLimit<char>());
#if __cpp_lib_char8_t
		else if constexpr (std::is_same_v<_FromChar_, char8_t>)
			return base_constexpr_mm256_set1_epu8(MaximumIntegralLimit<char8_t>());
#endif
		else if constexpr (std::is_same_v<_FromChar_, char16_t>)
			return base_constexpr_mm256_set1_epu16(MaximumIntegralLimit<char16_t>());

		else if constexpr (std::is_same_v<_FromChar_, char32_t>)
			return base_constexpr_mm256_set1_epu32(MaximumIntegralLimit<char32_t>());

		else if constexpr (std::is_same_v<_FromChar_, wchar_t>)
			if constexpr (sizeof(wchar_t) == 2)
				return base_constexpr_mm256_set1_epu16(MaximumIntegralLimit<wchar_t>());
			else if constexpr (sizeof(wchar_t) == 4)
				return base_constexpr_mm256_set1_epu32(MaximumIntegralLimit<wchar_t>());

		return {};
	}

	template <typename _ToChar_>
	// A vector for replacing characters in a narrowing conversion
	static constexpr NODISCARD base_vec256i_t replacementVector() noexcept {
		if constexpr (std::is_same_v<_ToChar_, char>)
			return base_constexpr_mm256_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
#if __cpp_lib_char8_t
		else if constexpr (std::is_same_v<_ToChar_, char8_t>)
			return base_constexpr_mm256_set1_epu8(_NarrowingConversionBehaviour_::replacementCharacter);
#endif
		else if constexpr (std::is_same_v<_ToChar_, char16_t>)
			return base_constexpr_mm256_set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter);

		else if constexpr (std::is_same_v<_ToChar_, char32_t>)
			return base_constexpr_mm256_set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter);

		else if constexpr (std::is_same_v<_ToChar_, wchar_t>)
			if constexpr (sizeof(wchar_t) == 2)
				return base_constexpr_mm256_set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter);
			else if constexpr (sizeof(wchar_t) == 4)
				return base_constexpr_mm256_set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter);

		return {};
	}

	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	// Is data loss possible when converting from _FromChar_ to _ToChar_
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {
		return StringConverterTraitsBase<
			CpuFeature::SSE,
			_NarrowingConversionBehaviour_>::template maybeNarrowingConversion<_FromChar_, _ToChar_>();
	}

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		StringConversionParameters<_FromChar_, _ToChar_, CpuFeature::AVX>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
};

__BASE_STRING_NAMESPACE_END
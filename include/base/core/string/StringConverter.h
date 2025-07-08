#pragma once 

#include <base/core/arch/ProcessorFeatures.h>
#include <src/core/string/traits/StringConverterAvx512Traits.h>

#include <src/core/string/traits/StringConverterAvxTraits.h>
#include <src/core/string/traits/StringConverterSseTraits.h>

#include <src/core/string/traits/StringConverterScalarTraits.h>

#include <base/core/memory/MemoryAllocation.h>
#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_ = DefaultReplacementConversionMode>
class StringConverter {
	template <
		typename _FromChar_,
		typename _ToChar_>
	// Is data loss possible when converting from _FromChar_ to _ToChar_
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {
		return (MaximumIntegralLimit<_FromChar_>() < MaximumIntegralLimit<_ToChar_>());
	}
public:
	// Allocates (length * sizeof(_ToChar_)) bytes of memory for conversion from string
	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const _FromChar_* const string,
		const size_t			length)
	{
		if (ProcessorFeatures::AVX512F())
			return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::AVX512, _NarrowingConversionBehaviour_>>(
				string, length, nullptr);

		else if (ProcessorFeatures::AVX())
			return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::AVX, _NarrowingConversionBehaviour_>>(
				string, length, nullptr);

		else if (ProcessorFeatures::SSE())
			return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::SSE, _NarrowingConversionBehaviour_>>(
				string, length, nullptr);

		return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::None, _NarrowingConversionBehaviour_>>(
			string, length, nullptr);
	}

	// Converts by writing to output->data() (without memory allocation)
	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	static void convertStringStore(
		const _FromChar_* const				string,
		const size_t						length,
		StringConversionResult<_ToChar_>*	output)
	{
		StringConversionResult<_ToChar_> temp;

		if (ProcessorFeatures::AVX512F())
			temp = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::AVX512, _NarrowingConversionBehaviour_>>(
				string, length, output->data());

		else if (ProcessorFeatures::AVX())
			temp = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::AVX, _NarrowingConversionBehaviour_>>(
				string, length, output->data());

		else if (ProcessorFeatures::SSE())
			temp = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::SSE, _NarrowingConversionBehaviour_>>(
				string, length, output->data());

		else
			temp = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<CpuFeature::None, _NarrowingConversionBehaviour_>>(
				string, length, output->data());

		*output = temp;
	}
private:
	template <
		class _FromChar_,
		class _ToChar_,
		class _StringConverterTraits_>
	static NODISCARD StringConversionResult<_ToChar_> convertStringImplementation(
			const _FromChar_* const string,
			const size_t			stringLength,	
			_ToChar_*				outputString)
	{
		using _Parameters_ = StringConversionParameters<_FromChar_, _ToChar_, _StringConverterTraits_::cpuFeature>;

		if (string == nullptr || stringLength == 0)
			return {};

		const auto fromStringSizeInBytes = size_t(stringLength * sizeof(_FromChar_));

		if (outputString == nullptr)
			outputString = static_cast<_ToChar_*>(memory::AllocateAligned(stringLength * sizeof(_ToChar_), 64));

		StringConversionResult<_ToChar_> conversionResult;

		if constexpr (maybeNarrowingConversion<_FromChar_, _ToChar_>() && _StringConverterTraits_::cpuFeature != CpuFeature::None) {
			constexpr auto toLimit = MaximumIntegralLimit<_ToChar_>();

			static_assert(
				_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
				"base::core::string::StringConverter::convertStringImplementation"
				"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

			_Parameters_ parameters;

			constexpr const auto replacementVector = _StringConverterTraits_::template replacementVector<_ToChar_>();
			constexpr const auto narrowingLimitVector = _StringConverterTraits_::template narrowingLimitVector<_FromChar_>();

			if constexpr (is_xmm_simd_feature_v<_StringConverterTraits_::cpuFeature>)
				parameters = _Parameters_(string, stringLength, outputString, 
					base_vec128i_t_pointer_as_m128i_pointer(&replacementVector), 
					base_vec128i_t_pointer_as_m128i_pointer(&narrowingLimitVector));

			else if constexpr (is_ymm_simd_feature_v<_StringConverterTraits_::cpuFeature>)
				parameters = _Parameters_(string, stringLength, outputString, 
					base_vec256i_t_pointer_as_m256i_pointer(&replacementVector), 
					base_vec256i_t_pointer_as_m256i_pointer(&narrowingLimitVector));

			else if constexpr (is_zmm_simd_feature_v<_StringConverterTraits_::cpuFeature>)
				parameters = _Parameters_(string, stringLength, outputString, 
					base_vec512i_t_pointer_as_m512i_pointer(&replacementVector),
					base_vec512i_t_pointer_as_m512i_pointer(&narrowingLimitVector));
			
			conversionResult = _StringConverterTraits_::template convertString<_FromChar_, _ToChar_>(parameters);
		}
		else {
			conversionResult = _StringConverterTraits_::template convertString<_FromChar_, _ToChar_>(
				StringConversionParameters<_FromChar_, _ToChar_, _StringConverterTraits_::cpuFeature>(
					string, stringLength, outputString)
			);
		}

		return conversionResult;
	}
};

__BASE_STRING_NAMESPACE_END

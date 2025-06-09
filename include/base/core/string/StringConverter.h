#pragma once 

#include <base/core/string/StringConfig.h>

#include <base/core/memory/MemoryAllocation.h>
#include <base/core/utility/IntegralTypesConversions.h>

#include <src/core/string/StringConversionOptions.h>

#include <src/core/utility/simd/SimdConstexprHelpers.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <src/core/string/traits/StringConverterAvx512Traits.h>

#include <src/core/string/traits/StringConverterAvxTraits.h>
#include <src/core/string/traits/StringConverterSseTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_ = DefaultReplacementConversionMode>
class StringConverter {
public:
	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	// Allocates (length * sizeof(_ToChar_)) bytes of memory for conversion from string
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const _FromChar_* const string,
		const size_t			length)
	{
		if (ProcessorFeatures::AVX512F())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::AVX512>(
				string, length, nullptr);

		else if (ProcessorFeatures::AVX())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::AVX>(
				string, length, nullptr);

		else if (ProcessorFeatures::SSE2())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::SSE>(
				string, length, nullptr);

		return convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::None>(
			string, length, nullptr);
	}

	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	// Converts by writing to output->data() (without memory allocation)
	static void convertStringStore(
		const _FromChar_* const				string,
		const size_t						length,
		StringConversionResult<_ToChar_>*	output)
	{
		StringConversionResult<_ToChar_> temp;

		if (ProcessorFeatures::AVX512F())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::AVX512>(
				string, length, output->data());

		else if (ProcessorFeatures::AVX())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::AVX>(
				string, length, output->data());

		else if (ProcessorFeatures::SSE2())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::SSE>(
				string, length, output->data());

		else
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeature::None>(
				string, length, output->data());

		*output = temp;
	}
private:
	template <
		class _FromChar_,
		class _ToChar_,
		CpuFeature _SimdType_>
	static NODISCARD StringConversionResult<_ToChar_> convertStringImplementation(
			const _FromChar_* const string,
			const size_t			stringLength,	
			_ToChar_*				outputString)
	{
		using StringConverterTraitsType = StringConverterTraits<_SimdType_, _NarrowingConversionBehaviour_>;
		using StringConversionParametersType = StringConversionParameters<_FromChar_, _ToChar_, _SimdType_>;

		if (string == nullptr || stringLength == 0)
			return {};

		const auto fromStringSizeInBytes = size_t(stringLength * sizeof(_FromChar_));

		if (outputString == nullptr)
			outputString = static_cast<_ToChar_*>(memory::AllocateAligned(stringLength * sizeof(_ToChar_), 64));

		StringConversionResult<_ToChar_> conversionResult;

		if constexpr (StringConverterTraitsType::template maybeNarrowingConversion<_FromChar_, _ToChar_>()) {
			constexpr auto toLimit = MaximumIntegralLimit<_ToChar_>();

			static_assert(
				_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
				"base::core::string::StringConverter::convertStringImplementation"
				"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

			if constexpr (_SimdType_ == CpuFeature::None) {
				auto conversionParameters = StringConversionParametersType(
					string, stringLength, outputString, nullptr, nullptr);

				conversionResult = StringConverterTraitsType::template convertString<_FromChar_, _ToChar_>(conversionParameters);
			}
			else {
				constexpr auto replacementVector = StringConverterTraitsType::template replacementVector<_ToChar_>();
				constexpr auto narrowingLimitVector = StringConverterTraitsType::template narrowingLimitVector<_FromChar_>();

				auto conversionParameters = StringConversionParametersType(
					string, stringLength, outputString, replacementVector, narrowingLimitVector);

				conversionResult = StringConverterTraitsType::template convertString<_FromChar_, _ToChar_>(conversionParameters);
			}
		}
		else { 
			auto conversionParameters = StringConversionParametersType(
				string, stringLength, outputString);

			conversionResult = StringConverterTraitsType::template convertString<_FromChar_, _ToChar_>(conversionParameters);
		}

		return conversionResult;
	}
};

__BASE_STRING_NAMESPACE_END

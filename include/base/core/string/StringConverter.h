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
	static inline NODISCARD StringConversionResult<_ToChar_> convertString(
		const _FromChar_* const string,
		const size_t			length)
	{
		if (ProcessorFeatures::AVX512F())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX512F>>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::AVX512F>{});

		else if (ProcessorFeatures::AVX())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX>>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::AVX>{});

		else if (ProcessorFeatures::SSE2())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::SSE2>>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::SSE2>{});

		return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::None>>(
			string, length, nullptr, CpuFeatureTag<CpuFeature::None>{});
	}

	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	// Converts by writing to output->data() (without memory allocation)
	static inline void convertStringStore(
		const _FromChar_* const				string,
		const size_t						length,
		StringConversionResult<_ToChar_>*	output)
	{
		StringConversionResult<_ToChar_> temp;

		if (ProcessorFeatures::AVX512F())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX512F>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::AVX512F>{});

		else if (ProcessorFeatures::AVX())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::AVX>{});

		else if (ProcessorFeatures::SSE2())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::SSE2>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::SSE2>{});

		else
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::None>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::None>{});

		*output = temp;
	}
private:
	template <
		class _FromChar_,
		class _ToChar_,
		class _SimdType_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value &&
			IsCompatibleCharType<_ToChar_>::value>>
	static always_inline NODISCARD StringConversionResult<_ToChar_> convertStringImplementation(
		const _FromChar_* const string,
		const size_t			stringLength,
		_ToChar_*				outputString,
		_SimdType_)
	{
		using StringConverterTraitsType = StringConverterTraits<_SimdType_, _NarrowingConversionBehaviour_>;

		if (string == nullptr || stringLength == 0)
			return {};

		const auto fromStringSizeInBytes = size_t(stringLength * sizeof(_FromChar_));

		if (outputString == nullptr)
			outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));

		if constexpr (StringConverterTraitsType::maybeNarrowingConversion<_FromChar_, _ToChar_>()) {
			constexpr auto toLimit = MaximumIntegralLimit<char>();

			static_assert(
				_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
				"base::core::string::StringConverter::convertStringImplementation"
				"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

			constexpr auto replacementVector = StringConverterTraitsType::replacementVector();
			constexpr auto narrowingLimitVector = StringConverterTraitsType::narrowingLimitVector();

			
		}
		else { 

		}
	}
};

__BASE_STRING_NAMESPACE_END

#pragma once 

#include <base/core/arch/ProcessorFeatures.h>
#include <src/core/string/traits/StringConverterAvx512Traits.h>

#include <src/core/string/traits/StringConverterAvxTraits.h>
#include <src/core/string/traits/StringConverterSseTraits.h>

#include <src/core/string/traits/StringConverterScalarTraits.h>

#include <base/core/memory/MemoryAllocation.h>
#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

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
		const size_t			length,
		_ToChar_				replacementCharacter = _ToChar_('?'))
	{
		if (arch::ProcessorFeatures::AVX512F())
			return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::AVX512>>(
				string, length, nullptr, replacementCharacter);

		else if (arch::ProcessorFeatures::AVX())
			return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::AVX>>(
				string, length, nullptr, replacementCharacter);

		else if (arch::ProcessorFeatures::SSE())
			return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::SSE>>(
				string, length, nullptr, replacementCharacter);

		return convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::None>>(
			string, length, nullptr, replacementCharacter);
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
		StringConversionResult<_ToChar_>*	output,
		_ToChar_							replacementCharacter = _ToChar_('?'))
	{
		if (arch::ProcessorFeatures::AVX512F())
			*output = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::AVX512>>(
				string, length, output->data(), replacementCharacter);

		else if (arch::ProcessorFeatures::AVX())
			*output = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::AVX>>(
				string, length, output->data(), replacementCharacter);

		else if (arch::ProcessorFeatures::SSE())
			*output = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::SSE>>(
				string, length, output->data(), replacementCharacter);

		else
			*output = convertStringImplementation<_FromChar_, _ToChar_, StringConverterTraits<arch::CpuFeature::None>>(
				string, length, output->data(), replacementCharacter);
	}
private:
	template <
		class _FromChar_,
		class _ToChar_,
		class _StringConverterTraits_>
	static NODISCARD StringConversionResult<_ToChar_> convertStringImplementation(
		const _FromChar_* const string,
		const size_t			stringLength,	
		_ToChar_*				outputString,
		_ToChar_				replacementCharacter)
	{
		if (string == nullptr || stringLength == 0)
			return {};

		if (outputString == nullptr)
			outputString = static_cast<_ToChar_*>(memory::AllocateAligned(stringLength * sizeof(_ToChar_), 64));

		return _StringConverterTraits_::template convertString<_FromChar_, _ToChar_>(
			StringConversionParameters<_FromChar_, _ToChar_>(
				string, stringLength, outputString
		));
	}
};

__BASE_STRING_NAMESPACE_END

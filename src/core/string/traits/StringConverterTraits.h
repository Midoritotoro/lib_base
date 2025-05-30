#pragma once 

#include <src/core/string/StringConversionOptions.h>
#include <base/core/arch/CpuFeatureTag.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	typename _FromChar_,
	typename _ToChar_,
	typename = std::enable_if_t<
		IsCompatibleCharType<_FromChar_>::value && 
		IsCompatibleCharType<_ToChar_>::value>>
struct ConversionParameters {
	ConversionParameters(
		const _FromChar_*	inputString,
		size_t				inputStringSizeInBytes,
		_ToChar_*			outputString,
		const void*			replacementVectorSimd = nullptr,
		const void*			narrowingLimitVectorSimd = nullptr
	) noexcept:
		inputStringDataStart(inputString),
		stringSizeInBytes(inputStringSizeInBytes),
		outputStringDataStart(outputString),
		replacementVector(replacementVectorSimd),
		narrowingLimitVector(narrowingLimitVectorSimd)
	{}

	const void* replacementVector = nullptr;
	const void* narrowingLimitVector = nullptr;

	const _FromChar_* inputStringDataStart = nullptr;
	size_t stringSizeInBytes = 0;

	_ToChar_* outputStringDataStart = nullptr;
};

template <
	class _SimdType_,
	class _NarrowingConversionBehaviour_>
class StringConverterTraits {
	static_assert(
		_SimdType_::value == CpuFeature::SSE2	|| 
		_SimdType_::value == CpuFeature::AVX	|| 
		_SimdType_::value == CpuFeature::AVX512 || 
		_SimdType_::value == CpuFeature::None,
		"base::core::string::StringConverterTraits<_SimdType_, _NarrowingConversionBehaviour_>: Invalid _SimdType_");
};

__BASE_STRING_NAMESPACE_END

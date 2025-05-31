#pragma once 

#include <src/core/arch/CpuFeatureSimdTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	typename	_FromChar_,
	typename	_ToChar_,
	class		_SimdType_>
struct StringConversionParameters
{
	using SimdVectorType = SimdVectorIntType<_SimdType_>;

	StringConversionParameters(
		const _FromChar_*			inputString,
		size_t						inputStringLength,
		_ToChar_*					outputString,
		const SimdVectorType*		replacementVectorSimd = nullptr,
		const SimdVectorType*		narrowingLimitVectorSimd = nullptr
	) noexcept:
		inputStringDataStart(inputString),
		stringLength(inputStringLength),
		outputStringDataStart(outputString),
		replacementVector(replacementVectorSimd),
		narrowingLimitVector(narrowingLimitVectorSimd)
	{}

	const SimdVectorType* replacementVector = nullptr;
	const SimdVectorType* narrowingLimitVector = nullptr;

	const _FromChar_* inputStringDataStart = nullptr;
	size_t stringLength = 0;

	_ToChar_* outputStringDataStart = nullptr;
};

__BASE_STRING_NAMESPACE_END

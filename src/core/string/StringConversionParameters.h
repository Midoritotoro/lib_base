#pragma once 

#include <src/core/arch/CpuFeatureSimdTraits.h>
#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	typename	_FromChar_,
	typename	_ToChar_,
	CpuFeature _SimdType_>
struct StringConversionParameters
{
	using SimdVectorType = SimdVectorIntType<_SimdType_>;
public:
	StringConversionParameters() noexcept = default;
	~StringConversionParameters() noexcept = default;

	StringConversionParameters(
		const _FromChar_*		inputString,
		size_t					inputStringLength,
		_ToChar_*				outputString,
		const SimdVectorType*	replacementVectorSimd = nullptr,
		const SimdVectorType*	narrowingLimitVectorSimd = nullptr
	) noexcept:
		inputStringDataStart(inputString),
		stringLength(inputStringLength),
		outputStringDataStart(outputString),
		replacementVector((replacementVectorSimd)),
		narrowingLimitVector((narrowingLimitVectorSimd))
	{}

	const SimdVectorType* replacementVector = nullptr;
	const SimdVectorType* narrowingLimitVector = nullptr;

	const _FromChar_* inputStringDataStart = nullptr;
	size_t stringLength = 0;

	_ToChar_* outputStringDataStart = nullptr;

	const _FromChar_* originalInputStringDataStart = nullptr;
	_ToChar_* originalOutputStringDataStart = nullptr;
};


__BASE_STRING_NAMESPACE_END

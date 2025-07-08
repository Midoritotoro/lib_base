#pragma once 

#include <src/core/arch/CpuFeatureSimdTraits.h>
#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	typename	_FromChar_,
	typename	_ToChar_>
struct StringConversionParameters
{
	using SimdVectorType = SimdVectorIntType<_SimdType_>;
public:
	static constexpr const CpuFeature feature = _SimdType_;

	StringConversionParameters() noexcept = default;
	~StringConversionParameters() noexcept = default;

	StringConversionParameters(
		const _FromChar_*		inputString,
		size_t					inputStringLength,
		_ToChar_*				outputString
	) noexcept:
		inputStringDataStart(inputString),
		stringLength(inputStringLength),
		outputStringDataStart(outputString)
	{}

	const _FromChar_* inputStringDataStart = nullptr;
	size_t stringLength = 0;

	_ToChar_* outputStringDataStart = nullptr;

	const _FromChar_* originalInputStringDataStart = nullptr;
	_ToChar_* originalOutputStringDataStart = nullptr;
};


__BASE_STRING_NAMESPACE_END

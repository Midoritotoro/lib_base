#pragma once 

#include <src/core/arch/CpuFeatureSimdTraits.h>
#include <base/core/arch/CpuFeature.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	typename	_FromChar_,
	typename	_ToChar_>
struct StringConversionParameters
{
public:
	StringConversionParameters() noexcept = default;
	~StringConversionParameters() noexcept = default;

	StringConversionParameters(
		const _FromChar_*		inputString,
		size_t					inputStringLength,
		_ToChar_*				outputString
	) noexcept:
		inputStringDataStart(inputString),
		stringLength(inputStringLength),
		outputStringDataStart(outputString),
		originalInputStringDataStart(inputString),
		originalOutputStringDataStart(outputString)
	{}

	const void* inputStringDataStart = nullptr;
	size_t stringLength = 0;

	void* outputStringDataStart = nullptr;

	const _FromChar_* const originalInputStringDataStart = nullptr;
	const _ToChar_* const originalOutputStringDataStart = nullptr;
};


__BASE_STRING_NAMESPACE_END

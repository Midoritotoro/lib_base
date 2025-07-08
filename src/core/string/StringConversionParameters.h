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
	static constexpr const CpuFeature feature = _SimdType_;

	StringConversionParameters() noexcept = default;
	~StringConversionParameters() noexcept = default;

	template <
		typename	_FromChar_,
		typename	_ToChar_, 
		CpuFeature	_SimdType_> 
	static NODISCARD StringConversionParameters<_FromChar_, _ToChar_, downcast_simd_feature_v<_SimdType_>>
		downcast(StringConversionParameters<_FromChar_, _ToChar_, _SimdType_>* parameters) noexcept
	{
		if constexpr (downcast_simd_feature_v<_SimdType_> == CpuFeature::None)
			return *parameters;
		
		using DowncastedSimdVectorType = SimdVectorIntType<downcast_simd_feature_v<_SimdType_>>;

		return StringConversionParameters<_FromChar_, _ToChar_, downcast_simd_feature_v<_SimdType_>>(
			parameters->inputStringDataStart, 
			parameters->stringLength, 
			parameters->outputStringDataStart,
			reinterpret_cast<const DowncastedSimdVectorType*>(parameters->replacementVector),
			reinterpret_cast<const DowncastedSimdVectorType*>(parameters->narrowingLimitVector));
	}

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

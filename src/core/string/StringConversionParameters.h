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

private:
	template <typename _BaseVector_>
	static NODISCARD SimdVectorType simdVectorFromBaseVector(_BaseVector_* vectorBase) noexcept {
		if constexpr (std::is_same_v<_BaseVector_, base_vec128i_t>)
			return base_vec128i_t_pointer_as_m128i_pointer(vectorBase);
		else if constexpr (std::is_same_v<_BaseVector_, base_vec256i_t>)
			return base_vec256i_t_pointer_as_m256i_pointer(vectorBase);
		else if constexpr (std::is_same_v<_BaseVector_, base_vec512i_t>)
			return base_vec512i_t_pointer_as_m512i_pointer(vectorBase);

		AssertUnreachable();
		return {};
	}
public:
	StringConversionParameters() noexcept = default;
	~StringConversionParameters() noexcept = default;

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

	template <
		typename _BaseVector_,
		std::enable_if_t<IsAnyOf_v<_BaseVector_, base_vec128i_t, base_vec256i_t, base_vec512i_t>> = 0>
	StringConversionParameters(
		const _FromChar_*			inputString,
		size_t						inputStringLength,
		_ToChar_*					outputString,
		const _BaseVector_*			replacementVectorSimd = nullptr,
		const _BaseVector_*			narrowingLimitVectorSimd = nullptr
	) noexcept:
		inputStringDataStart(inputString),
		stringLength(inputStringLength),
		outputStringDataStart(outputString),
		replacementVector(simdVectorFromBaseVector(replacementVectorSimd)),
		narrowingLimitVector(simdVectorFromBaseVector(narrowingLimitVectorSimd))
	{}

	const SimdVectorType* replacementVector = nullptr;
	const SimdVectorType* narrowingLimitVector = nullptr;

	const _FromChar_* inputStringDataStart = nullptr;
	size_t stringLength = 0;

	_ToChar_* outputStringDataStart = nullptr;
};

__BASE_STRING_NAMESPACE_END

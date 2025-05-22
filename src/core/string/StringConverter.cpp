#include <src/core/string/StringConverter.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <locale>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
template <
	class _FromType_,
	class _ToType_,
	std::enable_if_t<
		StringConverter<_NarrowingConversionBehaviour_>::IsSupportedString<_FromType_> &&
		StringConverter<_NarrowingConversionBehaviour_>::IsSupportedString<_ToType_>>>
NODISCARD _ToType_ StringConverter<_NarrowingConversionBehaviour_>::convertString(const _FromType_& string) {
	if (ProcessorFeatures::AVX512F())
		return convertStringImplementation<_FromType_, _ToType_>(
			string, CpuFeatureTag<CpuFeature::AVX512F>{});

	else if (ProcessorFeatures::AVX2())
		return convertStringImplementation<_FromType_, _ToType_>(
			string, CpuFeatureTag<CpuFeature::AVX2>{});

	else if (ProcessorFeatures::SSE2())
		return convertStringImplementation<_FromType_, _ToType_>(
			string, CpuFeatureTag<CpuFeature::SSE2>{});

	return convertStringImplementation<_FromType_, _ToType_>(
		string, CpuFeatureTag<CpuFeature::None>{});
}


__BASE_STRING_NAMESPACE_END
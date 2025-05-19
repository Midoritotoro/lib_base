#include <src/core/string/StringConverter.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	class _FromType_,
	class _ToType_,
	std::enable_if_t<
		StringConverter::IsSupportedString<_FromType_> &&
		StringConverter::IsSupportedString<_ToType_>>>
NODISCARD _ToType_ StringConverter::convertString(const _FromType_& string) { 

}

template <
	class _FromType_,
	class _ToType_,
	class _Tag_,
	std::enable_if_t<
		StringConverter::IsSupportedString<_FromType_> &&
		StringConverter::IsSupportedString<_ToType_>>>
NODISCARD _ToType_ StringConverter::convertStringImplementation(
	const _FromType_& string,
	CpuFeatureTag<CpuFeature::None>)
{

}

template <
	class _FromType_,
	class _ToType_,
	class _Tag_,
	std::enable_if_t<
		StringConverter::IsSupportedString<_FromType_> &&
		StringConverter::IsSupportedString<_ToType_>>>
NODISCARD _ToType_ StringConverter::convertStringImplementation(
	const _FromType_& string,
	CpuFeatureTag<CpuFeature::SSE2>)
{

}

template <
	class _FromType_,
	class _ToType_,
	class _Tag_,
	std::enable_if_t<
		StringConverter::IsSupportedString<_FromType_> &&
		StringConverter::IsSupportedString<_ToType_>>>
NODISCARD _ToType_ StringConverter::convertStringImplementation(
	const _FromType_& string,
	CpuFeatureTag<CpuFeature::AVX2>)
{

}

template <
	class _FromType_,
	class _ToType_,
	class _Tag_,
	std::enable_if_t<
		StringConverter::IsSupportedString<_FromType_> &&
		StringConverter::IsSupportedString<_ToType_>>>
NODISCARD _ToType_ StringConverter::convertStringImplementation(
	const _FromType_& string,
	CpuFeatureTag<CpuFeature::AVX512F>)
{

}

template <>
NODISCARD std::wstring StringConverter::convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
	const std::string& string,
	CpuFeatureTag<CpuFeature::AVX512F>)
{

}

__BASE_STRING_NAMESPACE_END
#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/core/string/StringConfig.h>

#include <base/core/arch/CpuFeatureTag.h>

__BASE_STRING_NAMESPACE_BEGIN

static class StringConverter {
public:
	template <class _String_>
	static inline constexpr bool IsSupportedString = 
		IsAnyOf_v<
			_String_, std::string, std::wstring,
			std::u8string, std::u16string, std::u32string>;

	template <
		class _FromType_,
		class _ToType_,
		std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertString(const _FromType_& string);
private:
	
	
	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::None>);

	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::SSE2>);

	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::AVX2>);

	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::AVX512F>);

	template <>
	NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>);
};

__BASE_STRING_NAMESPACE_END
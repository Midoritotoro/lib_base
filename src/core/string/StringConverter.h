#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/core/string/StringConfig.h>

#include <base/core/arch/CpuFeatureTag.h>

#include <base/core/memory/MemoryAllocation.h>
#include <src/core/memory/MemoryUtility.h>

#include <base/core/utility/IntegralTypesConversions.h>
#include <src/core/string/StringConversionOptions.h>

__BASE_STRING_NAMESPACE_BEGIN

// template <class _NarrowingConversionBehaviour_ = DefaultReplacementConversionMode>
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
		typename = std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		_Tag_)
	{
		return {};
	}


	/*template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		typename = std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		typename = std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		typename = std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		typename = std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	NODISCARD _ToType_ convertStringImplementation(
		const _FromType_& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}*/

	// =================================================================================================================
	//														AVX512F
	// =================================================================================================================

	// =================================================================================================================
	//													To std::string
	// =================================================================================================================

#if __cpp_lib_char8_t
	template <>
	NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		const auto fromSizeInBytes = size_t(string.size() * sizeof(char8_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		char* converted = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));

		if (avx512SizeInBytes != 0) {
			constexpr auto toLimit = MaximumIntegralLimit<std::string::value_type>();

			const void* stringDataStart = string.data();
			const void* stopAt = string.data() + avx512SizeInBytes;
		
			const auto lessThanCompare = _mm512_set1_epi8(toLimit);

			do {
				const auto loaded = _mm512_loadu_si512(stringDataStart);
				const uint64 comparedGreaterThan = _mm512_cmpgt_epi8_mask(loaded, lessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
					switch (0) {};
				}

				_mm512_store_si512(converted, loaded);
				memory::AdvanceBytes(stringDataStart, 64);
			} while (stringDataStart != stopAt);
		}
	}
#endif

	template <>
	NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::wstring
	// =================================================================================================================

	template <>
	NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u8string
	// =================================================================================================================
	

	template <>
	NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u16string
	// =================================================================================================================
	
	template <>
	NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u32string
	// =================================================================================================================

	template <>
	NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//														AVX2
	// =================================================================================================================

	// =================================================================================================================
	//													To std::string
	// =================================================================================================================

	template <>
	NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::wstring
	// =================================================================================================================

	template <>
	NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u8string
	// =================================================================================================================


	template <>
	NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u16string
	// =================================================================================================================

	template <>
	NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u32string
	// =================================================================================================================

	template <>
	NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}


	// =================================================================================================================
	//														SSE2
	// =================================================================================================================

	// =================================================================================================================
	//													To std::string
	// =================================================================================================================

	template <>
	NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::wstring
	// =================================================================================================================

	template <>
	NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u8string
	// =================================================================================================================


	template <>
	NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u16string
	// =================================================================================================================

	template <>
	NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u32string
	// =================================================================================================================

	template <>
	NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}
};

__BASE_STRING_NAMESPACE_END
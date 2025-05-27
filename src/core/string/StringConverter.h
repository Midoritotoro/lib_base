#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/core/string/StringConfig.h>

#include <base/core/arch/CpuFeatureTag.h>

#include <base/core/memory/MemoryAllocation.h>
#include <src/core/memory/MemoryUtility.h>

#include <base/core/utility/IntegralTypesConversions.h>
#include <src/core/string/StringConversionOptions.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <src/core/utility/simd/SimdConstexprHelpers.h>

#include <src/core/memory/Alignment.h>

#include <base/core/arch/ProcessorFeatures.h>


__BASE_STRING_NAMESPACE_BEGIN

template <class _Char_> 
static inline constexpr bool IsSupportedCharType = 
    IsAnyOf_v<
        _Char_, Char, char, 
        char16_t, char32_t, wchar_t
#if __cpp_lib_char8_t
    , char8_t
#endif
>;

template <
    typename _Char_,
    typename = std::enable_if_t<IsSupportedCharType<_Char_>>>
class StringConversionResult {
    constexpr StringConversionResult() noexcept = default;
    constexpr inline StringConversionResult(
        _Char_* dataStart, 
        size_t dataLength, 
        bool isNarrowingConversion
    ) noexcept:
        _dataStart(dataStart),
        _dataLength(dataLength),
        _isNarrowingConversion(isNarrowingConversion)
    {}
	

    constexpr inline NODISCARD bool isNull() const noexcept {
        return (_dataStart == nullptr || _dataLength == 0);
    }

	constexpr inline NODISCARD _Char_* data() noexcept {
		return _dataStart;
	}

	constexpr inline NODISCARD size_t length() const noexcept {
		return _dataLength;
	}

	constexpr inline NODISCARD size_t size() const noexcept {
		return _dataLength;
	}

	constexpr inline NODISCARD bool isNarrowingConversion() const noexcept {
		return _isNarrowingConversion;
	}

	constexpr inline void setData(_Char_* newData) noexcept {
		_dataStart = newData;
	}

	constexpr inline void setSize(size_t newSize) noexcept {
		_dataLength = newSize;
	}

	constexpr inline void setNarrowingConversion(bool isNarrowingConversion) noexcept {
		_isNarrowingConversion = isNarrowingConversion;
	}
private:
    _Char_* _dataStart           = nullptr;
    size_t _dataLength           = 0;
    // true if data loss occurred during conversion from a more 
    // spacious type to a less spacious one (narrowing conversion)
    bool _isNarrowingConversion  = false;
};

template <class _NarrowingConversionBehaviour_ = DefaultReplacementConversionMode>
class StringConverter {
public:
	template <
		typename _FromChar_,
		typename _ToChar_,
		std::enable_if_t<
			IsSupportedCharType<_FromChar_> &&
			IsSupportedCharType<_ToChar_>>>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const _FromChar_* const string,
		const size_t			length)
	{
		if (ProcessorFeatures::AVX512F())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX512F>>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::AVX512F>{});

		else if (ProcessorFeatures::AVX2())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX2>>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::AVX2>{});

		else if (ProcessorFeatures::SSE2())
			return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::SSE2>>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::SSE2>{});

		return convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::None>>(
			string, length, nullptr, CpuFeatureTag<CpuFeature::None>{});
	}
private:
	template <
		class _FromChar_,
		class _ToChar_,
		class _Tag_,
		typename = std::enable_if_t<
			IsSupportedCharType<_FromChar_> &&
			IsSupportedCharType<_ToChar_>>>
	static NODISCARD StringConversionResult<_ToChar_> convertStringImplementation(
		const _FromChar_* const string,
		const size_t			stringLength,
		_ToChar_*				outputString,
		_Tag_)
	{
		AssertUnreachable();
		return {};
	}

	// =================================================================================================================
	//														AVX512F
	// =================================================================================================================

	// =================================================================================================================
	//													    To char
	// =================================================================================================================

#if __cpp_lib_char8_t
	template <>
	static NODISCARD StringConversionResult<char> convertStringImplementation<char8_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char8_t* const	string,
		const size_t			stringLength,
		char*					outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
        constexpr auto toLimit = MaximumIntegralLimit<char8_t>();

		constexpr auto replacementVector = base_vec512i_t_pointer_as_m512i(
			base_constexpr_mm512_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter));

		constexpr auto lessThanCompare = base_vec512i_t_pointer_as_m512i(
			base_constexpr_mm512_set1_epi8(toLimit));

        static_assert(
            _NarrowingConversionBehaviour_::replacementCharacter <= toLimit, 
            "base::core::string::StringConverter::convertStringImplementation"
            "_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. "); 

        if (string == nullptr || stringLength == 0)
            return {};
        
        bool isNarrowingConversion = false;

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char8_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

        if (outputString == nullptr)
		    char* outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));
#if defined(_DEBUG)
        else
            DebugAssertLog(
                memory::isAligned(outputString, 64),
                "base::core::string::StringConverter::convertStringImplementation: "
                "outputString pointer must be aligned on a 64 byte boundary to improve performance. ");
#endif

        const void* stringDataStart = string;
        const void* stringDataEnd   = stringDataStart;

        const void* stopAt = stringDataStart;

        memory::AdvanceBytes(stopAt, avx512SizeInBytes);
        memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);
        
        if (avx512SizeInBytes != 0) {
			do {
				auto loaded = _mm512_loadu_si512(stringDataStart);
				const uint64 comparedGreaterThan = _mm512_cmpgt_epi8_mask(loaded, lessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
                    isNarrowingConversion = true;

					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
						case NarrowingConversionMode::CustomReplacement:
                        case NarrowingConversionMode::DefaultReplacement:
                            // If a character in 'loaded' is greater than the threshold (mask bit is 1), it's replaced with
                            // 'replacementCharacter'. Otherwise, it's copied from 'loaded'.
                            
                            _mm512_mask_store_epi8(outputString, ~comparedGreaterThan, loaded);
							_mm512_mask_store_epi8(outputString, comparedGreaterThan, replacementVector);

                            break;
						default:
                            AssertUnreachable();
                            break;
					};
				}
                else {
                    _mm512_store_si512(outputString, loaded);
                }

				memory::AdvanceBytes(stringDataStart, 64);
                memory::AdvanceBytes(outputString, 64);
			} while (stringDataStart != stopAt);
		}

        if (stringDataStart == stringDataEnd)
            return StringConversionResult<char>(
				outputString - fromSizeInBytes,
                fromSizeInBytes, isNarrowingConversion);

        return convertStringImplementation<char8_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
			static_cast<const char8_t*>(stringDataStart), fromSizeInBytes - avx512SizeInBytes,
            outputString, CpuFeatureTag<CpuFeature::AVX2>{});
	}
#endif

	template <>
	static NODISCARD StringConversionResult<char> convertStringImplementation<char16_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char16_t* const   string,
        const size_t            stringLength,
        char*                   outputString,
		CpuFeatureTag<CpuFeature::AVX512F> tag)
	{
		constexpr auto toLimit = MaximumIntegralLimit<char16_t>();

		constexpr auto replacementVector = base_vec512i_t_pointer_as_m512i(
			base_constexpr_mm512_set1_epi16(_NarrowingConversionBehaviour_::replacementCharacter));

		constexpr auto lessThanCompare = base_vec512i_t_pointer_as_m512i(
			base_constexpr_mm512_set1_epi16(toLimit));

		static_assert(
			_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
			"base::core::string::StringConverter::convertStringImplementation"
			"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

		if (string == nullptr || stringLength == 0)
			return {};

		bool isNarrowingConversion = false;

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char16_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			char* outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));
#if defined(_DEBUG)
		else
			DebugAssertLog(
				memory::isAligned(outputString, 64),
				"base::core::string::StringConverter::convertStringImplementation: "
				"outputString pointer must be aligned on a 64 byte boundary to improve performance. ");
#endif

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		const void* stopAt = stringDataStart;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);

		if (avx512SizeInBytes != 0) {
			do {
				auto loaded = _mm512_loadu_si512(stringDataStart);
				const uint32 comparedGreaterThan = _mm512_cmpgt_epi16_mask(loaded, lessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
					isNarrowingConversion = true;

					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
					case NarrowingConversionMode::CustomReplacement:
					case NarrowingConversionMode::DefaultReplacement:
						// If a character in 'loaded' is greater than the threshold (mask bit is 1), it's replaced with
						// 'replacementCharacter'. Otherwise, it's copied from 'loaded'.

						_mm512_mask_store_epi16(outputString, ~comparedGreaterThan, loaded);
						_mm512_mask_store_epi16(outputString, comparedGreaterThan, replacementVector);

						break;
					default:
						AssertUnreachable();
						break;
					};
				}
				else {
					_mm512_store_si512(outputString, loaded);
				}

				memory::AdvanceBytes(stringDataStart, 64);
				memory::AdvanceBytes(outputString, 64);
			} while (stringDataStart != stopAt);
		}

		if (stringDataStart == stringDataEnd)
			return StringConversionResult<char>(
				outputString - fromSizeInBytes,
				fromSizeInBytes, isNarrowingConversion);

		return convertStringImplementation<char16_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
		    static_cast<const char16_t*>(stringDataStart), fromSizeInBytes - avx512SizeInBytes,
			outputString, CpuFeatureTag<CpuFeature::AVX2>{});
    }

	template <>
	static NODISCARD StringConversionResult<char> convertStringImplementation<char32_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char32_t* const   string,
		const size_t            stringLength,
		char*					outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		constexpr auto toLimit = MaximumIntegralLimit<char32_t>();

		constexpr auto replacementVector = base_vec512i_t_pointer_as_m512i(
			base_constexpr_mm512_set1_epi32(_NarrowingConversionBehaviour_::replacementCharacter));

		constexpr auto lessThanCompare = base_vec512i_t_pointer_as_m512i(
			base_constexpr_mm512_set1_epi32(toLimit));

		static_assert(
			_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
			"base::core::string::StringConverter::convertStringImplementation"
			"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

		if (string == nullptr || stringLength == 0)
			return {};

		bool isNarrowingConversion = false;

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char32_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			char* outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));
#if defined(_DEBUG)
		else
			DebugAssertLog(
				memory::isAligned(outputString, 64),
				"base::core::string::StringConverter::convertStringImplementation: "
				"outputString pointer must be aligned on a 64 byte boundary to improve performance. ");
#endif

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		const void* stopAt = stringDataStart;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);

		if (avx512SizeInBytes != 0) {
			do {
				auto loaded = _mm512_loadu_si512(stringDataStart);
				const uint16 comparedGreaterThan = _mm512_cmpgt_epi32_mask(loaded, lessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
					isNarrowingConversion = true;

					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
					case NarrowingConversionMode::CustomReplacement:
					case NarrowingConversionMode::DefaultReplacement:
						// If a character in 'loaded' is greater than the threshold (mask bit is 1), it's replaced with
						// 'replacementCharacter'. Otherwise, it's copied from 'loaded'.

						_mm512_mask_store_epi32(outputString, ~comparedGreaterThan, loaded);
						_mm512_mask_store_epi32(outputString, comparedGreaterThan, replacementVector);

						break;
					default:
						AssertUnreachable();
						break;
					};
				}
				else {
					_mm512_store_si512(outputString, loaded);
				}

				memory::AdvanceBytes(stringDataStart, 64);
				memory::AdvanceBytes(outputString, 64);
			} while (stringDataStart != stopAt);
		}

		if (stringDataStart == stringDataEnd)
			return StringConversionResult<char>(
				outputString - fromSizeInBytes,
				fromSizeInBytes, isNarrowingConversion);

		return convertStringImplementation<char32_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
			static_cast<const char32_t*>(stringDataStart), fromSizeInBytes - avx512SizeInBytes,
			outputString, CpuFeatureTag<CpuFeature::AVX2>{});
	}

	template <>
	static NODISCARD StringConversionResult<char> convertStringImplementation<wchar_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const wchar_t* const	string,
		const size_t            stringLength,
		char*					outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		// Windows 
		if constexpr (std::is_same_v<wchar_t, char16_t>)
			return convertStringImplementation<char16_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
				static_cast<const char16_t* const>(string), stringLength,
				outputString, CpuFeatureTag<CpuFeature::AVX512F>{});

		// Linux and etc.
		else if constexpr (std::is_same_v<wchar_t, char32_t>)
			return convertStringImplementation<char32_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
				static_cast<const char32_t* const>(string), stringLength,
				outputString, CpuFeatureTag<CpuFeature::AVX512F>{});

		AssertUnreachable();
		return {};
	}

	template <>
	static NODISCARD StringConversionResult<char> convertStringImplementation<char, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char* const	string,
		const size_t        stringLength,
		char*				outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		unused(stringLength);
		unused(outputString);
		return StringConversionResult<char>(string, stringLength, false);
	}


	// =================================================================================================================
	//													To wchar_t
	// =================================================================================================================

	
	template <>
	static NODISCARD StringConversionResult<wchar_t> convertStringImplementation<char, wchar_t, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char* const	string,
		const size_t        stringLength,
		wchar_t*			outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		if (string == nullptr || stringLength == 0)
			return {};

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char));
		const auto outputSizeInBytes = size_t(stringLength * sizeof(wchar_t));

		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			wchar_t* outputString = static_cast<wchar_t*>(memory::AllocateAligned(outputSizeInBytes, 64));
#if defined(_DEBUG)
		else
			DebugAssertLog(
				memory::isAligned(outputString, 64),
				"base::core::string::StringConverter::convertStringImplementation: "
				"outputString pointer must be aligned on a 64 byte boundary to improve performance. ");
#endif

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		const void* stopAt = stringDataStart;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);
		
		if (avx512SizeInBytes != 0) {
			do {
				if constexpr (sizeof(wchar_t) == 4) {
				    _mm512_store_si512(outputString, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart))));

                    _mm512_store_si512(outputString + 64, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart + 16))));

                    _mm512_store_si512(outputString + 128, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart + 32))));

                    _mm512_store_si512(outputString + 192, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart + 48))));

                    memory::AdvanceBytes(outputString, 256);  
				}
				else if constexpr (sizeof(wchar_t) == 2) {
				    _mm512_store_si512(outputString, _mm512_cvtepi8_epi16(
                        _mm256_load_si256(reinterpret_cast<__m256i*>(stringDataStart))));

                    _mm512_store_si512(outputString + 64, _mm512_cvtepi8_epi16(
                        _mm256_load_si256(reinterpret_cast<__m256i*>(stringDataStart + 32))));

                    memory::AdvanceBytes(outputString, 128);
				}

				memory::AdvanceBytes(stringDataStart, 64);
			} while (stringDataStart != stopAt);
		}

        if (stringDataStart == stringDataEnd)
            return StringConversionResult<wchar_t>(
                outputString - outputSizeInBytes,
                outputSizeInBytes, false);

        return convertStringImplementation<char, wchar_t, CpuFeatureTag<CpuFeature::AVX2>>(
            static_cast<const char*>(stringDataStart), fromSizeInBytes - avx512SizeInBytes,
            outputString, CpuFeatureTag<CpuFeature::AVX2>{});
	}

#if __cpp_lib_char8_t
	template <>
	static NODISCARD StringConversionResult<wchar_t> convertStringImplementation<char8_t, wchar_t, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char8_t* const	string,
		const size_t        	stringLength,
		wchar_t*				outputString,
		CpuFeatureTag<CpuFeature::AVX512F> tag)
	{
		if (string == nullptr || stringLength == 0)
			return {};

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char));
		const auto outputSizeInBytes = size_t(stringLength * sizeof(wchar_t));

		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			wchar_t* outputString = static_cast<wchar_t*>(memory::AllocateAligned(outputSizeInBytes, 64));
#if defined(_DEBUG)
		else
			DebugAssertLog(
				memory::isAligned(outputString, 64),
				"base::core::string::StringConverter::convertStringImplementation: "
				"outputString pointer must be aligned on a 64 byte boundary to improve performance. ");
#endif

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		const void* stopAt = stringDataStart;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);
		
		if (avx512SizeInBytes != 0) { 
			do {
				if constexpr (sizeof(wchar_t) == 4) {
				    _mm512_store_si512(outputString, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart))));

                    _mm512_store_si512(outputString + 64, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart + 16))));

                    _mm512_store_si512(outputString + 128, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart + 32))));

                    _mm512_store_si512(outputString + 192, _mm512_cvtepi8_epi32(
                        _mm_load_si128(reinterpret_cast<__m128i*>(stringDataStart + 48))));

                    memory::AdvanceBytes(outputString, 256);  
				}
				else if constexpr (sizeof(wchar_t) == 2) {
				    _mm512_store_si512(outputString, _mm512_cvtepi8_epi16(
                        _mm256_load_si256(reinterpret_cast<__m256i*>(stringDataStart))));

                    _mm512_store_si512(outputString + 64, _mm512_cvtepi8_epi16(
                        _mm256_load_si256(reinterpret_cast<__m256i*>(stringDataStart + 32))));

                    memory::AdvanceBytes(outputString, 128);
				}

				memory::AdvanceBytes(stringDataStart, 64);
			} while (stringDataStart != stopAt);
		}

        if (stringDataStart == stringDataEnd)
            return StringConversionResult<wchar_t>(
                outputString - outputSizeInBytes,
                outputSizeInBytes, false);

        return convertStringImplementation<char8_t, wchar_t, CpuFeatureTag<CpuFeature::AVX2>>(
            static_cast<const char8_t*>(stringDataStart), fromSizeInBytes - avx512SizeInBytes,
            outputString, CpuFeatureTag<CpuFeature::AVX2>{});
	}
#endif

	//template <>
	//static NODISCARD StringConversionResult<wchar_t> convertStringImplementation<char16_t, wchar_t, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const char16_t* const	string,
	//	const size_t        	stringLength,
	//	wchar_t*				outputString = nullptr,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	// if constexpr (std::is_same_v<wchar_t, char16_t>)
	//	// 	return StringConversionResult<wchar_t>(static_cast<const)
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u8string
	//// =================================================================================================================
	//

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u16string
	//// =================================================================================================================
	//
	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u32string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX512F>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////														AVX2
	//// =================================================================================================================

	//// =================================================================================================================
	////													To std::string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::wstring
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u8string
	//// =================================================================================================================


	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u16string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u32string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::AVX2>)
	//{
	//	return string;
	//}


	//// =================================================================================================================
	////														SSE2
	//// =================================================================================================================

	//// =================================================================================================================
	////													To std::string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::wstring
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u8string
	//// =================================================================================================================


	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u16string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u32string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::SSE2>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////														Scalar
	//// =================================================================================================================

	//// =================================================================================================================
	////													To std::string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::wstring
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::None>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u8string
	//// =================================================================================================================


	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u16string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return string;
	//}

	//// =================================================================================================================
	////													To std::u32string
	//// =================================================================================================================

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::wstring& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u8string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u16string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return {};
	//}

	//template <>
	//static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
	//	const std::u32string& string,
	//	CpuFeatureTag<CpuFeature::None>)
	//{
	//	return string;
	//}

	// =================================================================================================================
	//														Masks
	// =================================================================================================================4
};

__BASE_STRING_NAMESPACE_END

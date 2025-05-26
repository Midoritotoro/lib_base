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
    std::enable_if_t<IsCompatibleCharType<_Char_>>>
struct NODISCARD StringConversionResult { 
    StringConversionResult() noexcept = default;
    StringConversionResult(
        _Char_* _data, 
        size_t _dataLength, 
        bool _isNarrowingConversion
    ) noexcept:
        dataStart(_data), 
        dataLength(_dataLength),
        isNarrowingConversion(_isNarrowingConversion)
    {}

    NODISCARD bool isNull() const noexcept { 
        return (dataStart == nullptr || dataLength == 0);
    }

    _Char_* dataStart           = nullptr;
    size_t dataLength           = 0;
    // true if data loss occurred during conversion from a more 
    // spacious type to a less spacious one (narrowing conversion)
    bool isNarrowingConversion  = false;
};

template <class _NarrowingConversionBehaviour_ = DefaultReplacementConversionMode>
class StringConverter {
public:
	template <
		class _FromChar_,
		class _ToChar_,
	std::enable_if_t<
		IsSupportedCharType<_FromChar_> &&
		IsSupportedCharType<_ToChar_>>>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const _FromChar_* const string,
		const size_t			length)
	{
		if (ProcessorFeatures::AVX512F())
			return convertStringImplementation<_FromChar_, _ToChar_>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::AVX512F>{});

		else if (ProcessorFeatures::AVX2())
			return convertStringImplementation<_FromChar_, _ToChar_>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::AVX2>{});

		else if (ProcessorFeatures::SSE2())
			return convertStringImplementation<_FromChar_, _ToChar_>(
				string, length, nullptr, CpuFeatureTag<CpuFeature::SSE2>{});

		return convertStringImplementation<_FromChar_, _ToChar_>(
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
    // Returns outputString
	static NODISCARD StringConversionResult<_ToChar_> convertStringImplementation(
		const _FromChar_* const string,
		const size_t			stringLength,
		_ToChar_*				outputString = nullptr,
		_Tag_)
	{
		AssertUnreachable();
		return {};
	}

	// =================================================================================================================
	//														AVX512F
	// =================================================================================================================

	// =================================================================================================================
	//													To std::string
	// =================================================================================================================

#if __cpp_lib_char8_t
	template <>
	static NODISCARD StringConversionResult<_ToChar_> convertStringImplementation<char8_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char8_t* const	string,
		const size_t			stringLength,
		char*					outputString = nullptr,
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
                memory::IsAlignment(outputString, 64),
                "base::core::string::StringConverter::convertStringImplementation: "
                "outputString pointer must be aligned on a 64 byte boundary to improve performance. ");
#endif

        const void* stringDataStart = string;
        const void* stringDataEnd   = stringDataStart;

        const void* stopAt = stringDataStart;

        memory::AdvanceBytes(stopAt, avx512SizeInBytes);
        memory::AdvanceBytes(stringDataEnd, stringLength * sizeof(char8_t));
        
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
                            
                            _mm512_mask_storeu_epi8(converted, ~comparedGreaterThan, loaded);
							_mm512_mask_storeu_epi8(converted, comparedGreaterThan, replacementVector);

                            break;
						default:
                            AssertUnreachable();
                            break;
					};
				}
                else {
                    _mm512_store_si512(converted, loaded);
                }

				memory::AdvanceBytes(stringDataStart, 64);
                memory::AdvanceBytes(converted, 64);
			} while (stringDataStart != stopAt);
		}

        if (stringDataStart == stringDataEnd)
            return StringConversionResult<char>(
                converted - fromSizeInBytes,
                fromSizeInBytes, isNarrowingConversion);

        return convertStringImplementation<char8_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
            converted - fromSizeInBytes, fromSizeInBytes,
            isNarrowindConversion, CpuFeatureTag<CpuFeature::AVX2>>);
	}
#endif

	template <>
	static NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
        constexpr auto toLimit = MaximumIntegralLimit<std::string::value_type>();

         static_assert(
             _NarrowingConversionBehaviour_::replacementCharacter <= toLimit, 
            "base::core::string::StringConverter::convertStringImplementation: _NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. "); 

         if (string.size() == 0)
            return std::string{};

        const auto fromSizeInBytes = size_t(string.size() * sizeof(std::u16string::value_type));
        const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

        char* converted = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));

        const void* stringDataStart = string.data();
        const void* stringDataEnd   = stringDataStart;

        const void* stopAt = stringDataStart;

        memory::AdvanceBytes(stopAt, avx512SizeInBytes);
        memory::AdvanceBytes(stringDataEnd, string.size() * sizeof(std::string::value_type));

		__m512i replacementVector;
		__m512i lessThanCompare; 

        if (avx512SizeInBytes != 0) {
			replacementVector = _mm512_set1_epi16(toLimit);
			lessThanCompare = _mm512_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
            
			do { 
                const auto loaded = _mm512_loadu_si512(stringDataStart);
                const auto comparedGreaterThan = _mm512_cmpgt_epi16_mask(loaded, lessThanCompare);
                
                // Narrowing conversion
                if (comparedGreaterThan != 0) { 
                    __m512i convertedEpi8;

                    if constexpr (std::is_same_v<std::u16string::value_type, ushort>)
                        convertedEpi8 = _mm512_packus_epi16(loaded, loaded);
                    else
                        convertedEpi8 = _mm512_packs_epi16(loaded, loaded);
                    
                    //  convertedEpi8 = _mm512_permutex_var64(convertedEpi8, _MM_SHUFFLE(3, 1, 2, 0));

                    _mm512_store_si512(converted, convertedEpi8);
                }
                else { 
                    _mm512_store_si512(converted, loaded);
                }

                memory::AdvanceBytes(stringDataStart, 64);
                memory::AdvanceBytes(converted, 64);
            } while (stringDataStart != stringDataEnd);
    
        }

        if (stringDataStart == stringDataEnd)
            return std::string(converted - fromSizeInBytes, fromSizeInBytes);

        const auto avx512TailSize = fromSizeInBytes/* & AVX512_ALIGNED_TAIL_MASK_UINT64*/;
        
        if (avx512TailSize != 0) {
        
        }
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::wstring
	// =================================================================================================================

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u8string
	// =================================================================================================================
	

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u16string
	// =================================================================================================================
	
	template <>
	static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u32string
	// =================================================================================================================

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::AVX512F>>(
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
	static NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::wstring
	// =================================================================================================================

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u8string
	// =================================================================================================================


	template <>
	static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u16string
	// =================================================================================================================

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u32string
	// =================================================================================================================

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::AVX2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::AVX2>>(
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
	static NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::wstring
	// =================================================================================================================

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u8string
	// =================================================================================================================


	template <>
	static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u16string
	// =================================================================================================================

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u32string
	// =================================================================================================================

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::SSE2>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::SSE2>)
	{
		return string;
	}

	// =================================================================================================================
	//														Scalar
	// =================================================================================================================

	// =================================================================================================================
	//													To std::string
	// =================================================================================================================

	template <>
	static NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::None>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::u16string, std::string, CpuFeatureTag<CpuFeature::None>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::u32string, std::string, CpuFeatureTag<CpuFeature::None>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::wstring, std::string, CpuFeatureTag<CpuFeature::None>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::string convertStringImplementation<std::string, std::string, CpuFeatureTag<CpuFeature::None>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::wstring
	// =================================================================================================================

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u8string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u16string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::u32string, std::wstring, CpuFeatureTag<CpuFeature::None>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::wstring convertStringImplementation<std::wstring, std::wstring, CpuFeatureTag<CpuFeature::None>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u8string
	// =================================================================================================================


	template <>
	static NODISCARD std::u8string convertStringImplementation<std::string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::wstring, std::u8string, CpuFeatureTag<CpuFeature::None>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u16string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u32string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u8string convertStringImplementation<std::u8string, std::u8string, CpuFeatureTag<CpuFeature::None>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u16string
	// =================================================================================================================

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::wstring, std::u16string, CpuFeatureTag<CpuFeature::None>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u8string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u32string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u16string convertStringImplementation<std::u16string, std::u16string, CpuFeatureTag<CpuFeature::None>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return string;
	}

	// =================================================================================================================
	//													To std::u32string
	// =================================================================================================================

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
		const std::string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::wstring, std::u32string, CpuFeatureTag<CpuFeature::None>>(
		const std::wstring& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u8string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u16string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
		const std::u16string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return {};
	}

	template <>
	static NODISCARD std::u32string convertStringImplementation<std::u32string, std::u32string, CpuFeatureTag<CpuFeature::None>>(
		const std::u32string& string,
		CpuFeatureTag<CpuFeature::None>)
	{
		return string;
	}
};

__BASE_STRING_NAMESPACE_END

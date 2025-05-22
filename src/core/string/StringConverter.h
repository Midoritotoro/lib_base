#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/core/string/StringConfig.h>

#include <base/core/arch/CpuFeatureTag.h>

#include <base/core/memory/MemoryAllocation.h>
#include <src/core/memory/MemoryUtility.h>

#include <base/core/utility/IntegralTypesConversions.h>
#include <src/core/string/StringConversionOptions.h>

#include <src/core/utility/simd/SimdTailMask.h>


__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_ = DefaultReplacementConversionMode>
class StringConverter {
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
	static NODISCARD _ToType_ convertString(const _FromType_& string);
private:
	template <
		class _FromType_,
		class _ToType_,
		class _Tag_,
		typename = std::enable_if_t<
			IsSupportedString<_FromType_> &&
			IsSupportedString<_ToType_>>>
	static NODISCARD _ToType_ convertStringImplementation(
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
	static NODISCARD std::string convertStringImplementation<std::u8string, std::string, CpuFeatureTag<CpuFeature::AVX512F>>(
		const std::u8string& string,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
        constexpr auto toLimit = MaximumIntegralLimit<std::string::value_type>();

        static_assert(
            _NarrowingConversionBehaviour_::replacementCharacter <= toLimit, 
            "base::core::string::StringConverter::convertStringImplementation: _NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. "); 

        if (string.size() == 0)
            return std::string{};

		const auto fromSizeInBytes = size_t(string.size() * sizeof(char8_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		char* converted = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));

        const void* stringDataStart = string.data();
        const void* stringDataEnd   = stringDataStart;

        const void* stopAt = stringDataStart;

        memory::AdvanceBytes(stopAt, avx512SizeInBytes);
        memory::AdvanceBytes(stringDataEnd, string.size() * sizeof(std::string::value_type));

		if (avx512SizeInBytes != 0) {
            const auto replacementVector = _mm512_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
			const auto lessThanCompare = _mm512_set1_epi8(toLimit);

			do {
				auto loaded = _mm512_loadu_si512(stringDataStart);
				const uint64 comparedGreaterThan = _mm512_cmpgt_epi8_mask(loaded, lessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
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

		const auto avx512TailSize = fromSizeInBytes & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64;
        if (stringDataStart == stringDataEnd)
            return std::string(converted - fromSizeInBytes, fromSizeInBytes);

		
        return std::string(converted - fromSizeInBytes, fromSizeInBytes);

 //       const auto avx2SizeInBytes = fromSizeInBytes & ~size_t(0x1F);
 //       memory::AdvanceBytes(stopAt, avx2SizeInBytes);

 //       if (avx2SizeInBytes != 0) { 
 //           const auto replacementVector = _mm256_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
 //           const auto lessThanCompare = _mm256_set1_epi8(toLimit);

 //           do {
 //               const auto loaded = _mm256_loadu_si256(stringDataStart);
 //               const uint32 comparedGreaterThan = _mm256_cmpgt_epi8_mask(loaded, lessThanCompare);

 //               // Narrowing conversion
 //               if (comparedGreaterThan != 0) {
 //                   switch (_NarrowindConversionBehaviour_::narrowingConversionMode) {
 //                       case NarrowingConversionMode::CustomReplacement:
 //                       case NarrowindConversionMode::DefaultReplacement:
 //                           // If a character in 'loaded' is greater than the threshold (mask bit is 1), it's replaced with
 //                           // 'replacementCharacter'. Otherwise, it's copied from 'loaded'.

 //                           _mm256_mask_storeu_epi8(converted, ~comparedGreaterThan, loaded);
 //                           _mm256_mask_storeu_epi8(converted, comparedGreaterThan, replacementVector);
 //                   }
 //               }
 //               else {
 //                   _mm256_store_si256(converted, loaded);
 //               }

 //               memory::AdvanceBytes(stringDataStart, 32);
 //           } while (stringDataStart != stopAt);
 //       }

 //       if (stringDataStart == stringDataEnd) 
 //           return std::wstring(converted);

 //       const auto sse2SizeInBytes = fromSizeInBytes & ~size_t(0xF);
 //       memory::AdvanceBytes(stopAt, sse2SizeInBytes);

 //       if (sse2SizeInBytes != 0) {
 //           const auto replacementVector = _mm_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
 //           const auto lessThanCompare = _mm_set1_epi8(toLimit);

 //           do {
 //               const auto loaded = _mm_loadu_si128(stringDataStart);
 //               const uint16 comparedGreaterThan = _mm_cmpgt_epi8_mask(loaded, lessThanCompare);

 //               // Narrowing conversion
 //               if (comparedGreaterThan != 0) { 

 //               }
 //               else { 

 //               }
 //               
 //               memory::AdvanceBytes
 //           } while (stringDataStart != stopAt);
 //       }
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
                const auto comparedGreaterThan = _mm512_cmpgt_epi16(loaded, lessThanCompare);
                
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
                    _mm512_store(converted, loaded);
                }

                memory::AdvanceBytes(stringDataStart, 64);
                memory::AdvanceBytes(converted, 64);
            } while (stringDataStart != stringDataEnd);
    
        }

        if (stringDataStart == stringDataEnd)
            return std::string(converted - fromSizeInBytes, fromSizeInBytes);

        const auto avx512TailSize = fromSizeInBytes & AVX512_ALIGNED_TAIL_MASK_UINT64;
        
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
};

__BASE_STRING_NAMESPACE_END

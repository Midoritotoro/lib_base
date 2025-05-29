#pragma once 

#include <base/core/string/StringConfig.h>

#include <base/core/memory/MemoryAllocation.h>
#include <base/core/utility/IntegralTypesConversions.h>

#include <src/core/string/StringConversionOptions.h>

#include <src/core/utility/simd/SimdConstexprHelpers.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>

__BASE_STRING_NAMESPACE_BEGIN

#ifndef base_combine_u32ints_to_u64int
#  define base_combine_u32ints_to_u64int(high, low) ((uint64)high << 32) | (uint64)low
#endif

#ifndef base_combine_u16ints_to_u32int
#  define base_combine_u16ints_to_u32int(high, low) ((uint32)high << 16) | (uint32)low
#endif

#ifndef base_combine_u16ints_to_u64int
#  define base_combine_u16ints_to_u64int(high1, high2, low1, low2)	\
	base_combine_u32ints_to_u64int(									\
		base_combine_u16ints_to_u32int(high1, high2),				\
		base_combine_u16ints_to_u32int(low1, low2))
#endif


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
public:
	constexpr StringConversionResult() noexcept {}

	constexpr inline StringConversionResult(const StringConversionResult& other) noexcept :
		_dataStart(other._dataStart),
		_dataLength(other._dataLength),
		_isNarrowingConversion(other._isNarrowingConversion)
	{}

    constexpr inline StringConversionResult(
        _Char_* dataStart, 
        size_t dataLength, 
        bool isNarrowingConversion
    ) noexcept:
        _dataStart(dataStart),
        _dataLength(dataLength),
        _isNarrowingConversion(isNarrowingConversion)
    {}
	
	constexpr ~StringConversionResult() noexcept {}

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
		typename = std::enable_if_t<
			IsSupportedCharType<_FromChar_> &&
			IsSupportedCharType<_ToChar_>>>
	// Allocates (length * sizeof(_ToChar_)) bytes of memory for conversion from string
	static inline NODISCARD StringConversionResult<_ToChar_> convertString(
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

	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsSupportedCharType<_FromChar_>&&
			IsSupportedCharType<_ToChar_>>>
	// Converts by writing to output->data() (without memory allocation)
	static inline void convertStringStore(
		const _FromChar_* const				string,
		const size_t						length,
		StringConversionResult<_ToChar_>*	output)
	{
		StringConversionResult<_ToChar_> temp;

		if (ProcessorFeatures::AVX512F())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX512F>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::AVX512F>{});

		else if (ProcessorFeatures::AVX2())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX2>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::AVX2>{});

		else if (ProcessorFeatures::SSE2())
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::SSE2>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::SSE2>{});

		else
			temp = convertStringImplementation<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::None>>(
				string, length, output->data(), CpuFeatureTag<CpuFeature::None>{});

		*output = temp;
	}
private:
	using WCharAvx512MaskIntegerType = std::conditional_t<sizeof(wchar_t) == 2,
		uint32,
		std::conditional_t<sizeof(wchar_t) == 4, uint16, void>>;

	using WCharAvxMaskIntegerType = std::conditional_t<sizeof(wchar_t) == 2,
		uint16,
		std::conditional_t<sizeof(wchar_t) == 4, uint8, void>>;

	using WCharSseMaskIntegerType = uint8;

	template <size_t conversionToLimit>
	static constexpr NODISCARD base_vec512i_t wcharSseLessThenCompareVector() noexcept {
		if constexpr (sizeof(wchar_t) == 2)
			return base_constexpr_mm_set1_epu16(
					static_cast<uint16>(conversionToLimit));
		else if constexpr (sizeof(wchar_t) == 4)
			return base_constexpr_mm_set1_epu32(
				static_cast<uint32>(conversionToLimit));
	}

	template <size_t conversionToLimit>
	static constexpr NODISCARD base_vec512i_t wcharAvxLessThenCompareVector() noexcept {
		if constexpr (sizeof(wchar_t) == 2)
			return base_constexpr_mm256_set1_epu16(
					static_cast<uint16>(conversionToLimit));
		else if constexpr (sizeof(wchar_t) == 4)
			return base_constexpr_mm256_set1_epu32(
					static_cast<uint32>(conversionToLimit));
	}

	template <size_t conversionToLimit>
	static constexpr NODISCARD base_vec512i_t wcharAvx512LessThenCompareVector() noexcept {
		if constexpr (sizeof(wchar_t) == 2)
			return base_constexpr_mm512_set1_epu16(
					static_cast<uint16>(conversionToLimit));
		else if constexpr (sizeof(wchar_t) == 4)
			return base_constexpr_mm512_set1_epu32(
					static_cast<uint32>(conversionToLimit));
	}


	template <
		class _FromChar_,
		class _ToChar_,
		class _Tag_,
		typename = std::enable_if_t<
			IsSupportedCharType<_FromChar_> &&
			IsSupportedCharType<_ToChar_>>>
	static always_inline NODISCARD StringConversionResult<_ToChar_> convertStringImplementation(
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
	static NODISCARD always_inline StringConversionResult<char> convertStringImplementation<char8_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char8_t* const	string,
		const size_t			stringLength,
		char*					outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
        constexpr auto toLimit = MaximumIntegralLimit<char>();

		static_assert(
			_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
			"base::core::string::StringConverter::convertStringImplementation"
			"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

		constexpr auto replacementVector = base_constexpr_mm512_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
		constexpr auto lessThanCompare = base_constexpr_mm512_set1_epu8(toLimit);

        if (string == nullptr || stringLength == 0)
            return {};
        
        bool isNarrowingConversion = false;

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char8_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));
		
        const void* stringDataStart = string;
        const void* stringDataEnd   = stringDataStart;

        const void* stopAt = stringDataStart;
		void* outputStringVoidPointer = outputString;

        memory::AdvanceBytes(stopAt, avx512SizeInBytes);
        memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);
        
        if (avx512SizeInBytes != 0) {
			const auto runtimeReplacementVector = base_vec512i_t_pointer_as_m512i(&replacementVector);
			const auto runtimeLessThanCompare = base_vec512i_t_pointer_as_m512i(&lessThanCompare);

			do {
				const auto loaded = _mm512_loadu_si512(stringDataStart);
				const uint64 comparedGreaterThan = _mm512_cmpgt_epu8_mask(loaded, runtimeLessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
                    isNarrowingConversion = true;

					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
						case NarrowingConversionMode::CustomReplacement:
                        case NarrowingConversionMode::DefaultReplacement:
                            // If a character in 'loaded' is greater than the threshold (mask bit is 1), it's replaced with
                            // 'replacementCharacter'. Otherwise, it's copied from 'loaded'.
                            
                            _mm512_mask_storeu_epi8(outputStringVoidPointer, ~comparedGreaterThan, loaded);
							_mm512_mask_storeu_epi8(outputStringVoidPointer, comparedGreaterThan, runtimeReplacementVector);

                            break;
						default:
                            AssertUnreachable();
                            break;
					};
				}
                else
                    _mm512_store_si512(outputStringVoidPointer, loaded);

				memory::AdvanceBytes(stringDataStart, 64);
                memory::AdvanceBytes(outputStringVoidPointer, 64);
			} while (stringDataStart != stopAt);
		}

        if (stringDataStart == stringDataEnd)
            return StringConversionResult<char>(
				outputString - fromSizeInBytes,
                stringLength, isNarrowingConversion);

        return convertStringImplementation<char8_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
			static_cast<const char8_t*>(stringDataStart), fromSizeInBytes - avx512SizeInBytes,
            outputString, CpuFeatureTag<CpuFeature::AVX2>{});
	}
#endif

	template <>
	static NODISCARD always_inline StringConversionResult<char> convertStringImplementation<char16_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char16_t* const   string,
        const size_t            stringLength,
        char*                   outputString,
		CpuFeatureTag<CpuFeature::AVX512F> tag)
	{
		constexpr auto toLimit = MaximumIntegralLimit<char>();

		static_assert(
			_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
			"base::core::string::StringConverter::convertStringImplementation"
			"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

		constexpr auto replacementVector = base_constexpr_mm512_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
		constexpr auto lessThanCompare = base_constexpr_mm512_set1_epi16(toLimit);

		if (string == nullptr || stringLength == 0)
			return {};

		bool isNarrowingConversion = false;

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char16_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		void* outputStringVoidPointer = outputString;
		const void* stopAt = stringDataStart;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);

		if (avx512SizeInBytes != 0) {
			const auto runtimeReplacementVector = base_vec512i_t_pointer_as_m512i(&replacementVector);
			const auto runtimeLessThanCompare = base_vec512i_t_pointer_as_m512i(&lessThanCompare);

			do {
				const auto loaded = _mm512_loadu_si512(stringDataStart);
				const auto loaded2 = _mm512_loadu_si512(memory::UnCheckedToConstChar(stringDataStart) + 64);

				const uint32 comparedGreaterThan = _mm512_cmpgt_epi16_mask(loaded, runtimeLessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
					isNarrowingConversion = true;
					
					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
					case NarrowingConversionMode::CustomReplacement:
					case NarrowingConversionMode::DefaultReplacement:
						// If a character in 'loaded' is greater than the threshold (mask bit is 1), it's replaced with
						// 'replacementCharacter'. Otherwise, it's copied from 'loaded'.
				/*		const auto convertedToChar = _mm512_shuffle_i64x2(
							_mm512_packs_epi16(loaded, loaded2),
							_MM_SHUFFLE(3, 2, 1, 0));
						*/

					//	_mm512_mask_storeu_epi8(outputString, ~comparedGreaterThan, _mm512_cvtepi16_epi8(loaded));
					//	_mm512_mask_storeu_epi8(outputString, comparedGreaterThan, runtimeReplacementVector);

						break;
					default:
						AssertUnreachable();
						break;
					};
				}
				else
					_mm512_store_si512(outputString, loaded);

				memory::AdvanceBytes(stringDataStart, 128);
				memory::AdvanceBytes(outputString, 64);
			} while (stringDataStart != stopAt);
		}

		if (stringDataStart == stringDataEnd)
			return StringConversionResult<char>(
				outputString - fromSizeInBytes,
				stringLength, isNarrowingConversion);

		return convertStringImplementation<char16_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
		    static_cast<const char16_t*>(stringDataStart), stringLength - (avx512SizeInBytes / sizeof(char16_t)),
			outputString, CpuFeatureTag<CpuFeature::AVX2>{});
    }

	template <>
	static NODISCARD always_inline StringConversionResult<char> convertStringImplementation<char32_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char32_t* const   string,
		const size_t            stringLength,
		char*					outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		constexpr auto toLimit = MaximumIntegralLimit<char32_t>();

		constexpr auto replacementVector = base_constexpr_mm512_set1_epi32(_NarrowingConversionBehaviour_::replacementCharacter);
		constexpr auto lessThanCompare = base_constexpr_mm512_set1_epi32(toLimit);

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
			outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		void* outputStringVoidPointer = outputString;
		const void* stopAt = stringDataStart;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);

		if (avx512SizeInBytes != 0) {
			const auto runtimeReplacementVector = base_vec512i_t_pointer_as_m512i(&replacementVector);
			const auto runtimeLessThanCompare = base_vec512i_t_pointer_as_m512i(&lessThanCompare);

			do {
				auto loaded = _mm512_loadu_si512(stringDataStart);
				const uint16 comparedGreaterThan = _mm512_cmpgt_epi32_mask(loaded, runtimeLessThanCompare);

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
					isNarrowingConversion = true;

					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
					case NarrowingConversionMode::CustomReplacement:
					case NarrowingConversionMode::DefaultReplacement:
						// If a character in 'loaded' is greater than the threshold (mask bit is 1), it's replaced with
						// 'replacementCharacter'. Otherwise, it's copied from 'loaded'.

						_mm512_mask_store_epi32(outputStringVoidPointer, ~comparedGreaterThan, loaded);
						_mm512_mask_store_epi32(outputStringVoidPointer, comparedGreaterThan, runtimeReplacementVector);

						break;
					default:
						AssertUnreachable();
						break;
					};
				}
				else
					_mm512_store_si512(outputStringVoidPointer, loaded);

				memory::AdvanceBytes(stringDataStart, 64);
				memory::AdvanceBytes(outputStringVoidPointer, 64);
			} while (stringDataStart != stopAt);
		}

		if (stringDataStart == stringDataEnd)
			return StringConversionResult<char>(
				outputString - fromSizeInBytes,
				stringLength, isNarrowingConversion);

		return convertStringImplementation<char32_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
			static_cast<const char32_t*>(stringDataStart), stringLength - (avx512SizeInBytes / sizeof(char32_t)),
			outputString, CpuFeatureTag<CpuFeature::AVX2>{});
	}

	template <>
	static NODISCARD always_inline StringConversionResult<char> convertStringImplementation<wchar_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const wchar_t* const	string,
		const size_t            stringLength,
		char*					outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		constexpr auto toLimit = MaximumIntegralLimit<char>();

		static_assert(
			_NarrowingConversionBehaviour_::replacementCharacter <= toLimit,
			"base::core::string::StringConverter::convertStringImplementation"
			"_NarrowingConversionBehaviour_::replacementCharacter must be in range [0, toLimit]. ");

		constexpr auto replacementVector = base_constexpr_mm512_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
		constexpr auto lessThanCompare = wcharAvx512LessThenCompareVector<toLimit>();

		if (string == nullptr || stringLength == 0)
			return {};

		bool isNarrowingConversion = false;

		const auto fromSizeInBytes = size_t(stringLength * sizeof(char16_t));
		const auto avx512SizeInBytes = fromSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			outputString = static_cast<char*>(memory::AllocateAligned(fromSizeInBytes, 64));

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		void* outputStringVoidPointer = outputString;
		const void* stopAt = stringDataStart;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);

		if (avx512SizeInBytes != 0) {
			const auto runtimeReplacementVector = base_vec512i_t_pointer_as_m512i(&replacementVector);
			const auto runtimeLessThanCompare = base_vec512i_t_pointer_as_m512i(&lessThanCompare);

			do {
				const auto loaded1 = _mm512_loadu_si512(stringDataStart);
				const auto loaded2 = _mm512_loadu_si512(memory::UnCheckedToConstChar(stringDataStart) + 64);

				uint64 comparedGreaterThan = 0;

				__m512i convertedToCharVector;

				if constexpr (sizeof(wchar_t) == 2) {
					uint32 first64BytesMask = _mm512_cmpgt_epu16_mask(loaded1, runtimeLessThanCompare);
					uint32 second64BytesMask = _mm512_cmpgt_epu16_mask(loaded2, runtimeLessThanCompare);

					comparedGreaterThan = base_combine_u32ints_to_u64int(first64BytesMask, second64BytesMask);

					convertedToCharVector = _mm512_castsi256_si512(_mm512_cvtepi16_epi8(loaded1));
					convertedToCharVector = _mm512_inserti32x8(convertedToCharVector, _mm512_cvtepi16_epi8(loaded2), 1);
				}
				else if constexpr (sizeof(wchar_t) == 4) {
					const auto loaded3 = _mm512_loadu_si512(memory::UnCheckedToConstChar(stringDataStart) + 128);
					const auto loaded4 = _mm512_loadu_si512(memory::UnCheckedToConstChar(stringDataStart) + 192);

					uint16 first64BytesMask = _mm512_cmpgt_epu32_mask(loaded1, runtimeLessThanCompare);
					uint16 second64BytesMask = _mm512_cmpgt_epu32_mask(loaded2, runtimeLessThanCompare);

					uint16 third64BytesMask = _mm512_cmpgt_epu32_mask(loaded1, runtimeLessThanCompare);
					uint16 fourth64BytesMask = _mm512_cmpgt_epu32_mask(loaded2, runtimeLessThanCompare);

					comparedGreaterThan = base_combine_u16ints_to_u64int(
						first64BytesMask, second64BytesMask, third64BytesMask, fourth64BytesMask);

					convertedToCharVector = _mm512_castsi128_si512(_mm512_cvtepi32_epi8(loaded1));

					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded2), 1);
					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded3), 2);
					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded4), 3);
				}

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
					isNarrowingConversion = true;

					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
					case NarrowingConversionMode::CustomReplacement:
					case NarrowingConversionMode::DefaultReplacement:
						// If a character in 'convertedToCharVector' is greater than the threshold (mask bit is 1), it's replaced with
						// 'replacementCharacter'. Otherwise, it's copied from 'convertedToCharVector'.

						_mm512_mask_storeu_epi8(outputStringVoidPointer, ~comparedGreaterThan, convertedToCharVector);
						_mm512_mask_storeu_epi8(outputStringVoidPointer, comparedGreaterThan, runtimeReplacementVector);

						break;
					default:
						AssertUnreachable();
						break;
					};
				}
				else
					_mm512_store_si512(outputStringVoidPointer, convertedToCharVector);
				
				if constexpr (sizeof(wchar_t) == 2)
					memory::AdvanceBytes(stringDataStart, 128);
				else if constexpr (sizeof(wchar_t) == 4)
					memory::AdvanceBytes(stringDataStart, 256);

				memory::AdvanceBytes(outputStringVoidPointer, 64);
			} while (stringDataStart != stopAt);
		}

		if (stringDataStart == stringDataEnd)
			return StringConversionResult<char>(
				outputString - fromSizeInBytes,
				stringLength, isNarrowingConversion);

		return convertStringImplementation<char16_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
			static_cast<const char16_t*>(stringDataStart), stringLength - (avx512SizeInBytes / sizeof(char16_t)),
			outputString, CpuFeatureTag<CpuFeature::AVX2>{});
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
	static NODISCARD always_inline StringConversionResult<wchar_t> convertStringImplementation<char, wchar_t, CpuFeatureTag<CpuFeature::AVX512F>>(
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
			outputString = static_cast<wchar_t*>(memory::AllocateAligned(outputSizeInBytes, 64));

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		const void* stopAt = stringDataStart;
		void* outputStringVoidPointer = outputString;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, fromSizeInBytes);
		
		if (avx512SizeInBytes != 0) {
			do {
				if constexpr (sizeof(wchar_t) == 4) {
					const char* inputStringDataStartChar = memory::UnCheckedToConstChar(stringDataStart);
					char* outputStringCharPointer = memory::UnCheckedToChar(outputStringVoidPointer);

				    _mm512_store_si512(outputStringVoidPointer, _mm512_cvtepi8_epi32(
                        _mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar))));

					_mm512_store_si512(outputStringCharPointer + 64, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 16))));

                    _mm512_store_si512(outputStringCharPointer + 128, _mm512_cvtepi8_epi32(
                        _mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 32))));

                    _mm512_store_si512(outputStringCharPointer + 192, _mm512_cvtepi8_epi32(
                        _mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 48))));

                    memory::AdvanceBytes(outputStringVoidPointer, 256);
				}
				else if constexpr (sizeof(wchar_t) == 2) {
				    _mm512_store_si512(outputStringVoidPointer, _mm512_cvtepi8_epi16(
                        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(stringDataStart))));

                    _mm512_store_si512(memory::UnCheckedToChar(outputStringVoidPointer) + 64, _mm512_cvtepi8_epi16(
                        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(
							memory::UnCheckedToConstChar(stringDataStart) + 32))));

                    memory::AdvanceBytes(outputStringVoidPointer, 128);
				}

				memory::AdvanceBytes(stringDataStart, 64);
			} while (stringDataStart != stopAt);
		}

        if (stringDataStart == stringDataEnd)
            return StringConversionResult<wchar_t>(
                const_cast<wchar_t*>(reinterpret_cast<const wchar_t*>(outputStringVoidPointer)) - stringLength,
				stringLength, false);

        return convertStringImplementation<char, wchar_t, CpuFeatureTag<CpuFeature::AVX2>>(
            static_cast<const char*>(stringDataStart), stringLength - (avx512SizeInBytes / sizeof(char)),
            reinterpret_cast<wchar_t*>(outputStringVoidPointer), CpuFeatureTag<CpuFeature::AVX2>{});
	}

#if __cpp_lib_char8_t
	template <>
	static NODISCARD always_inline StringConversionResult<wchar_t> convertStringImplementation<char8_t, wchar_t, CpuFeatureTag<CpuFeature::AVX512F>>(
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

		if (outputString == nullptr) {
			outputString = static_cast<wchar_t*>(memory::AllocateAligned(outputSizeInBytes, 64));
		}
#if defined(_DfEBUG)
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
                        _mm_loadu_si128(reinterpret_cast<const __m128i*>(stringDataStart))));

                    _mm512_store_si512(outputString + 64, _mm512_cvtepi8_epi32(
                        _mm_loadu_si128(reinterpret_cast<const __m128i*>(
							memory::UnCheckedToConstChar(stringDataStart) + 16))));

                    _mm512_store_si512(outputString + 128, _mm512_cvtepi8_epi32(
                        _mm_loadu_si128(reinterpret_cast<const __m128i*>(
							memory::UnCheckedToConstChar(stringDataStart) + 32))));

                    _mm512_store_si512(outputString + 192, _mm512_cvtepi8_epi32(
                        _mm_loadu_si128(reinterpret_cast<const __m128i*>(
							memory::UnCheckedToConstChar(stringDataStart) + 48))));

                    memory::AdvanceBytes(outputString, 256);  
				}
				else if constexpr (sizeof(wchar_t) == 2) {
				    _mm512_store_si512(outputString, _mm512_cvtepi8_epi16(
                        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(stringDataStart))));

                    _mm512_store_si512(outputString + 64, _mm512_cvtepi8_epi16(
                        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(
							memory::UnCheckedToConstChar(stringDataStart) + 32))));

                    memory::AdvanceBytes(outputString, 128);
				}

				memory::AdvanceBytes(stringDataStart, 64);
			} while (stringDataStart != stopAt);
		}

        if (stringDataStart == stringDataEnd)
            return StringConversionResult<wchar_t>(
                outputString - outputSizeInBytes,
                stringLength, false);

        return convertStringImplementation<char8_t, wchar_t, CpuFeatureTag<CpuFeature::AVX2>>(
            static_cast<const char8_t*>(stringDataStart), stringLength - (avx512SizeInBytes / sizeof(char8_t)),
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
#if __cpp_lib_char8_t
	template <>
	static NODISCARD StringConversionResult<char8_t> convertStringImplementation<char, char8_t, CpuFeatureTag<CpuFeature::AVX512F>>(
		const char* const	string,
		const size_t        stringLength,
		char8_t*			outputString,
		CpuFeatureTag<CpuFeature::AVX512F>)
	{
		if (string == nullptr || stringLength == 0)
			return {};

		const auto inputAndOutputSizeInBytes = size_t(stringLength * sizeof(char));
		const auto avx512SizeInBytes = inputAndOutputSizeInBytes & ~size_t(0x3F);

		if (outputString == nullptr)
			outputString = static_cast<char8_t*>(memory::AllocateAligned(inputAndOutputSizeInBytes, 64));

		const void* stringDataStart = string;
		const void* stringDataEnd = stringDataStart;

		const void* stopAt = stringDataStart;
		void* outputStringVoidPointer = outputString;

		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
		memory::AdvanceBytes(stringDataEnd, inputAndOutputSizeInBytes);

		if (avx512SizeInBytes != 0) {
			do {
				_mm512_storeu_si512(outputStringVoidPointer, _mm512_loadu_si512(stringDataStart));

				memory::AdvanceBytes(stringDataStart, 64);
				memory::AdvanceBytes(outputStringVoidPointer, 64);
			} while (stringDataStart != stopAt);
		}

		if (stringDataStart == stringDataEnd)
			return StringConversionResult<char8_t>(
				const_cast<char8_t*>(reinterpret_cast<const char8_t*>(outputStringVoidPointer)) - stringLength,
				stringLength, false);

		return convertStringImplementation<char, char8_t, CpuFeatureTag<CpuFeature::AVX2>>(
			static_cast<const char*>(stringDataStart), stringLength - (avx512SizeInBytes / sizeof(char)),
			reinterpret_cast<char8_t*>(outputStringVoidPointer), CpuFeatureTag<CpuFeature::AVX2>{});
	}

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
#endif

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

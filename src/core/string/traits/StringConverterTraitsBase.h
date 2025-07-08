#pragma once 

#include <src/core/string/StringConversionOptions.h> 
#include <src/core/utility/simd/SimdConstexprHelpers.h>

#include <base/core/utility/IntegralTypesConversions.h>

__BASE_STRING_NAMESPACE_BEGIN

#if !defined(__BASE_DECLARE_CONVERTER_TRAITS_MAYBE_NARROWING_CONVERSION)
#  define __BASE_DECLARE_CONVERTER_TRAITS_MAYBE_NARROWING_CONVERSION()																							\
	template <typename _FromChar_, typename _ToChar_, typename = std::enable_if_t<IsCompatibleCharType<_FromChar_>::value && IsCompatibleCharType<_ToChar_>::value>>	\
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept { return base::string::StringConverterTraitsBase::maybeNarrowingConversion<_FromChar_, _ToChar_>(); }
#endif

// ====================================================================================================================================

#if __cpp_lib_char8_t && !defined(__BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE)
#define __BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE(bits)														\
	 else if constexpr (std::is_same_v<_ToChar_, char8_t>) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu8(_NarrowingConversionBehaviour_::replacementCharacter));}
#else 
#  define __BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE(bits)
#endif		

#if !defined(__BASE_CONVERTER_SIMD_SET1_FROM_CHAR_TYPE)
#  define __BASE_CONVERTER_SIMD_SET1_FROM_CHAR_TYPE(bits)																															\
	if constexpr (std::is_same_v<_ToChar_, char>) {			 return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter));}		\
	else if constexpr (std::is_same_v<_ToChar_, char16_t>) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter));}	\
	else if constexpr (std::is_same_v<_ToChar_, char32_t>) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter));}	\
	else if constexpr (std::is_same_v<_ToChar_, wchar_t>) {																															\
		if constexpr (sizeof(wchar_t) == 2) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter));}					\
		else if constexpr (sizeof(wchar_t) == 4) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter));}				\
	}																																												\
	__BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE(bits);
#endif

#if !defined(__BASE_DECLARE_CONVERTER_TRAITS_REPLACEMENT_VECTOR)
#  define __BASE_DECLARE_CONVERTER_TRAITS_REPLACEMENT_VECTOR(vectorType, bits)								\
	template <typename _ToChar_> static constexpr NODISCARD vectorType replacementVector() noexcept { __BASE_CONVERTER_SIMD_SET1_FROM_CHAR_TYPE(bits) return {}; }
#endif

// ====================================================================================================================================

#if __cpp_lib_char8_t && !defined(__BASE_CONVERTER_NARROWING_CONVERSION_LIMIT_VECTOR_FOR_CHAR8_TYPE)
#define __BASE_CONVERTER_NARROWING_CONVERSION_LIMIT_VECTOR_FOR_CHAR8_TYPE(bits)														\
	 else if constexpr (std::is_same_v<_FromChar_, char8_t>) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu8(MaximumIntegralLimit<char8_t>()));}
#else 
#  define __BASE_CONVERTER_NARROWING_CONVERSION_LIMIT_VECTOR_FOR_CHAR8_TYPE(bits)
#endif		

#if !defined(__BASE_CONVERTER_NARROWING_CONVERSION_LIMIT_VECTOR_FOR_CHAR_TYPE)
#  define __BASE_CONVERTER_NARROWING_CONVERSION_LIMIT_VECTOR_FOR_CHAR_TYPE(bits)																				\
	if constexpr (std::is_same_v<_FromChar_, char>) {			 return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epi8(MaximumIntegralLimit<char>()));}		\
	else if constexpr (std::is_same_v<_FromChar_, char16_t>) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu16(MaximumIntegralLimit<char16_t>()));}	\
	else if constexpr (std::is_same_v<_FromChar_, char32_t>) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu32(MaximumIntegralLimit<char32_t>()));}	\
	else if constexpr (std::is_same_v<_FromChar_, wchar_t>) {																									\
		if constexpr (sizeof(wchar_t) == 2) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu16(MaximumIntegralLimit<wchar_t>()));}					\
		else if constexpr (sizeof(wchar_t) == 4) { return PP_CAT(PP_CAT(base::base_constexpr_mm, bits), _set1_epu32(MaximumIntegralLimit<wchar_t>()));}				\
	}																																							\
	__BASE_CONVERTER_NARROWING_CONVERSION_LIMIT_VECTOR_FOR_CHAR8_TYPE(bits);
#endif

#if !defined(__BASE_DECLARE_CONVERTER_TRAITS_NARROWING_CONVERSION_LIMIT_VECTOR)
#  define __BASE_DECLARE_CONVERTER_TRAITS_NARROWING_CONVERSION_LIMIT_VECTOR(vectorType, bits)								\
	template <typename _FromChar_> static constexpr NODISCARD vectorType narrowingLimitVector() noexcept { __BASE_CONVERTER_NARROWING_CONVERSION_LIMIT_VECTOR_FOR_CHAR_TYPE(bits) return {};	}
#endif

__BASE_STRING_NAMESPACE_END

#pragma once 

#include <src/core/string/StringConversionOptions.h> 

__BASE_STRING_NAMESPACE_BEGIN

#if !defined(__BASE_DECLARE_CONVERTER_TRAITS_MAYBE_NARROWING_CONVERSION)
#  define __BASE_DECLARE_CONVERTER_TRAITS_MAYBE_NARROWING_CONVERSION(feature)							\
	template <																							\
		typename _FromChar_,																			\
		typename _ToChar_,																				\
		typename = std::enable_if_t<																	\
			IsCompatibleCharType<_FromChar_>::value &&													\	
			IsCompatibleCharType<_ToChar_>::value>>														\
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {								\
		return StringConverterTraitsBase<																\
			feature,																					\
			_NarrowingConversionBehaviour_>::template maybeNarrowingConversion<_FromChar_, _ToChar_>();	\
	}
#endif


#if __cpp_lib_char8_t && !defined(__BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE)
#define __BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE(bits)			\
	 else if constexpr (std::is_same_v<_ToChar_, char8_t>)			\											
		return base_constexpr_mm512_set1_epu8(_NarrowingConversionBehaviour_::replacementCharacter);		
#else 
#  define __BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE(bits)
#endif		

#if !defined(__BASE_CONVERTER_SIMD_SET1_FROM_CHAR_TYPE)
#  define __BASE_CONVERTER_SIMD_SET1_FROM_CHAR_TYPE(bits)																		\
	if constexpr (std::is_same_v<_ToChar_, char>)																				\
		return PP_CAT(PP_CAT(_base_constexpr_mm, bits), _set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter));		\																							
	else if constexpr (std::is_same_v<_ToChar_, char16_t>)																		\
		return PP_CAT(PP_CAT(_base_constexpr_mm, bits), _set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter));		\
	else if constexpr (std::is_same_v<_ToChar_, char32_t>)																		\	
		return PP_CAT(PP_CAT(_base_constexpr_mm, bits), _set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter));		\
	else if constexpr (std::is_same_v<_ToChar_, wchar_t>)																		\
		if constexpr (sizeof(wchar_t) == 2)																						\
			return PP_CAT(PP_CAT(_base_constexpr_mm, bits), _set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter)); \
		else if constexpr (sizeof(wchar_t) == 4)																				\
			return PP_CAT(PP_CAT(_base_constexpr_mm, bits), _set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter));	\
	__BASE_CONVERTER_SIMD_SET1_FROM_CHAR8_TYPE(bits);
#endif



#if !defined(__BASE_DECLARE_CONVERTER_TRAITS_REPLACEMENT_VECTOR)
#  define __BASE_DECLARE_CONVERTER_TRAITS_REPLACEMENT_VECTOR(vectorType, bits)								\
	template <typename _ToChar_>																			\
static constexpr NODISCARD vectorType replacementVector() noexcept {									\
	__BASE_CONVERTER_SIMD_SET1_FROM_CHAR_TYPE(bits);																\
	return {};																								\
}
#endif

template <
	CpuFeature _SimdType_,
	class _NarrowingConversionBehaviour_>
class StringConverterTraitsBase {
public:
	template <typename _ToChar_> static constexpr [[nodiscard]] base_vec128i_t replacementVector() noexcept {
		if constexpr (std::is_same_v<_ToChar_, char>) return _base_constexpr_mm128_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter); \; return {};
	}
	template <
		typename _FromChar_,
		typename _ToChar_>
	// Is data loss possible when converting from _FromChar_ to _ToChar_
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {
		return (MaximumIntegralLimit<_FromChar_>() < MaximumIntegralLimit<_ToChar_>());
	}
};


__BASE_STRING_NAMESPACE_END

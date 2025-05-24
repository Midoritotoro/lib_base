#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/SimdVectorType.h>

#include <src/core/memory/Copy.h>
#include <base/core/utility/CommonMacros.h>

__BASE_NAMESPACE_BEGIN

template <
    typename _Integer_,
    typename ... _Args_,
    typename =  std::enable_if_t<std::is_integral_v<_Integer_>>>
constexpr void SetConstexpr(
    _Integer_*   array,
    _Args_&& ... args) noexcept 
{ 
    constexpr auto arraySizeInBytes = sizeof(_Integer_) * sizeof...(_Args_);

    _Integer_ data[sizeof...(_Args_)];

    for (_Integer_ i = 0; i < sizeof...(_Args_); ++i);
} 

#if defined(CPP_MSVC)

#if !defined(__BASE_CHOOSE__MM_REGISTER)
#  define __BASE_CHOOSE__MM_REGISTER(registerVariableName, registerSizeInBits, integerSizeInBits, sign)                            \
    PP_CAT(PP_CAT(registerVariableName, .), PP_CAT(m, PP_CAT(PP_CAT(registerSizeInBits, i), PP_CAT(_, PP_CAT(sign, integerSizeInBits)))))  
#endif

#if !defined(__BASE_FILL_MM_ARRAY)
#  define __BASE_FILL_MM_ARRAY(variableName, arraySize, value)                          \
        for (int currentElement = 0; currentElement < (arraySize); ++currentElement) {  \
            (variableName)[currentElement] = (value);                                   \
        }                                                                               
#endif

#if !defined(__BASE_INTRIN_CONSTEXPR_SET1)
#  define __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, value, sign)      \
    do {                                                                                                                \
        vectorType vec;                                                                                            \
        __BASE_FILL_MM_ARRAY(__BASE_CHOOSE__MM_REGISTER(vec, registerSizeInBits, integerSizeInBits, sign), arraySize, value);     \
        return vec;                                                                                                     \
    } while(0)
#endif

#if !defined(__BASE_DECLARE_CONSTEXPR_SET1)
#  define __BASE_DECLARE_CONSTEXPR_SET1(name, vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, sign)           \
    template <integerType value>                                                            \
    DECLARE_NOALIAS constexpr NODISCARD vectorType name() noexcept {          \
        __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, value, sign);            \
    }                                                                                                               
#endif

#elif defined(CPP_CLANG) || defined(CPP_GNU) 
// ?

#if !defined(__BASE_INTRIN_CONSTEXPR_SET1)
#  define __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, arraySize, value)   \
    do {                                                                            \
        vectorType vec = { __BASE_REPEAT_N(arraySize, integerType(value)) };        \
        return vec;                                                                 \
    } while(0)
#endif

#if !defined(__BASE_DECLARE_CONSTEXPR_SET1)
#  define __BASE_DECLARE_CONSTEXPR_SET1(name, vectorType, integerType, arraySize)           \
    template <integerType value>                                                            \
    DECLARE_NOALIAS constexpr always_inline NODISCARD vectorType name() noexcept {          \
        __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, arraySize, value);            \
    }                                                                                                               
#endif

#endif


//#if !defined(__BASE_INTRIN_CONSTEXPR_SET)
//#  define __BASE_INTRIN_CONSTEXPR_SET(type, size, ...)      \
//        type vec;
//        
//#endif

// ========================================================================================
//                                          SET1
// ========================================================================================

// XMM



__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi8,  base_vec128i_t, int8, 128, 8, 16, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi16, base_vec128i_t, int16, 128, 16, 8, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi32, base_vec128i_t, int32, 128, 32, 4, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi64, base_vec128i_t, int64, 128, 64, 2, i)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu8,  base_vec128i_t, uint8, 128, 8, 16, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu16, base_vec128i_t, uint16, 128, 16, 8, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu32, base_vec128i_t, uint32, 128, 32, 4, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu64, base_vec128i_t, uint64, 128, 64, 2, u)

#define base_constexpr_mm_set1_epi8(value)  _base_constexpr_mm_set1_epi8<value>()
#define base_constexpr_mm_set1_epi16(value) _base_constexpr_mm_set1_epi16<value>()
#define base_constexpr_mm_set1_epi32(value) _base_constexpr_mm_set1_epi32<value>()
#define base_constexpr_mm_set1_epi64(value) _base_constexpr_mm_set1_epi64<value>()

#define base_constexpr_mm_set1_epu8(value)  _base_constexpr_mm_set1_epu8<value>()
#define base_constexpr_mm_set1_epu16(value) _base_constexpr_mm_set1_epu16<value>()
#define base_constexpr_mm_set1_epu32(value) _base_constexpr_mm_set1_epu32<value>()
#define base_constexpr_mm_set1_epu64(value) _base_constexpr_mm_set1_epu64<value>()

#define base_constexpr_mm_setzero()          base_constexpr_mm_set1_epi64(0)

// YMM

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi8,  base_vec256i_t, int8, 256, 8, 32, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi16, base_vec256i_t, int16, 256, 16, 16, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi32, base_vec256i_t, int32, 256, 32, 8, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi64, base_vec256i_t, int64, 256, 64, 4, i)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu8,  base_vec256i_t, uint8, 256, 8, 32, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu16, base_vec256i_t, uint16, 256, 16, 16, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu32, base_vec256i_t, uint32, 256, 32, 8, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu64, base_vec256i_t, uint64, 256, 64, 4, u)

#define base_constexpr_mm256_set1_epi8(value)   _base_constexpr_mm256_set1_epi8<value>()
#define base_constexpr_mm256_set1_epi16(value)  _base_constexpr_mm256_set1_epi16<value>()
#define base_constexpr_mm256_set1_epi32(value)  _base_constexpr_mm256_set1_epi32<value>()
#define base_constexpr_mm256_set1_epi64(value)  _base_constexpr_mm256_set1_epi64<value>()

#define base_constexpr_mm256_set1_epu8(value)   _base_constexpr_mm256_set1_epu8<value>()
#define base_constexpr_mm256_set1_epu16(value)  _base_constexpr_mm256_set1_epu16<value>()
#define base_constexpr_mm256_set1_epu32(value)  _base_constexpr_mm256_set1_epu32<value>()
#define base_constexpr_mm256_set1_epu64(value)  _base_constexpr_mm256_set1_epu64<value>()

#define base_constexpr_mm256_setzero()          base_constexpr_mm256_set1_epi64(0)

// ZMM 

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi8,  base_vec512i_t, int8, 512, 8, 64, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi16, base_vec512i_t, int16, 512, 16, 32, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi32, base_vec512i_t, int32, 512, 32, 16, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi64, base_vec512i_t, int64, 512, 64, 8, i)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu8,  base_vec512i_t, uint8, 512, 8, 64, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu16, base_vec512i_t, uint16, 512, 16, 32, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu32, base_vec512i_t, uint32, 512, 32, 16, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu64, base_vec512i_t, uint64, 512, 64, 8, u)

#define base_constexpr_mm512_set1_epi8(value)   _base_constexpr_mm512_set1_epi8<value>()
#define base_constexpr_mm512_set1_epi16(value)  _base_constexpr_m512_set1_epi16<value>()
#define base_constexpr_mm512_set1_epi32(value)  _base_constexpr_mm512_set1_epi32<value>()
#define base_constexpr_mm512_set1_epi64(value)  _base_constexpr_mm512_set1_epi64<value>()


#define base_constexpr_mm512_set1_epu8(value)   _base_constexpr_mm512_set1_epu8<value>()
#define base_constexpr_mm512_set1_epu16(value)  _base_constexpr_mm512_set1_epu16<value>()
#define base_constexpr_mm512_set1_epu32(value)  _base_constexpr_mm512_set1_epu32<value>()
#define base_constexpr_mm512_set1_epu64(value)  _base_constexpr_mm512_set1_epu64<value>()

#define base_constexpr_mm512_setzero()          base_constexpr_mm512_set1_epi64(0)


// ========================================================================================
//                                          SET
// ========================================================================================

// XMM 


__BASE_NAMESPACE_END


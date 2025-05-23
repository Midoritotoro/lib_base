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
    
#if !defined(__BASE_INTRIN_CONSTEXPR_SET1)
#  define __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, arraySize, value)   \
    do {                                                                            \
        vectorType vec { __BASE_REPEAT_N(arraySize, value) };                       \
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


//#if !defined(__BASE_INTRIN_CONSTEXPR_SET)
//#  define __BASE_INTRIN_CONSTEXPR_SET(type, size, ...)      \
//        type vec;
//        
//#endif

// ========================================================================================
//                                          SET1
// ========================================================================================

// XMM

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi8,  base_vec128i_t, int8,  16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi16, base_vec128i_t, int16, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi32, base_vec128i_t, int32, 4)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi64, base_vec128i_t, int64, 2)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu8,  base_vec128i_t, uint8,  16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu16, base_vec128i_t, uint16, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu32, base_vec128i_t, uint32, 4)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu64, base_vec128i_t, uint64, 2)


#define base_constexpr_mm_set1_epi8(value)  _base_constexpr_mm_set1_epi8<value>()
#define base_constexpr_mm_set1_epi16(value) _base_constexpr_mm_set1_epi16<value>()
#define base_constexpr_mm_set1_epi32(value) _base_constexpr_mm_set1_epi32<value>()
#define base_constexpr_mm_set1_epi64(value) _base_constexpr_mm_set1_epi64<value>()

#define base_constexpr_mm_set1_epu8(value)  _base_constexpr_mm_set1_epu8<value>()
#define base_constexpr_mm_set1_epu16(value) _base_constexpr_mm_set1_epu16<value>()
#define base_constexpr_mm_set1_epu32(value) _base_constexpr_mm_set1_epu32<value>()
#define base_constexpr_mm_set1_epu64(value) _base_constexpr_mm_set1_epu64<value>()

// YMM

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi8,  base_vec256i_t, int8,  32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi16, base_vec256i_t, int16, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi32, base_vec256i_t, int32, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi64, base_vec256i_t, int64, 4)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu8,  base_vec256i_t, uint8,  32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu16, base_vec256i_t, uint16, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu32, base_vec256i_t, uint32, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu64, base_vec256i_t, uint64, 4)

#define base_constexpr_mm256_set1_epi8(value)   _base_constexpr_mm256_set1_epi8<value>()
#define base_constexpr_mm256_set1_epi16(value)  _base_constexpr_mm256_set1_epi16<value>()
#define base_constexpr_mm256_set1_epi32(value)  _base_constexpr_mm256_set1_epi32<value>()
#define base_constexpr_mm256_set1_epi64(value)  _base_constexpr_mm256_set1_epi64<value>()

#define base_constexpr_mm256_set1_epu8(value)   _base_constexpr_mm256_set1_epu8<value>()
#define base_constexpr_mm256_set1_epu16(value)  _base_constexpr_mm256_set1_epu16<value>()
#define base_constexpr_mm256_set1_epu32(value)  _base_constexpr_mm256_set1_epu32<value>()
#define base_constexpr_mm256_set1_epu64(value)  _base_constexpr_mm256_set1_epu64<value>()

// ZMM 

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi8,  base_vec512i_t, int8,  64)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi16, base_vec512i_t, int16, 32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi32, base_vec512i_t, int32, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi64, base_vec512i_t, int64, 9)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu8,  base_vec512i_t, uint8,  64)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu16, base_vec512i_t, uint16, 32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu32, base_vec512i_t, uint32, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu64, base_vec512i_t, uint64, 8)

#define base_constexpr_mm512_set1_epi8(value)   _base_constexpr_mm512_set1_epi8<value>()
#define base_constexpr_mm512_set1_epi16(value)  _base_constexpr_m512_set1_epi16<value>()
#define base_constexpr_mm512_set1_epi32(value)  _base_constexpr_mm512_set1_epi32<value>()
#define base_constexpr_mm512_set1_epi64(value)  _base_constexpr_mm512_set1_epi64<value>()


#define base_constexpr_mm512_set1_epu8(value)   _base_constexpr_mm512_set1_epu8<value>()
#define base_constexpr_mm512_set1_epu16(value)  _base_constexpr_mm512_set1_epu16<value>()
#define base_constexpr_mm512_set1_epu32(value)  _base_constexpr_mm512_set1_epu32<value>()
#define base_constexpr_mm512_set1_epu64(value)  _base_constexpr_mm512_set1_epu64<value>()


// ========================================================================================
//                                          SET
// ========================================================================================

// XMM 


__BASE_NAMESPACE_END


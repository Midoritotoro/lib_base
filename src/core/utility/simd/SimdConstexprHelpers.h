#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/SimdVectorType.h>

__BASE_NAMESPACE_BEGIN


template <
    typename _Integer_,
    _Integer_ arraySize,
    typename = std::enable_if_t<std::is_integral_v<_Integer_>>>
constexpr always_inline void Set1Constexpr(
    void*       array,
    _Integer_   value) noexcept
{
   /// static_assert(arraySize >= 0 && arraySize <= 64, "base::core::Set1Constexpr: Invalid array size. ");

    _Integer_ data[arraySize];

    for (_Integer_ i = 0; i < arraySize; ++i) 
        data[i] = value;

    std::memcpy(array, data, sizeof(data)); 
}

template <
    typename _Integer_,
    typename ... _Args_,
    typename =  std::enable_if_t<std::is_integral_v<_Integer_>>>
constexpr always_inline void SetConstexpr(
        _Integer_*   array,
        _Args_&& ... args) noexcept 
{ 
    constexpr auto arraySizeInBytes = sizeof(_Integer_) * sizeof...(_Args_)

    _Integer_ data[sizeof...(_Args_)];

    for (_Integer_ i = 0; i < sizeof...(_Args_); ++i)
        ;
} 

#if !defined(__BASE_INTRIN_CONSTEXPR_SET1)
#  define __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, arraySize, value)   \
    do {                                                                            \   
        vectorType vec;                                                             \  
        Set1Constexpr<integerType, arraySize>(vec, value);                          \
        return vec;                                                                 \
    } while (0)
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

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi8(int8 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, int8, 16, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi16(int16 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 8, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi32(int32 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 4, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi64(int64 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 2, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu8(uint8 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 16, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu16(uint16 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 8, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu32(uint32 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 4, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu64(uint64 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 2, value);
}

// YMM

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epi8(int8 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 32, value);
}

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epi16(int16 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 16, value);
}

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epi32(int32 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 8, value);
}

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epi64(int64 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 4, value);
}

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epu8(uint8 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 32, value);
}

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epu16(uint16 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 16, value);
}

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epu32(uint32 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 8, value);
}

constexpr NODISCARD base_vec256i_t _base_constexpr_mm256_set1_epu64(uint64 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec256i_t, 4, value);
}

// ZMM 

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epi8(int8 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 64, value);
}

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epi16(int16 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 32, value);
}

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epi32(int32 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 16, value);
}

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epi64(int64 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 8, value);
}

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epu8(uint8 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 64, value);
}

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epu16(uint16 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 32, value);
}

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epu32(uint32 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 16, value);
}

constexpr NODISCARD base_vec512i_t _base_constexpr_mm512_set1_epu64(uint64 value) noexcept { 
    __BASE_INTRIN_CONSTEXPR_SET1(base_vec512i_t, 8, value);
}

// ========================================================================================
//                                              SET1
// ========================================================================================

// XMM 

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set_epi8(int8 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 16, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi16(int16 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 8, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi32(int32 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 4, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi64(int64 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 2, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu8(uint8 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 16, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu16(uint16 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 8, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu32(uint32 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 4, value);
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu64(uint64 value) noexcept { 
        __BASE_INTRIN_CONSTEXPR_SET1(base_vec128i_t, 2, value);
}


__BASE_NAMESPACE_END


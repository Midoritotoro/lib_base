#pragma once

#include <base/core/arch/SimdHelpers.h>

__BASE_NAMESPACE_BEGIN

#if defined(CPP_MSVC)
#  ifndef DECLARE_INTRIN_TYPE
#    define DECLARE_INTRIN_TYPE __declspec(intrin_type)
#  endif
#else 
#  define DECLARE_INTRIN_TYPE 
#endif

typedef union DECLARE_INTRIN_TYPE __declspec(align(16)) __m128i {
    int8    m128i_i8[16];
    int16   m128i_i16[8];
    int32   m128i_i32[4];
    int64   m128i_i64[2];
    uint8   m128i_u8[16];
    uint16  m128i_u16[8];
    uint32  m128i_u32[4];
    uint64  m128i_u64[2];
} base_vec128i;

typedef struct __declspec(intrin_type) __declspec(align(16)) __m128d {
    double              m128d_f64[2];
} base_vec128d;





__BASE_NAMESPACE_END

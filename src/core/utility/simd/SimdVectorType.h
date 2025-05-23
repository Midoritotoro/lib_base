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

#if defined(CPP_MSVC)

// XMM 

typedef union DECLARE_INTRIN_TYPE SIMD_ALIGNAS(16) base_vec128i_t {
    int8    m128i_i8[16];
    int16   m128i_i16[8];
    int32   m128i_i32[4];
    int64   m128i_i64[2];
    uint8   m128i_u8[16];
    uint16  m128i_u16[8];
    uint32  m128i_u32[4];
    uint64  m128i_u64[2];
} base_vec128i_t;

typedef struct DECLARE_INTRIN_TYPE SIMD_ALIGNAS(16) base_vec128d_t {
    double              m128d_f64[2];
} base_vec128d_t;


typedef union DECLARE_INTRIN_TYPE SIMD_ALIGNAS(16) base_vec128f_t {
    float m128_f32[8];
} base_vec128f_t;

// YMM

typedef union DECLARE_INTRIN_TYPE SIMD_ALIGNAS(32) base_vec256f_t {
    float m256_f32[8];
} base_vec256f_t;

typedef struct DECLARE_INTRIN_TYPE SIMD_ALIGNAS(32) base_vec256d_t {
    double m256d_f64[4];
} base_vec256d_t;

typedef union DECLARE_INTRIN_TYPE SIMD_ALIGNAS(32) base_vec256i_t {
    int8    m256i_i8[32];
    int16   m256i_i16[16];
    int32   m256i_i32[8];
    int64   m256i_i64[4];
    uint8   m256i_u8[32];
    uint16  m256i_u16[16];
    uint32  m256i_u32[8];
    uint64  m256i_u64[4];
} base_vec256i_t;

// ZMM 

typedef union DECLARE_INTRIN_TYPE SIMD_ALIGNAS(64) base_vec512f_t {
    float m512_f32[16];
} base_vec512f_t;

typedef struct DECLARE_INTRIN_TYPE SIMD_ALIGNAS(64) base_vec512d_t {
    double m512d_f64[8];
} base_vec512d_t;

typedef union DECLARE_INTRIN_TYPE SIMD_ALIGNAS(64) base_vec512i_t {
    int8    m512i_i8[64];
    int16   m512i_i16[32];
    int32   m512i_i32[16];
    int64   m512i_i64[8];
    uint8   m512i_u8[64];
    uint16  m512i_u16[32];
    uint32  m512i_u32[16];
    uint64  m512i_u64[8];
} base_vec512i_t;

#elif defined(CPP_CLANG) || defined(CPP_GNU)

typedef float       base_vec128f_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE(16);
typedef double      base_vec128d_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE(16);
typedef long long   base_vec128i_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE(16);

typedef float       base_vec256f_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(32);
typedef double      base_vec256d_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(32);
typedef long long   base_vec256i_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(32);

typedef float       base_vec512f_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(64);
typedef double      base_vec512d_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(64);
typedef long long   base_vec512i_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(64)

#endif

#ifndef BASE_XMM_SIZE_IN_BYTES
#  define BASE_XMM_SIZE_IN_BYTES 16
#endif 

#ifndef BASE_YMM_SIZE_IN_BYTES
#  define BASE_YMM_SIZE_IN_BYTES 32
#endif 

#ifndef BASE_ZMM_SIZE_IN_BYTES
#  define BASE_ZMM_SIZE_IN_BYTES 64
#endif 



#ifndef BASE_XMM_SIZE_IN_BITS
#  define BASE_XMM_SIZE_IN_BITS 128
#endif 

#ifndef BASE_YMM_SIZE_IN_BITS
#  define BASE_YMM_SIZE_IN_BITS 256
#endif 

#ifndef BASE_ZMM_SIZE_IN_BITS
#  define BASE_ZMM_SIZE_IN_BITS 512
#endif 

__BASE_NAMESPACE_END

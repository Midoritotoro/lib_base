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

using base_v2df_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) double;
using base_v2di_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) long long;
using base_v2du_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned long long;
using base_v4si_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) int;
using base_v4su_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned int;
using base_v8hi_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) short;
using base_v8hu_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned short;
using base_v16qi_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) char;
using base_v16qs_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) signed char;
using base_v16qu_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned char;

/* The Intel API is flexible enough that we must allow aliasing with other
   vector types, and their scalar components.  */
using base_vec128i_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(16) long long;
using base_vec128d_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(16) double;
using base_vec128f_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) float;
//


/* Unaligned version of the same types.  */
using base_vec128i_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(16, 1) long long;
using base_vec128d_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(16, 1) double;
using base_vec128f_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(16, 1) float;

using base_x86_double_u_t = BASE_ATTRIBUTE_MAY_ALIAS_ALIGNED(1) double;

/* AVX */
/* Internal data types for implementing the intrinsics.  */
using base_v4df_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) double;
using base_v8sf_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) float;
using base_v4di_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) long long;
using base_v4du_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) unsigned long long;
using base_v8si_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) int;
using base_v8su_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) unsigned int;
using base_v16hi_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) short;
using base_v16hu_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) unsigned short;
using base_v32qi_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) char;
using base_v32qs_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) signed char;
using base_v32qu_t = BASE_ATTRIBUTE_VECTOR_SIZE(32) unsigned char;

/* The Intel API is flexible enough that we must allow aliasing with other
   vector types, and their scalar components.  */
using base_vec256f_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(32) float;
using base_vec256i_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(32) long long;
using base_vec256d_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(32) double;

/* Unaligned version of the same types.  */
using base_vec256f_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(32, 1) float;
using base_vec256i_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(32, 1) long long;
using base_vec256d_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(32, 1) double;

/* AVX512 */
/* Internal data types for implementing the intrinsics.  */
using base_v8df_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) double;
using base_v16sf_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) float;
using base_v8di_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) long long;
using base_v8du_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) unsigned long long;
using base_v16si_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) int;
using base_v16su_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) unsigned int;
using base_v32hi_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) short;
using base_v32hu_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) unsigned short;
using base_v64qi_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) char;
using base_v64qu_t = BASE_ATTRIBUTE_VECTOR_SIZE(64) unsigned char;

/* The Intel API is flexible enough that we must allow aliasing with other
   vector types, and their scalar components.  */
using base_vec512f_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(64) float;
using base_vec512i_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(64) long long;
using base_vec512d_t = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(64) double;


using base_vec512f_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(64, 1) float;
using base_vec512i_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(64, 1) long long;
using base_vec512d_u_t = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(64, 1) double;

using base_v4si_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) int;
using base_v4sf_t = BASE_ATTRIBUTE_VECTOR_SIZE(16) float;

using base_v4su = BASE_ATTRIBUTE_VECTOR_SIZE(16)  unsigned int;

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

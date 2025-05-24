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

//typedef float       base_vec128f_t BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(16);
//typedef double      base_vec128d_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE(16);
//typedef long long   base_vec128i_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE(16);
//
//typedef float       base_vec256f_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(32);
//typedef double      base_vec256d_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(32);
//typedef long long   base_vec256i_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(32);
//
//typedef float       base_vec512f_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(64);
//typedef double      base_vec512d_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(64);
//typedef long long   base_vec512i_t BASE_LLVM_ATTRIBUTE_VECTOR_SIZE_ALIGNED(64);


/* SSE2 */
using __v2df = BASE_ATTRIBUTE_VECTOR_SIZE(16) double;
using __v2di = BASE_ATTRIBUTE_VECTOR_SIZE(16) long long;
using __v2du = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned long long;
using __v4si = BASE_ATTRIBUTE_VECTOR_SIZE(16) int;
using __v4su = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned int;
using __v8hi = BASE_ATTRIBUTE_VECTOR_SIZE(16) short;
using __v8hu = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned short;
using __v16qi = BASE_ATTRIBUTE_VECTOR_SIZE(16) char;
using __v16qs = BASE_ATTRIBUTE_VECTOR_SIZE(16) signed char;
using __v16qu = BASE_ATTRIBUTE_VECTOR_SIZE(16) unsigned char;

/* The Intel API is flexible enough that we must allow aliasing with other
   vector types, and their scalar components.  */
using __m128i = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(16) long long;
using __m128d = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(16) double;

/* Unaligned version of the same types.  */
using __m128i_u = __attribute__((__vector_size__(16), __may_alias__, __aligned__(1))) long long;
using __m128d_u = __attribute__((__vector_size__(16), __may_alias__, __aligned__(1))) double;
using __x86_double_u = __attribute__((__may_alias__, __aligned__(1))) double;

/* AVX */
/* Internal data types for implementing the intrinsics.  */
using __v4df = __attribute__((__vector_size__(32))) double;
using __v8sf = __attribute__((__vector_size__(32))) float;
using __v4di = __attribute__((__vector_size__(32))) long long;
using __v4du = __attribute__((__vector_size__(32))) unsigned long long;
using __v8si = __attribute__((__vector_size__(32))) int;
using __v8su = __attribute__((__vector_size__(32))) unsigned int;
using __v16hi = __attribute__((__vector_size__(32))) short;
using __v16hu = __attribute__((__vector_size__(32))) unsigned short;
using __v32qi = __attribute__((__vector_size__(32))) char;
using __v32qs = __attribute__((__vector_size__(32))) signed char;
using __v32qu = __attribute__((__vector_size__(32))) unsigned char;

/* The Intel API is flexible enough that we must allow aliasing with other
   vector types, and their scalar components.  */
using __m256 = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(32) float;
using __m256i = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(32) long long;
using __m256d = BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(32) double;

/* Unaligned version of the same types.  */
using __m256_u = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(32, 1) float;
using __m256i_u = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(32, 1) long long;
using __m256d_u = BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(32, 1) double;

/* AVX512 */
/* Internal data types for implementing the intrinsics.  */
using __v8df = __attribute__((__vector_size__(64))) double;
using __v16sf = __attribute__((__vector_size__(64))) float;
using __v8di = __attribute__((__vector_size__(64))) long long;
using __v8du = __attribute__((__vector_size__(64))) unsigned long long;
using __v16si = __attribute__((__vector_size__(64))) int;
using __v16su = __attribute__((__vector_size__(64))) unsigned int;
using __v32hi = __attribute__((__vector_size__(64))) short;
using __v32hu = __attribute__((__vector_size__(64))) unsigned short;
using __v64qi = __attribute__((__vector_size__(64))) char;
using __v64qu = __attribute__((__vector_size__(64))) unsigned char;

/* The Intel API is flexible enough that we must allow aliasing with other
   vector types, and their scalar components.  */
using __m512 = __attribute__((__vector_size__(64), __may_alias__)) float;
using __m512i = __attribute__((__vector_size__(64), __may_alias__)) long long;
using __m512d = __attribute__((__vector_size__(64), __may_alias__)) double;



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

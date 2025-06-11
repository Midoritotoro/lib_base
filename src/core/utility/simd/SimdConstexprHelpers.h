#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/SimdVectorType.h>

#include <src/core/memory/Copy.h>
#include <base/core/utility/CommonMacros.h>

__BASE_NAMESPACE_BEGIN

// ========================================================================================
//                                          SET1
// ========================================================================================


#if defined(CPP_MSVC)

#if !defined(__BASE_CHOOSE__MM_REGISTER)
#  define __BASE_CHOOSE__MM_REGISTER(registerVariableName, registerSizeInBits, integerSizeInBits, sign)         \
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
    do {                                                                                                                            \
        vectorType vec;                                                                                                             \
        __BASE_FILL_MM_ARRAY(__BASE_CHOOSE__MM_REGISTER(vec, registerSizeInBits, integerSizeInBits, sign), arraySize, value);       \
        return vec;                                                                                                                 \
    } while(0)
#endif

#if !defined(__BASE_DECLARE_CONSTEXPR_SET1)
#  define __BASE_DECLARE_CONSTEXPR_SET1(name, vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, sign)      \
    template <integerType value>                                                                                                    \
    DECLARE_NOALIAS constexpr NODISCARD always_inline vectorType name() noexcept {                                                  \
        __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, value, sign);       \
    }                                                                                                               
#endif

#elif defined(CPP_CLANG) || defined(CPP_GNU) 

#if !defined(__BASE_INTRIN_CONSTEXPR_SET1)
#  define __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, arraySize, value)   \
    do {                                                                            \
        vectorType vec = { __BASE_REPEAT_N(arraySize, integerType(value)) };        \
        return vec;                                                                 \
    } while(0) 
#endif

#if !defined(__BASE_DECLARE_CONSTEXPR_SET1)
#  define __BASE_DECLARE_CONSTEXPR_SET1(name, vectorType, integerType, arraySize)       \     
    template <integerType value>                                                        \    
    DECLARE_NOALIAS constexpr NODISCARD always_inline vectorType name() noexcept {      \
        __BASE_INTRIN_CONSTEXPR_SET1(vectorType, integerType, arraySize, value);        \ 
    }
#endif

#endif

#if defined(CPP_MSVC)

// XMM

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi8,  base_vec128i_t, int8, 128, 8, 16, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi16, base_vec128i_t, int16, 128, 16, 8, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi32, base_vec128i_t, int32, 128, 32, 4, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi64, base_vec128i_t, int64, 128, 64, 2, i)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu8,  base_vec128i_t, uint8, 128, 8, 16, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu16, base_vec128i_t, uint16, 128, 16, 8, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu32, base_vec128i_t, uint32, 128, 32, 4, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu64, base_vec128i_t, uint64, 128, 64, 2, u)

// YMM

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi8,  base_vec256i_t, int8, 256, 8, 32, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi16, base_vec256i_t, int16, 256, 16, 16, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi32, base_vec256i_t, int32, 256, 32, 8, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi64, base_vec256i_t, int64, 256, 64, 4, i)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu8,  base_vec256i_t, uint8, 256, 8, 32, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu16, base_vec256i_t, uint16, 256, 16, 16, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu32, base_vec256i_t, uint32, 256, 32, 8, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu64, base_vec256i_t, uint64, 256, 64, 4, u)

// ZMM 

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi8,  base_vec512i_t, int8, 512, 8, 64, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi16, base_vec512i_t, int16, 512, 16, 32, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi32, base_vec512i_t, int32, 512, 32, 16, i)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi64, base_vec512i_t, int64, 512, 64, 8, i)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu8,  base_vec512i_t, uint8, 512, 8, 64, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu16, base_vec512i_t, uint16, 512, 16, 32, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu32, base_vec512i_t, uint32, 512, 32, 16, u)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu64, base_vec512i_t, uint64, 512, 64, 8, u)

#elif defined(CPP_GNU) || defined(CPP_CLANG)

// XMM

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi8, base_v16qi_t, int8, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi16, base_v8hi_t, int16, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi32, base_v4si_t, int32, 4)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epi64, base_v2di_t, int64, 2)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu8, base_v16qu_t, uint8, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu16, base_v8hu_t, uint16, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu32, base_v4su_t, uint32, 4)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm_set1_epu64, base_v2du_t, uint64, 2)

// YMM

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi8, base_v32qi_t, int8, 32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi16, base_v16hi_t, int16, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi32, base_v8si_t, int32, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epi64, base_v4di_t, int64, 4)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu8, base_v32qu_t, uint8, 32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu16, base_v16hu_t, uint16, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu32, base_v8su_t, uint32, 8)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm256_set1_epu64, base_v4du_t, uint64, 4)

// ZMM 

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi8, base_v64qi_t, int8, 64)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi16, base_v32hi_t, int16, 32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi32, base_v16si_t, int32, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epi64, base_v8di_t, int64, 8)

__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu8, base_v64qu_t, uint8, 64)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu16, base_v32hu_t, uint16, 32)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu32, base_v16su_t, uint32, 16)
__BASE_DECLARE_CONSTEXPR_SET1(_base_constexpr_mm512_set1_epu64, base_v8du_t, uint64, 8)

#endif

// XMM

#define base_constexpr_mm_set1_epi8(value)  _base_constexpr_mm_set1_epi8<value>()
#define base_constexpr_mm_set1_epi16(value) _base_constexpr_mm_set1_epi16<value>()
#define base_constexpr_mm_set1_epi32(value) _base_constexpr_mm_set1_epi32<value>()
#define base_constexpr_mm_set1_epi64(value) _base_constexpr_mm_set1_epi64<value>()

#define base_constexpr_mm_set1_epu8(value)  _base_constexpr_mm_set1_epu8<value>()
#define base_constexpr_mm_set1_epu16(value) _base_constexpr_mm_set1_epu16<value>()
#define base_constexpr_mm_set1_epu32(value) _base_constexpr_mm_set1_epu32<value>()
#define base_constexpr_mm_set1_epu64(value) _base_constexpr_mm_set1_epu64<value>()

#define base_constexpr_mm_setzero()          base_constexpr_mm_set1_epi64(0)

#define base_constexpr_mm128_set1_epi8(value)  _base_constexpr_mm_set1_epi8<value>()
#define base_constexpr_mm128_set1_epi16(value) _base_constexpr_mm_set1_epi16<value>()
#define base_constexpr_mm128_set1_epi32(value) _base_constexpr_mm_set1_epi32<value>()
#define base_constexpr_mm128_set1_epi64(value) _base_constexpr_mm_set1_epi64<value>()

#define base_constexpr_mm128_set1_epu8(value)  _base_constexpr_mm_set1_epu8<value>()
#define base_constexpr_mm128_set1_epu16(value) _base_constexpr_mm_set1_epu16<value>()
#define base_constexpr_mm128_set1_epu32(value) _base_constexpr_mm_set1_epu32<value>()
#define base_constexpr_mm128_set1_epu64(value) _base_constexpr_mm_set1_epu64<value>()

#define base_constexpr_mm128_setzero()          base_constexpr_mm_set1_epi64(0)

// YMM

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

#define base_constexpr_mm512_set1_epi8(value)   _base_constexpr_mm512_set1_epi8<value>()
#define base_constexpr_mm512_set1_epi16(value)  _base_constexpr_mm512_set1_epi16<value>()
#define base_constexpr_mm512_set1_epi32(value)  _base_constexpr_mm512_set1_epi32<value>()
#define base_constexpr_mm512_set1_epi64(value)  _base_constexpr_mm512_set1_epi64<value>()


#define base_constexpr_mm512_set1_epu8(value)   _base_constexpr_mm512_set1_epu8<value>()
#define base_constexpr_mm512_set1_epu16(value)  _base_constexpr_mm512_set1_epu16<value>()
#define base_constexpr_mm512_set1_epu32(value)  _base_constexpr_mm512_set1_epu32<value>()
#define base_constexpr_mm512_set1_epu64(value)  _base_constexpr_mm512_set1_epu64<value>()

#define base_constexpr_mm512_setzero()          base_constexpr_mm512_set1_epi64(0)



#define base_vec128i_t_pointer_as_m128i(pointer_to_base_vec128i_t)     \
    *((const_cast<__m128i*>((reinterpret_cast<const volatile __m128i*>(pointer_to_base_vec128i_t)))))

#define base_vec256i_t_pointer_as_m256i(pointer_to_base_vec256i_t)  \
    *((const_cast<__m256i*>((reinterpret_cast<const volatile __m256i*>(pointer_to_base_vec256i_t)))))

#define base_vec512i_t_pointer_as_m512i(pointer_to_base_vec512i_t)  \
    *((const_cast<__m512i*>((reinterpret_cast<const volatile __m512i*>(pointer_to_base_vec512i_t)))))

#define base_vec128i_t_pointer_as_m128i_pointer(pointer_to_base_vec128i_t)     \
    ((const_cast<__m128i*>((reinterpret_cast<const volatile __m128i*>(pointer_to_base_vec128i_t)))))

#define base_vec256i_t_pointer_as_m256i_pointer(pointer_to_base_vec256i_t)  \
    ((const_cast<__m256i*>((reinterpret_cast<const volatile __m256i*>(pointer_to_base_vec256i_t)))))

#define base_vec512i_t_pointer_as_m512i_pointer(pointer_to_base_vec512i_t)  \
    ((const_cast<__m512i*>((reinterpret_cast<const volatile __m512i*>(pointer_to_base_vec512i_t)))))



#define base_vec128d_t_pointer_as_m128d(pointer_to_base_vec128d_t)  \
    *((const_cast<__m128d*>((reinterpret_cast<const volatile __m128d*>(pointer_to_base_vec128d_t)))))

#define base_vec256d_t_pointer_as_m256d(pointer_to_base_vec256d_t)  \
    *((const_cast<__m256d*>((reinterpret_cast<const volatile __m256d*>(pointer_to_base_vec256d_t)))))

#define base_vec512d_t_pointer_as_m512d(pointer_to_base_vec512d_t)  \
    *((const_cast<__m512d*>((reinterpret_cast<const volatile __m512d*>(pointer_to_base_vec512d_t)))))

#define base_vec128d_t_pointer_as_m128d_pointer(pointer_to_base_vec128d_t)  \
    ((const_cast<__m128d*>((reinterpret_cast<const volatile __m128d*>(pointer_to_base_vec128d_t)))))

#define base_vec256d_t_pointer_as_m256d_pointer(pointer_to_base_vec256d_t)  \
    ((const_cast<__m256d*>((reinterpret_cast<const volatile __m256d*>(pointer_to_base_vec256d_t)))))

#define base_vec512d_t_pointer_as_m512d_pointer(pointer_to_base_vec512d_t)  \
    ((const_cast<__m512d*>((reinterpret_cast<const volatile __m512d*>(pointer_to_base_vec512d_t)))))



#define base_vec128f_t_pointer_as_m128(pointer_to_base_vec128f_t)   \
    *((const_cast<__m128*>((reinterpret_cast<const volatile __m128*>(pointer_to_base_vec128f_t)))))

#define base_vec256f_t_pointer_as_m256(pointer_to_base_vec256f_t)   \
    *((const_cast<__m256*>((reinterpret_cast<const volatile __m256*>(pointer_to_base_vec256f_t)))))

#define base_vec512f_t_pointer_as_m512(pointer_to_base_vec512f_t)   \
    *((const_cast<__m512*>((reinterpret_cast<const volatile __m512*>(pointer_to_base_vec512f_t)))))


#define base_vec128f_t_pointer_as_m128_pointer(pointer_to_base_vec128f_t)   \
   *((const_cast<__m128*>((reinterpret_cast<const volatile __m128*>(pointer_to_base_vec128f_t)))))

#define base_vec256f_t_pointer_as_m256_pointer(pointer_to_base_vec256f_t)   \
    ((const_cast<__m256*>((reinterpret_cast<const volatile __m256*>(pointer_to_base_vec256f_t)))))

#define base_vec512f_t_pointer_as_m512_pointer(pointer_to_base_vec512f_t)   \
    ((const_cast<__m512*>((reinterpret_cast<const volatile __m512*>(pointer_to_base_vec512f_t)))))

// ========================================================================================
//                                          SET
// ========================================================================================


//template<typename T, T head, T... tail, typename FwdIt>
//constexpr void fill(FwdIt it) {
//    *it = head;
//    fill<T, head, tail...>(++it);
//}
//
//template<class T, T... args>
//constexpr std::array<T, sizeof...(args)> parametersToArray() {
//    std::array<T, sizeof...(args)> a;
//    fill<T, args...>(a.begin());
//    return a;
//};
//
//#if defined(CPP_MSVC)
//
//#if !defined(__BASE_CHOOSE__MM_REGISTER)
//#  define __BASE_CHOOSE__MM_REGISTER(registerVariableName, registerSizeInBits, integerSizeInBits, sign)         \
//    PP_CAT(PP_CAT(registerVariableName, .), PP_CAT(m, PP_CAT(PP_CAT(registerSizeInBits, i), PP_CAT(_, PP_CAT(sign, integerSizeInBits)))))  
//#endif
//
//#if !defined(__BASE_COPY_MM_ARRAY)
//#  define __BASE_COPY_MM_ARRAY(arrayTo, arrayFrom, arraySize)                           \
//        for (int currentElement = 0; currentElement < (arraySize); ++currentElement) {  \
//            (arrayTo)[currentElement] = ((arrayFrom)[currentElement]);                  \
//        }                                                                               
//#endif
//
//#if !defined(__BASE_INTRIN_CONSTEXPR_SET)
//#  define __BASE_INTRIN_CONSTEXPR_SET(vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, sign, values)  \
//    do {                                                                                                                        \
//        static_assert(arraySize == sizeof...(values),                                                                           \
//            "base::core::__BASE_INTRIN_CONSTEXPR_SET: arraySize must be equal to sizeof...(values)");                           \
//        vectorType vec;                                                                                                         \
//        integerType data[arraySize] = { values... };                                                                            \
//        __BASE_COPY_MM_ARRAY(__BASE_CHOOSE__MM_REGISTER(vec, registerSizeInBits, integerSizeInBits, sign), data, arraySize);    \
//        return vec;                                                                                                             \
//    } while(0)
//#endif
//
//#if !defined(__BASE_DECLARE_CONSTEXPR_SET)
//#  define __BASE_DECLARE_CONSTEXPR_SET(name, vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, sign)   \
//    template <integerType ... values>                                                                                           \
//    DECLARE_NOALIAS NODISCARD always_inline vectorType name() noexcept {                                                        \
//        __BASE_INTRIN_CONSTEXPR_SET(vectorType, integerType, registerSizeInBits, integerSizeInBits, arraySize, sign, values);   \
//    }                                                                                                               
//#endif
//
//// XMM
//
//
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epi8, base_vec128i_t, int8, 128, 8, 16, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epi16, base_vec128i_t, int16, 128, 16, 8, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epi32, base_vec128i_t, int32, 128, 32, 4, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epi64, base_vec128i_t, int64, 128, 64, 2, i)
//
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epu8, base_vec128i_t, uint8, 128, 8, 16, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epu16, base_vec128i_t, uint16, 128, 16, 8, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epu32, base_vec128i_t, uint32, 128, 32, 4, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm_set_epu64, base_vec128i_t, uint64, 128, 64, 2, u)
//
//// YMM
//
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epi8, base_vec256i_t, int8, 256, 8, 32, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epi16, base_vec256i_t, int16, 256, 16, 16, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epi32, base_vec256i_t, int32, 256, 32, 8, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epi64, base_vec256i_t, int64, 256, 64, 4, i)
//
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epu8, base_vec256i_t, uint8, 256, 8, 32, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epu16, base_vec256i_t, uint16, 256, 16, 16, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epu32, base_vec256i_t, uint32, 256, 32, 8, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm256_set_epu64, base_vec256i_t, uint64, 256, 64, 4, u)
//
//// ZMM 
//
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epi8, base_vec512i_t, int8, 512, 8, 64, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epi16, base_vec512i_t, int16, 512, 16, 32, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epi32, base_vec512i_t, int32, 512, 32, 16, i)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epi64, base_vec512i_t, int64, 512, 64, 8, i)
//
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epu8, base_vec512i_t, uint8, 512, 8, 64, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epu16, base_vec512i_t, uint16, 512, 16, 32, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epu32, base_vec512i_t, uint32, 512, 32, 16, u)
//__BASE_DECLARE_CONSTEXPR_SET(_base_constexpr_mm512_set_epu64, base_vec512i_t, uint64, 512, 64, 8, u)
//
//
//#elif defined(CPP_CLANG) || defined(CPP_GNU) 
//
//#endif
//
//// XMM
//
//#define base_constexpr_mm_set_epi8(...)  _base_constexpr_mm_set_epi8<__VA_ARGS__>()
//#define base_constexpr_mm_set_epi16(...) _base_constexpr_mm_set_epi16<__VA_ARGS__>()
//#define base_constexpr_mm_set_epi32(...) _base_constexpr_mm_set_epi32<__VA_ARGS__>()
//#define base_constexpr_mm_set_epi64(...) _base_constexpr_mm_set_epi64<__VA_ARGS__>()
//
//#define base_constexpr_mm_set_epu8(...)  _base_constexpr_mm_set_epu8<__VA_ARGS__>()
//#define base_constexpr_mm_set_epu16(...) _base_constexpr_mm_set_epu16<__VA_ARGS__>()
//#define base_constexpr_mm_set_epu32(...) _base_constexpr_mm_set_epu32<__VA_ARGS__>()
//#define base_constexpr_mm_set_epu64(...) _base_constexpr_mm_set_epu64<__VA_ARGS__>()
//
//// YMM
//
//#define base_constexpr_mm256_set_epi8(...)   _base_constexpr_mm256_set_epi8<__VA_ARGS__>()
//#define base_constexpr_mm256_set_epi16(...)  _base_constexpr_mm256_set_epi16<__VA_ARGS__>()
//#define base_constexpr_mm256_set_epi32(...)  _base_constexpr_mm256_set_epi32<__VA_ARGS__>()
//#define base_constexpr_mm256_set_epi64(...)  _base_constexpr_mm256_set_epi64<__VA_ARGS__>()
//
//#define base_constexpr_mm256_set_epu8(...)   _base_constexpr_mm256_set_epu8<__VA_ARGS__>()
//#define base_constexpr_mm256_set_epu16(...)  _base_constexpr_mm256_set_epu16<__VA_ARGS__>()
//#define base_constexpr_mm256_set_epu32(...)  _base_constexpr_mm256_set_epu32<__VA_ARGS__>()
//#define base_constexpr_mm256_set_epu64(...)  _base_constexpr_mm256_set_epu64<__VA_ARGS__>()
//
//// ZMM
//
//#define base_constexpr_mm512_set_epi8(...)   _base_constexpr_mm512_set_epi8<__VA_ARGS__>()
//#define base_constexpr_mm512_set_epi16(...)  _base_constexpr_m512_set_epi16<__VA_ARGS__>()
//#define base_constexpr_mm512_set_epi32(...)  _base_constexpr_mm512_set_epi32<__VA_ARGS__>()
//#define base_constexpr_mm512_set_epi64(...)  _base_constexpr_mm512_set_epi64<__VA_ARGS__>()
//
//
//#define base_constexpr_mm512_set_epu8(...)   _base_constexpr_mm512_set_epu8<__VA_ARGS__>()
//#define base_constexpr_mm512_set_epu16(...)  _base_constexpr_mm512_set_epu16<__VA_ARGS__>()
//#define base_constexpr_mm512_set_epu32(...)  _base_constexpr_mm512_set_epu32<__VA_ARGS__>()
//#define base_constexpr_mm512_set_epu64(...)  _base_constexpr_mm512_set_epu64<__VA_ARGS__>()
//
//
__BASE_NAMESPACE_END

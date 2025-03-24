#pragma once 

#include <base/core/arch/ProcessorFeatures.h>
#include <base/core/memory/MemoryUtility.h>

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)


__BASE_CONTAINER_NAMESPACE_BEGIN

using xmmint    = __m128i;
using ymmint    = __m256i;
using zmmint    = __m512i;

using xmmdouble = __m128d;
using ymmdouble = __m256d;
using zmmdouble = __m512d;

using xmmfloat  = __m128;
using ymmfloat  = __m256;
using zmmfloat  = __m512;

struct MinMaxInt8Bit {
    int8 minimum = 0;
    int8 maximum = 0;
};

struct MinMaxUInt8Bit {
    uint8 minimum = 0;
    uint8 maximum = 0;
};

struct MinMaxInt16Bit {
    int16 minimum = 0;
    int16 maximum = 0;
};

struct MinMaxUInt16Bit {
    uint16 minimum = 0;
    uint16 maximum = 0;
};

struct MinMaxInt32Bit {
    int32 minimum = 0;
    int32 maximum = 0;
};

struct MinMaxUInt32Bit {
    uint32 minimum = 0;
    uint32 maximum = 0;
};

struct MinMaxInt64Bit {
    int64 minimum = 0;
    int64 maximum = 0;
};

struct MinMaxUInt64Bit {
    uint64 minimum = 0;
    uint64 maximum = 0;
};

struct MinMaxIntWordSize {
    registerint minimum = 0;
    registerint maximum = 0;
};

struct MinMaxUIntWordSize {
    registeruint minimum = 0;
    registeruint maximum = 0;
};

struct MinMaxFloat {
    float minimum = 0;
    float maximum = 0;
};

struct MinMaxDouble {
    double minimum = 0;
    double maximum = 0;
};

namespace SSE2 {
    class NumberTraits8Bit {
    public:
        using NumberSigned = MinMaxInt8Bit;
        using NumberUnsigned = MinMaxUInt8Bit;

        using SignedType = int8;
        using UnsignedType = uint8;

        using SimdType = xmmint;

        static constexpr auto MaxInt = BASE_INT8_MAX;
        static constexpr auto MaxUInt = BASE_UINT8_MAX;


        NumberTraits8Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][16] = {
                {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, {} };
            return _mm_sub_epi8(_Val, _mm_load_si128(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm_add_epi8(_Idx, _mm_set1_epi8(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm_sub_epi8(_Idx, _mm_set1_epi8(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm_add_epi8(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi8(_Start);
        }

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_min_epi8(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_max_epi8(_First, _Second);
        }

        NODISCARD static xmmdouble ToDouble(SimdType _Vector) {
            return _mm_cvtepi32_pd(_Vector);
        }

        NODISCARD static xmmfloat ToFloat(SimdType _Vector) {
            return _mm_cvtepi32_ps(_Vector);
        }
    };

    class NumberTraits16Bit {
    public:
        using NumberSigned = MinMaxInt16Bit;
        using NumberUnsigned = MinMaxUInt16Bit;

        using SignedType = int16;
        using UnsignedType = uint16;

        using SimdType = xmmint;

        static constexpr auto MaxInt = BASE_INT16_MAX;
        static constexpr auto MaxUInt = BASE_UINT16_MAX;


        NumberTraits16Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][8] = {
                {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}, {} };

            return _mm_sub_epi16(
                _Val, _mm_load_si128(
                    reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm_add_epi16(_Idx, _mm_set1_epi16(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm_sub_epi16(_Idx, _mm_set1_epi16(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm_add_epi16(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi16(_Start);
        }

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_min_epi16(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_max_epi16(_First, _Second);
        }

        NODISCARD static xmmdouble ToDouble(SimdType _Vector) {
            return _mm_cvtepi32_pd(_Vector);
        }

        NODISCARD static xmmfloat ToFloat(SimdType _Vector) {
            return _mm_cvtepi32_ps(_Vector);
        }
    };

    class NumberTraits32Bit {
    public:
        using NumberSigned = MinMaxInt32Bit;
        using NumberUnsigned = MinMaxUInt32Bit;

        using SignedType = int32;
        using UnsignedType = uint32;

        using SimdType = xmmint;

        static constexpr auto MaxInt = BASE_INT32_MAX;
        static constexpr auto MaxUInt = BASE_UINT32_MAX;


        NumberTraits32Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][4] = {
                { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF }, {} };

            return _mm_sub_epi32(
                _Val, _mm_load_si128(
                    reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm_add_epi32(_Idx, _mm_set1_epi32(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm_sub_epi32(_Idx, _mm_set1_epi32(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm_add_epi32(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(BASE_GUARDOVERFLOW const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi32(_Start);
        }

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_min_epi32(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_max_epi32(_First, _Second);
        }

        NODISCARD static xmmdouble ToDouble(SimdType _Vector) {
            return _mm_cvtepi32_pd(_Vector);
        }

        NODISCARD static xmmfloat ToFloat(SimdType _Vector) {
            return _mm_cvtepi32_ps(_Vector);
        }
    };

    class NumberTraits64Bit {
    public:
        using NumberSigned = MinMaxInt64Bit;
        using NumberUnsigned = MinMaxUInt64Bit;

        using SignedType = int64;
        using UnsignedType = uint64;

        using SimdType = xmmint;

        static constexpr auto MaxInt = BASE_INT64_MAX;
        static constexpr auto MaxUInt = BASE_UINT64_MAX;


        NumberTraits64Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][2] = {
                { 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF }, {} };

            return _mm_sub_epi64(
                _Val, _mm_load_si128(
                    reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm_add_epi64(_Idx, _mm_set1_epi64x(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm_sub_epi64(_Idx, _mm_set1_epi64x(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm_add_epi64(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(BASE_GUARDOVERFLOW const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi64(_Start);
        }

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_min_epi64(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm_max_epi64(_First, _Second);
        }

        NODISCARD static xmmdouble ToDouble(SimdType _Vector) {
            return _mm_cvtepi64_pd(_Vector);
        }

        NODISCARD static xmmfloat ToFloat(SimdType _Vector) {
            return _mm_cvtepi64_ps(_Vector);
        }
    };
} // namespace SSE2

template <typename _Type_>
concept IsValidIntegerType = std::is_integral_v<_Type_> 
    && sizeof(_Type_) >= 8 
    && sizeof(_Type_) <= 64;

class FakeTraits {};


template <
    typename Integer,
    std::enable_if_t<IsValidIntegerType<Integer>>>
inline DECLARE_NOALIAS void __CDECL MaxIntegerImplementation(
    const void* _Start,
    const void* _End,
    Integer* _Out)
{
    
}
template <
    typename Integer,
    std::enable_if_t<IsValidIntegerType<Integer>>>
inline DECLARE_NOALIAS void __CDECL MinIntegerImplementation(
    const void* _Start,
    const void* _End,
    Integer*    _Out)
{

}


inline DECLARE_NOALIAS void __CDECL MinimumElement8Bit(
    const void* _Start,
    const void* _End,
    int8*       _Out)
{
    const auto _Length = memory::ByteLength(_Start, _End);

    if (ProcessorFeatures::AVX2()) {

    }
    else if (ProcessorFeatures::AVX()) {

    }
    else if (ProcessorFeatures::SSE2()) {
            using _Traits_ = SSE2::NumberTraits8Bit;
        }
}

__BASE_CONTAINER_NAMESPACE_END
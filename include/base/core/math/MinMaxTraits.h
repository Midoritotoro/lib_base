#pragma once

#include <base/core/arch/ProcessorFeatures.h>
#include <base/core/memory/MemoryUtility.h>

#include <base/core/math/MinMax.h>

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)

__BASE_NAMESPACE_BEGIN

template <typename _Type_>
concept IsValidIntegerType = std::is_integral_v<_Type_>;
    //&& sizeof(_Type_) >= 1 
    //&& sizeof(_Type_) <= 8;

namespace Scalar {
    template <
        typename Integer,
        typename = std::enable_if_t<IsValidIntegerType<Integer>>>
    bool GetMinimum(
        const void* _Start,
        const void* _End,
        Integer*    Out)
    {
        const auto _Length = memory::ByteLength(_Start, _End);

        sizetype   _Temp = 0;
        sizetype   _TempMin = _Temp;

        if (memory::MemoryCopy(memory::AdressOf(_Temp), _Start, sizeof(Integer)) == false)
            return false;

        for (sizetype i = 0; i < _Length; ++i) {
            if (memory::MemoryCopy(memory::AdressOf(_TempMin), _Start + i, sizeof(Integer)) == false)
                return false;

            if (_TempMin < _Temp)
                _Temp = _TempMin;
        }

        *_Out = _Temp;
        return true;
    }

    template <
        typename Integer,
        typename = std::enable_if_t<IsValidIntegerType<Integer>>>
    bool GetMaximum(
        const void* _Start,
        const void* _End,
        Integer*    _Out)
    {
        const auto _Length = memory::ByteLength(_Start, _End);

        sizetype   _Temp = 0;
        sizetype   _TempMax = _Temp;

        if (memory::MemoryCopy(memory::AdressOf(_Temp), _Start, sizeof(Integer)) == false)
            return false;

        for (sizetype i = 0; i < _Length; ++i) {
            if (memory::MemoryCopy(memory::AdressOf(_TempMax), _Start + i, sizeof(Integer)) == false)
                return false;

            if (_TempMax > _Temp)
                _Temp = _TempMax;
        }

        *_Out = _Temp;
        return true;
    }
} // namespace Scalar

namespace AVX512 {
    class NumberTraits8Bit {
    public:
        using NumberSigned = MinMaxInt8Bit;
        using NumberUnsigned = MinMaxUInt8Bit;

        using SignedType = int8;
        using UnsignedType = uint8;

        using SimdType = zmmint;

        static constexpr auto MaxInt = BASE_INT8_MAX;
        static constexpr auto MaxUInt = BASE_UINT8_MAX;

        NumberTraits8Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][64] = {
                {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, {} };
            return _mm512_sub_epi8(_Val, _mm512_load_si512(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm512_add_epi8(_Idx, _mm512_set1_epi8(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm512_sub_epi8(_Idx, _mm512_set1_epi8(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm512_add_epi8(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi8(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_min_epi8(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_max_epi8(_First, _Second);
        }

        NODISCARD static zmmdouble ToDouble(SimdType _Vector) {
            return _mm512_castsi512_pd(_Vector);
        }

        NODISCARD static zmmfloat ToFloat(SimdType _Vector) {
            return _mm512_castsi512_ps(_Vector);
        }
    };

    class NumberTraits16Bit {
    public:
        using NumberSigned = MinMaxInt16Bit;
        using NumberUnsigned = MinMaxUInt16Bit;

        using SignedType = int16;
        using UnsignedType = uint16;

        using SimdType = zmmint;

        static constexpr auto MaxInt = BASE_INT16_MAX;
        static constexpr auto MaxUInt = BASE_UINT16_MAX;

        NumberTraits16Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][32] = {
                {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}, {} };
            return _mm512_sub_epi16(_Val, _mm512_load_si512(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm512_add_epi16(_Idx, _mm512_set1_epi16(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm512_sub_epi16(_Idx, _mm512_set1_epi16(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm512_add_epi16(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi16(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_min_epi16(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_max_epi16(_First, _Second);
        }

        NODISCARD static zmmdouble ToDouble(SimdType _Vector) {
            return _mm512_castsi512_pd(_Vector);
        }

        NODISCARD static zmmfloat ToFloat(SimdType _Vector) {
            return _mm512_castsi512_ps(_Vector);
        }
    };

    class NumberTraits32Bit {
    public:
        using NumberSigned = MinMaxInt32Bit;
        using NumberUnsigned = MinMaxUInt32Bit;

        using SignedType = int32;
        using UnsignedType = uint32;

        using SimdType = zmmint;

        static constexpr auto MaxInt = BASE_INT32_MAX;
        static constexpr auto MaxUInt = BASE_UINT32_MAX;

        NumberTraits32Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][16] = {
                {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,
                 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,
                 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,
                 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF}, {} };
            return _mm512_sub_epi32(_Val, _mm512_load_si512(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm512_add_epi32(_Idx, _mm512_set1_epi32(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm512_sub_epi32(_Idx, _mm512_set1_epi32(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm512_add_epi32(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi32(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_min_epi32(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_max_epi32(_First, _Second);
        }

        NODISCARD static zmmdouble ToDouble(SimdType _Vector) {
            return _mm512_castsi512_pd(_Vector);
        }

        NODISCARD static zmmfloat ToFloat(SimdType _Vector) {
            return _mm512_castsi512_ps(_Vector);
        }
    };

    class NumberTraits64Bit {
    public:
        using NumberSigned = MinMaxInt64Bit;
        using NumberUnsigned = MinMaxUInt64Bit;

        using SignedType = int64;
        using UnsignedType = uint64;

        using SimdType = zmmint;

        static constexpr auto MaxInt = BASE_INT64_MAX;
        static constexpr auto MaxUInt = BASE_UINT64_MAX;

        NumberTraits64Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][8] = {
                {0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF,
                 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF}, {} };
            return _mm512_sub_epi64(_Val, _mm512_load_si512(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm512_add_epi64(_Idx, _mm512_set1_epi64(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm512_sub_epi64(_Idx, _mm512_set1_epi64(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm512_add_epi64(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi64(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_min_epi64(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm512_max_epi64(_First, _Second);
        }

        NODISCARD static zmmdouble ToDouble(SimdType _Vector) {
            return _mm512_castsi512_pd(_Vector);
        }

        NODISCARD static zmmfloat ToFloat(SimdType _Vector) {
            return _mm512_castsi512_ps(_Vector);
        }
    };
} // AVX512

namespace AVX {
    class NumberTraits8Bit {
    public:
        using NumberSigned = MinMaxInt8Bit;
        using NumberUnsigned = MinMaxUInt8Bit;

        using SignedType = int8;
        using UnsignedType = uint8;

        using SimdType = ymmint;

        static constexpr auto MaxInt = BASE_INT8_MAX;
        static constexpr auto MaxUInt = BASE_UINT8_MAX;

        NumberTraits8Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][32] = {
                {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, {} };
            return _mm256_sub_epi8(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm256_add_epi8(_Idx, _mm256_set1_epi8(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm256_sub_epi8(_Idx, _mm256_set1_epi8(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm256_add_epi8(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm256_loadu_epi8(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_min_epi8(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_max_epi8(_First, _Second);
        }

        NODISCARD static ymmdouble ToDouble(SimdType _Vector) {
            return _mm256_castsi256_pd(_Vector);
        }

        NODISCARD static ymmfloat ToFloat(SimdType _Vector) {
            return _mm256_castsi256_ps(_Vector);
        }
    };

    class NumberTraits16Bit {
    public:
        using NumberSigned = MinMaxInt16Bit;
        using NumberUnsigned = MinMaxUInt16Bit;

        using SignedType = int16;
        using UnsignedType = uint16;

        using SimdType = ymmint;

        static constexpr auto MaxInt = BASE_INT16_MAX;
        static constexpr auto MaxUInt = BASE_UINT16_MAX;

        NumberTraits16Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][16] = {
                {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF }, {} };
            return _mm256_sub_epi16(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm256_add_epi16(_Idx, _mm256_set1_epi16(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm256_sub_epi16(_Idx, _mm256_set1_epi16(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm256_add_epi16(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm256_loadu_epi16(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_min_epi16(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_max_epi16(_First, _Second);
        }

        NODISCARD static ymmdouble ToDouble(SimdType _Vector) {
            return _mm256_castsi256_pd(_Vector);
        }

        NODISCARD static ymmfloat ToFloat(SimdType _Vector) {
            return _mm256_castsi256_ps(_Vector);
        }
    };

    class NumberTraits32Bit {
    public:
        using NumberSigned = MinMaxInt32Bit;
        using NumberUnsigned = MinMaxUInt32Bit;

        using SignedType = int32;
        using UnsignedType = uint32;

        using SimdType = ymmint;

        static constexpr auto MaxInt = BASE_INT32_MAX;
        static constexpr auto MaxUInt = BASE_UINT32_MAX;

        NumberTraits32Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][8] = {
                {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,
                 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF}, {} };
            return _mm256_sub_epi32(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm256_add_epi32(_Idx, _mm256_set1_epi32(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm256_sub_epi32(_Idx, _mm256_set1_epi32(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm256_add_epi32(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm256_loadu_epi32(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_min_epi32(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_max_epi32(_First, _Second);
        }

        NODISCARD static ymmdouble ToDouble(SimdType _Vector) {
            return _mm256_castsi256_pd(_Vector);
        }

        NODISCARD static ymmfloat ToFloat(SimdType _Vector) {
            return _mm256_castsi256_ps(_Vector);
        }
    };

    class NumberTraits64Bit {
    public:
        using NumberSigned = MinMaxInt64Bit;
        using NumberUnsigned = MinMaxUInt64Bit;

        using SignedType = int64;
        using UnsignedType = uint64;

        using SimdType = ymmint;

        static constexpr auto MaxInt = BASE_INT64_MAX;
        static constexpr auto MaxUInt = BASE_UINT64_MAX;

        NumberTraits64Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign)
        {
            SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][4] = {
                {0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF}, {} };
            return _mm256_sub_epi64(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
        }

        static SimdType Increment(SimdType _Idx) {
            return _mm256_add_epi64(_Idx, _mm256_set1_epi64x(1));
        }

        static SimdType Decrement(SimdType _Idx) {
            return _mm256_sub_epi64(_Idx, _mm256_set1_epi64x(1));
        }

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector)
        {
            return _mm256_add_epi64(_FirstVector, _SecondVector);
        }

        /*NODISCARD static SignedType GetByOffset(const sizetype _Offset) {
            return _mm_cvtsi128_si64x()
        }*/

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm256_loadu_epi64(_Start);
        }

        /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
                return _mm_load_epi32(_Start);
            }*/

        NODISCARD static SimdType Min(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_min_epi64(_First, _Second);
        }

        NODISCARD static SimdType Max(
            SimdType _First,
            SimdType _Second)
        {
            return _mm256_max_epi64(_First, _Second);
        }

        NODISCARD static ymmdouble ToDouble(SimdType _Vector) {
            return _mm256_castsi256_pd(_Vector);
        }

        NODISCARD static ymmfloat ToFloat(SimdType _Vector) {
            return _mm256_castsi256_ps(_Vector);
        }
    };
} // namespace AVX

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

    /*    NODISCARD static SimdType LoadAligned(const void* _Start) {
            return _mm_load_epi32(_Start);
        }*/

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

__BASE_NAMESPACE_END
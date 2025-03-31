#pragma once

#include <base/core/arch/ProcessorFeatures.h>
#include <base/core/memory/MemoryUtility.h>

#include <base/core/math/MinMax.h>

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)

__BASE_NAMESPACE_BEGIN

template <typename _Integer_>
static inline constexpr void FillArray(
    _Integer_* _Array,
    sizetype _Size,
    _Integer_ _Val)
{
    for (sizetype i = 0; i < _Size; ++i)
        _Array[i] = _Val;
}

template <typename _Type_>
concept IsValidIntegerType = std::is_integral_v<_Type_>;
    //&& sizeof(_Type_) >= 1 
    //&& sizeof(_Type_) <= 8;

namespace Scalar {
    template <
        typename Integer,
        typename = std::enable_if_t<IsValidIntegerType<Integer>>>
    bool GetMinimum(
        const Integer*  _Start,
        const Integer*  _End,
        Integer*        _Out)
    {
        const auto _Length = memory::ByteLength(_Start, _End);

        sizetype   _Temp = 0;
        sizetype   _TempMin = _Temp;

        if (memory::MemoryCopy(memory::AdressOf(&_Temp), _Start, sizeof(Integer)) == false)
            return false;

        for (sizetype i = 0; i < _Length; ++i) {
            if (memory::MemoryCopy(memory::AdressOf(&_TempMin), _Start + i, sizeof(Integer)) == false)
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
        const Integer*  _Start,
        const Integer*  _End,
        Integer*        _Out)
    {
        const auto _Length = memory::ByteLength(_Start, _End);

        sizetype   _Temp = 0;
        sizetype   _TempMax = _Temp;

        if (memory::MemoryCopy(memory::AdressOf(&_Temp), _Start, sizeof(Integer)) == false)
            return false;

        for (sizetype i = 0; i < _Length; ++i) {
            if (memory::MemoryCopy(memory::AdressOf(&_TempMax), _Start + i, sizeof(Integer)) == false)
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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi8(_Start);
        }

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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi8(_Start);
        }

        template <class _Fn>
        NODISCARD static __m128i HorizontalFunc(const __m128i _Cur, _Fn _Funct) noexcept {
            const __m128i _Shuf_bytes = _mm_set_epi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
            const __m128i _Shuf_words = _mm_set_epi8(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

            __m128i _H_min_val = _Cur;

            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(1, 0, 3, 2)));
            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(2, 3, 0, 1)));

            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi8(_H_min_val, _Shuf_words));
            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi8(_H_min_val, _Shuf_bytes));

            return _H_min_val;
        }

        NODISCARD static __m128i HorizontalMinimum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) { 
                return _mm_min_epi8(_Val1, _Val2); 
            });
        }

        NODISCARD static __m128i HorizontalMaximum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) {
                return _mm_max_epi8(_Val1, _Val2); 
            });
        }

        NODISCARD static __m128i HorizontalMinimumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) {
                return _mm_min_epu8(_Val1, _Val2);
            });
        }

        NODISCARD static __m128i HorizontalMaximumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) { 
                return _mm_max_epu8(_Val1, _Val2); 
            });
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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi16(_Start);
        }

        template <class _Fn>
        NODISCARD static __m128i HorizontalFunc(const __m128i _Cur, _Fn _Funct) noexcept {
            const __m128i _Shuf_words = _mm_set_epi8(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

            __m128i _H_min_val = _Cur;

            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(1, 0, 3, 2)));
            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(2, 3, 0, 1)));

            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi8(_H_min_val, _Shuf_words));

            return _H_min_val;
        }

        NODISCARD static __m128i HorizontalMinimum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) { 
                return _mm_min_epi16(_Val1, _Val2); 
            });
        }

        NODISCARD static __m128i HorizontalMaximum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) {
                return _mm_max_epi16(_Val1, _Val2); 
            });
        }

        NODISCARD static __m128i HorizontalMinimumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) {
                return _mm_min_epu16(_Val1, _Val2); 
            });
        }

        NODISCARD static __m128i HorizontalMaximumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) {
                return _mm_max_epu16(_Val1, _Val2); 
            });
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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi32(_Start);
        }

        template <class _Fn>
        NODISCARD static __m128i HorizontalFunc(const __m128i _Cur, _Fn _Funct) noexcept {
            __m128i _H_min_val = _Cur;

            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(1, 0, 3, 2)));
            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(2, 3, 0, 1)));

            return _H_min_val;
        }

        NODISCARD static __m128i HorizontalMinimum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) {
                return _mm_min_epi32(_Val1, _Val2);
            });
        }

        NODISCARD static __m128i HorizontalMaximum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) { 
                return _mm_max_epi32(_Val1, _Val2); 
            });
        }

        NODISCARD static __m128i HorizontalMinimumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) {
                return _mm_min_epu32(_Val1, _Val2); 
            });
        }

        NODISCARD static __m128i HorizontalMaximumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](__m128i _Val1, __m128i _Val2) { 
                return _mm_max_epu32(_Val1, _Val2); 
            });
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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm_loadu_epi64(_Start);
        }

        template <class _Fn>
        static __m128i HorizontalFunc(const __m128i _Cur, _Fn _Funct) noexcept {
            _Signed_t _H_min_a = _Get_any(_Cur);
            _Signed_t _H_min_b = _Get_any(_mm_bsrli_si128(_Cur, 8));
            if (_Funct(_H_min_b, _H_min_a)) {
                _H_min_a = _H_min_b;
            }
            return _mm_set1_epi64x(_H_min_a);
        }

        static __m128i _H_min(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](_Signed_t _Lhs, _Signed_t _Rhs) { return _Lhs < _Rhs; });
        }

        static __m128i _H_max(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](_Signed_t _Lhs, _Signed_t _Rhs) { return _Lhs > _Rhs; });
        }

        static __m128i _H_min_u(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](_Unsigned_t _Lhs, _Unsigned_t _Rhs) { return _Lhs < _Rhs; });
        }

        static __m128i _H_max_u(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](_Unsigned_t _Lhs, _Unsigned_t _Rhs) { return _Lhs > _Rhs; });
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
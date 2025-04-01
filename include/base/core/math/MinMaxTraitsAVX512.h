#pragma once 

#include <base/core/arch/ProcessorFeatures.h>
#include <base/core/memory/MemoryUtility.h>

#include <base/core/math/MinMax.h>

__BASE_NAMESPACE_BEGIN

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

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_min_epi8(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_max_epi8(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_min_epu8(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_max_epu8(_First, _Second);
        }

        template <class _Fn>
        NODISCARD static SimdType HorizontalFunc(const SimdType _Cur, _Fn _Funct) noexcept {
            SimdType _H_min_val = _Cur;
            return _H_min_val;
        }

        NODISCARD static SimdType HorizontalMinimum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epi8(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epi8(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epu8(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epu8(_Val1, _Val2);
            });
        }

        NODISCARD static SignedType GetAny(const SimdType _Cur) noexcept {
            return static_cast<SignedType>(_mm512_cvtsi512_si32(_Cur));
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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi16(_Start);
        }

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_min_epi16(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_max_epi16(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_min_epu16(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_max_epu16(_First, _Second);
        }

         template <class _Fn>
        NODISCARD static SimdType HorizontalFunc(const SimdType _Cur, _Fn _Funct) noexcept {
            SimdType _H_min_val = _Cur;
            return _H_min_val;
        }

        NODISCARD static SimdType HorizontalMinimum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epi16(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epi16(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epu16(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epu16(_Val1, _Val2);
            });
        }

        NODISCARD static SignedType GetAny(const SimdType _Cur) noexcept {
            return static_cast<SignedType>(_mm512_cvtsi512_si32(_Cur));
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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi32(_Start);
        }

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_min_epi32(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_max_epi32(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_min_epu32(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_max_epu32(_First, _Second);
        }

         template <class _Fn>
        NODISCARD static SimdType HorizontalFunc(const SimdType _Cur, _Fn _Funct) noexcept {
            SimdType _H_min_val = _Cur;
            return _H_min_val;
        }

        NODISCARD static SimdType HorizontalMinimum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epi32(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epi32(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epu32(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epu32(_Val1, _Val2);
            });
        }

        NODISCARD static SignedType GetAny(const SimdType _Cur) noexcept {
            return static_cast<SignedType>(_mm512_cvtsi512_si32(_Cur));
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

        NODISCARD static SimdType Load(const void* _Start) {
            return _mm512_loadu_epi64(_Start);
        }

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_min_epi64(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm512_undefined_epi32()) noexcept
        {
            return _mm512_max_epi64(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_min_epu64(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm512_max_epu64(_First, _Second);
        }

         template <class _Fn>
        NODISCARD static SimdType HorizontalFunc(const SimdType _Cur, _Fn _Funct) noexcept {
            SimdType _H_min_val = _Cur;
            return _H_min_val;
        }

        NODISCARD static SimdType HorizontalMinimum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epi64(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epi64(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_min_epu64(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm512_max_epu64(_Val1, _Val2);
            });
        }

        NODISCARD static SignedType GetAny(const SimdType _Cur) noexcept {
            return static_cast<SignedType>(GetVPos(_Cur, 0));
        }

        NODISCARD static UnsignedType GetVPos(
            const SimdType _Idx,
            const unsigned long _H_pos) noexcept
        {
            UnsignedType _Array[8];
            _mm512_storeu_si512(reinterpret_cast<SimdType*>(&_Array), _Idx);
            return _Array[_H_pos >> 3];
        }

        NODISCARD static zmmdouble ToDouble(SimdType _Vector) {
            return _mm512_castsi512_pd(_Vector);
        }

        NODISCARD static zmmfloat ToFloat(SimdType _Vector) {
            return _mm512_castsi512_ps(_Vector);
        }
    };
} // namespace AVX512

__BASE_NAMESPACE_END

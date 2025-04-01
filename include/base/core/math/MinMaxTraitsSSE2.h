#pragma once 

#include <base/core/arch/ProcessorFeatures.h>
#include <base/core/memory/MemoryUtility.h>

#include <base/core/math/MinMax.h>

__BASE_NAMESPACE_BEGIN

namespace SSE2 {
    class NumberTraits8Bit {
    public:
        using NumberSigned = MinMaxInt8Bit;
        using NumberUnsigned = MinMaxUInt8Bit;

        using SignedType = int8;
        using UnsignedType = uint8;

        using SimdType = xmmint;

        static constexpr auto MaxInt    = BASE_INT8_MAX;
        static constexpr auto MaxUInt = BASE_UINT8_MAX;
        
        static constexpr auto StepSizeInBytes = 16;

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
        NODISCARD static SimdType HorizontalFunc(const SimdType _Cur, _Fn _Funct) noexcept {
            const SimdType _Shuf_bytes = _mm_set_epi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
            const SimdType _Shuf_words = _mm_set_epi8(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

            SimdType _H_min_val = _Cur;

            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(1, 0, 3, 2)));
            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi32(_H_min_val, _MM_SHUFFLE(2, 3, 0, 1)));

            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi8(_H_min_val, _Shuf_words));
            _H_min_val = _Funct(_H_min_val, _mm_shuffle_epi8(_H_min_val, _Shuf_bytes));

            return _H_min_val;
        }

        NODISCARD static SimdType HorizontalMinimum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm_min_epi8(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximum(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm_max_epi8(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm_min_epu8(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SimdType _Val1, SimdType _Val2) {
                return _mm_max_epu8(_Val1, _Val2);
            });
        }

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_min_epi8(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_max_epi8(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_min_epu8(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_max_epu8(_First, _Second);
        }

        NODISCARD static SignedType GetAny(const SimdType _Cur) noexcept {
            return static_cast<SignedType>(_mm_cvtsi128_si32(_Cur));
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

        static constexpr auto StepSizeInBytes = 16;

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

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_min_epi16(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_max_epi16(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_min_epu16(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_max_epu16(_First, _Second);
        }

        NODISCARD static SignedType GetAny(const SimdType _Cur) noexcept {
            return static_cast<SignedType>(_mm_cvtsi128_si32(_Cur));
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

        static constexpr auto StepSizeInBytes = 16;

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

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_min_epi32(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_max_epi32(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_min_epu32(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_max_epu32(_First, _Second);
        }

        NODISCARD static SignedType GetAny(const SimdType _Cur) noexcept {
            return static_cast<SignedType>(_mm_cvtsi128_si32(_Cur));
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

        static constexpr auto StepSizeInBytes = 16;

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
        NODISCARD static __m128i HorizontalFunc(const __m128i _Cur, _Fn _Funct) noexcept {
            SignedType _H_min_a = GetAny(_Cur);
            SignedType _H_min_b = GetAny(_mm_bsrli_si128(_Cur, 8));

            if (_Funct(_H_min_b, _H_min_a))
                _H_min_a = _H_min_b;

            return _mm_set1_epi64x(_H_min_a);
        }

        NODISCARD static __m128i HorizontalMinimum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SignedType _Lhs, SignedType _Rhs) {
                return _Lhs < _Rhs;
            });
        }

        NODISCARD static __m128i HorizontalMaximum(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SignedType _Lhs, SignedType _Rhs) {
                return _Lhs > _Rhs;
            });
        }

        NODISCARD static __m128i HorizontalMinimumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SignedType _Lhs, SignedType _Rhs) {
                return _Lhs < _Rhs;
            });
        }

        NODISCARD static __m128i HorizontalMaximumUnsigned(const __m128i _Cur) noexcept {
            return HorizontalFunc(_Cur, [](SignedType _Lhs, SignedType _Rhs) {
                return _Lhs > _Rhs;
            });
        }

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_min_epi64(_First, _Second);
        }

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm_undefined_si128()) noexcept
        {
            return _mm_max_epi64(_First, _Second);
        }

        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_min_epu64(_First, _Second);
        }

        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept
        {
            return _mm_max_epu64(_First, _Second);
        }

        NODISCARD static SignedType GetAny(const __m128i _Cur) noexcept {
            return static_cast<SignedType>(GetVPos(_Cur, 0));
        }

        NODISCARD static UnsignedType GetVPos(
            const __m128i _Idx, 
            const unsigned long _H_pos) noexcept 
        {
            UnsignedType _Array[2];
            _mm_storeu_si128(reinterpret_cast<__m128i*>(&_Array), _Idx);
            return _Array[_H_pos >> 3];
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
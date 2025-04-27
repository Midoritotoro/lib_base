#pragma once 

#include <base/core/arch/ProcessorFeatures.h>
#include <base/core/memory/MemoryUtility.h>

#include <src/core/utility/simd/MinMax.h>

__BASE_NAMESPACE_BEGIN

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

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits8Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign);

        NODISCARD static SimdType Increment(SimdType _Idx);
        NODISCARD static SimdType Decrement(SimdType _Idx);

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);
        NODISCARD static SimdType Load(const void* _Start);

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;
        
        template <class _Functor_>
        NODISCARD static SimdType HorizontalFunc(
            const SimdType current,
            _Functor_ functor) noexcept;

        NODISCARD static SimdType HorizontalMinimum(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximum(const SimdType current) noexcept;

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        NODISCARD static SignedType GetAny(const SimdType current) noexcept;
        NODISCARD static ymmdouble ToDouble(SimdType _Vector);
        NODISCARD static ymmfloat ToFloat(SimdType _Vector);
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

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits16Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign);

        static SimdType Increment(SimdType _Idx);
        static SimdType Decrement(SimdType _Idx);

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);
        NODISCARD static SimdType Load(const void* _Start);

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        template <class _Functor_>
        NODISCARD static SimdType HorizontalFunc(
            const SimdType  current, 
            _Functor_       functor) noexcept;

        NODISCARD static SimdType HorizontalMinimum(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximum(const SimdType current) noexcept;
        
        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        NODISCARD static SignedType GetAny(const SimdType current) noexcept;
        NODISCARD static ymmdouble ToDouble(SimdType _Vector);
        NODISCARD static ymmfloat ToFloat(SimdType _Vector);
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

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits32Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign);

        static SimdType Increment(SimdType _Idx);
        static SimdType Decrement(SimdType _Idx);

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);

        NODISCARD static SimdType Load(const void* _Start);

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        template <class _Functor_>
        NODISCARD static SimdType HorizontalFunc(
            const SimdType  current,
            _Functor_       functor) noexcept;

        NODISCARD static SimdType HorizontalMinimum(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximum(const SimdType current) noexcept;

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        NODISCARD static SignedType GetAny(const SimdType current) noexcept;
        NODISCARD static ymmdouble ToDouble(SimdType _Vector);
        NODISCARD static ymmfloat ToFloat(SimdType _Vector);
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

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits64Bit() noexcept = default;

        NODISCARD static SimdType SignCorrection(
            const SimdType  _Val,
            const bool      _Sign);

        static SimdType Increment(SimdType _Idx);
        static SimdType Decrement(SimdType _Idx);

        NODISCARD static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);

        NODISCARD static SimdType Load(const void* _Start);

        NODISCARD static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        NODISCARD static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        NODISCARD static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        template <class _Functor_>
        NODISCARD static SimdType HorizontalFunc(
            const SimdType  current, 
            _Functor_       functor) noexcept;

        NODISCARD static SimdType HorizontalMinimum(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximum(const SimdType current) noexcept;

        NODISCARD static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        NODISCARD static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        NODISCARD static SignedType GetAny(const SimdType current) noexcept;
        NODISCARD static UnsignedType GetVPos(
            const SimdType      _Idx,
            const unsigned long _H_pos) noexcept;

        NODISCARD static ymmdouble ToDouble(SimdType _Vector);
        NODISCARD static ymmfloat ToFloat(SimdType _Vector);
    };
} // namespace AVX

__BASE_NAMESPACE_END

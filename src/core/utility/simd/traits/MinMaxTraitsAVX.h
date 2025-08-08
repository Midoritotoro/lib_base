#pragma once 

#include <base/core/arch/ProcessorFeatures.h>
#include <src/core/utility/simd/MinMax.h>

#include <base/core/arch/SimdHelpers.h>


__BASE_NAMESPACE_BEGIN

namespace AVX {
    class NumberTraits8Bit {
    public:
        using NumberSigned = MinMaxInt8Bit;
        using NumberUnsigned = MinMaxUInt8Bit;

        using SignedType = int8;
        using UnsignedType = uint8;

        using SimdType = arch::ymmint;

        static constexpr auto MaxInt = BASE_INT8_MAX;
        static constexpr auto MaxUInt = BASE_UINT8_MAX;

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits8Bit() noexcept = default;

        base_nodiscard static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign);

        base_nodiscard static SimdType Increment(SimdType _Idx);
        base_nodiscard static SimdType Decrement(SimdType _Idx);

        base_nodiscard static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);
        base_nodiscard static SimdType Load(const void* _Start);

        base_nodiscard static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        base_nodiscard static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;
        
        template <class _Functor_>
        base_nodiscard static SimdType HorizontalFunc(
            const SimdType current,
            _Functor_ functor) noexcept;

        base_nodiscard static SimdType HorizontalMinimum(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximum(const SimdType current) noexcept;

        base_nodiscard static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        base_nodiscard static SignedType GetAny(const SimdType current) noexcept;
        base_nodiscard static arch::ymmdouble ToDouble(SimdType _Vector);
        base_nodiscard static arch::ymmfloat ToFloat(SimdType _Vector);
    };

    class NumberTraits16Bit {
    public:
        using NumberSigned = MinMaxInt16Bit;
        using NumberUnsigned = MinMaxUInt16Bit;

        using SignedType = int16;
        using UnsignedType = uint16;

        using SimdType = arch::ymmint;

        static constexpr auto MaxInt = BASE_INT16_MAX;
        static constexpr auto MaxUInt = BASE_UINT16_MAX;

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits16Bit() noexcept = default;

        base_nodiscard static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign);

        static SimdType Increment(SimdType _Idx);
        static SimdType Decrement(SimdType _Idx);

        base_nodiscard static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);
        base_nodiscard static SimdType Load(const void* _Start);

        base_nodiscard static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        base_nodiscard static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        template <class _Functor_>
        base_nodiscard static SimdType HorizontalFunc(
            const SimdType  current, 
            _Functor_       functor) noexcept;

        base_nodiscard static SimdType HorizontalMinimum(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximum(const SimdType current) noexcept;
        
        base_nodiscard static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        base_nodiscard static SignedType GetAny(const SimdType current) noexcept;
        base_nodiscard static arch::ymmdouble ToDouble(SimdType _Vector);
        base_nodiscard static arch::ymmfloat ToFloat(SimdType _Vector);
    };

    class NumberTraits32Bit {
    public:
        using NumberSigned = MinMaxInt32Bit;
        using NumberUnsigned = MinMaxUInt32Bit;

        using SignedType = int32;
        using UnsignedType = uint32;

        using SimdType = arch::ymmint;

        static constexpr auto MaxInt = BASE_INT32_MAX;
        static constexpr auto MaxUInt = BASE_UINT32_MAX;

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits32Bit() noexcept = default;

        base_nodiscard static SimdType SignCorrection(
            const SimdType _Val,
            const bool _Sign);

        static SimdType Increment(SimdType _Idx);
        static SimdType Decrement(SimdType _Idx);

        base_nodiscard static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);

        base_nodiscard static SimdType Load(const void* _Start);

        base_nodiscard static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        base_nodiscard static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        template <class _Functor_>
        base_nodiscard static SimdType HorizontalFunc(
            const SimdType  current,
            _Functor_       functor) noexcept;

        base_nodiscard static SimdType HorizontalMinimum(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximum(const SimdType current) noexcept;

        base_nodiscard static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        base_nodiscard static SignedType GetAny(const SimdType current) noexcept;
        base_nodiscard static arch::ymmdouble ToDouble(SimdType _Vector);
        base_nodiscard static arch::ymmfloat ToFloat(SimdType _Vector);
    };

    class NumberTraits64Bit {
    public:
        using NumberSigned = MinMaxInt64Bit;
        using NumberUnsigned = MinMaxUInt64Bit;

        using SignedType = int64;
        using UnsignedType = uint64;

        using SimdType = arch::ymmint;

        static constexpr auto MaxInt = BASE_INT64_MAX;
        static constexpr auto MaxUInt = BASE_UINT64_MAX;

        static constexpr auto StepSizeInBytes = 32;

        NumberTraits64Bit() noexcept = default;

        base_nodiscard static SimdType SignCorrection(
            const SimdType  _Val,
            const bool      _Sign);

        static SimdType Increment(SimdType _Idx);
        static SimdType Decrement(SimdType _Idx);

        base_nodiscard static SimdType Add(
            SimdType _FirstVector,
            SimdType _SecondVector);

        base_nodiscard static SimdType Load(const void* _Start);

        base_nodiscard static SimdType Minimum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MinimumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        base_nodiscard static SimdType Maximum(
            const SimdType _First,
            const SimdType _Second,
            SimdType = _mm256_undefined_si256()) noexcept;
        base_nodiscard static SimdType MaximumUnsigned(
            const SimdType _First,
            const SimdType _Second) noexcept;

        template <class _Functor_>
        base_nodiscard static SimdType HorizontalFunc(
            const SimdType  current, 
            _Functor_       functor) noexcept;

        base_nodiscard static SimdType HorizontalMinimum(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximum(const SimdType current) noexcept;

        base_nodiscard static SimdType HorizontalMinimumUnsigned(const SimdType current) noexcept;
        base_nodiscard static SimdType HorizontalMaximumUnsigned(const SimdType current) noexcept;

        base_nodiscard static SignedType GetAny(const SimdType current) noexcept;
        base_nodiscard static UnsignedType GetVPos(
            const SimdType      _Idx,
            const unsigned long _H_pos) noexcept;

        base_nodiscard static arch::ymmdouble ToDouble(SimdType _Vector);
        base_nodiscard static arch::ymmfloat ToFloat(SimdType _Vector);
    };
} // namespace AVX

__BASE_NAMESPACE_END

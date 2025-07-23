#include <src/core/utility/simd/traits/MinMaxTraitsAVX512.h>

__BASE_NAMESPACE_BEGIN

namespace AVX512 {
    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::SignCorrection(
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

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Increment(SimdType _Idx) {
        return _mm512_add_epi8(_Idx, _mm512_set1_epi8(1));
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Decrement(SimdType _Idx) {
        return _mm512_sub_epi8(_Idx, _mm512_set1_epi8(1));
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm512_add_epi8(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Load(const void* _Start) {
        return _mm512_loadu_epi8(_Start);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_min_epi8(_First, _Second);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_max_epi8(_First, _Second);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_min_epu8(_First, _Second);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_max_epu8(_First, _Second);
    }

    template <class _Functor_>
    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalFunc(
        const SimdType  current,
        _Functor_       functor) noexcept
    {
        SimdType horizontalMinimumValues = current;

        SimdType shuffled = _mm512_shuffle_i32x4(
            horizontalMinimumValues,
            horizontalMinimumValues, _MM_SHUFFLE(1, 0, 3, 2));

        horizontalMinimumValues = functor(horizontalMinimumValues, shuffled);

        shuffled = _mm512_shuffle_i32x4(
            horizontalMinimumValues,
            horizontalMinimumValues, _MM_SHUFFLE(2, 3, 0, 1));

        horizontalMinimumValues = functor(horizontalMinimumValues, shuffled);

        return horizontalMinimumValues;
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_min_epi8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_max_epi8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_min_epu8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_max_epu8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SignedType NumberTraits8Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm512_cvtsi512_si32(current));
    }

    NODISCARD arch::zmmdouble NumberTraits8Bit::ToDouble(SimdType _Vector) {
        return _mm512_castsi512_pd(_Vector);
    }

    NODISCARD arch::zmmfloat NumberTraits8Bit::ToFloat(SimdType _Vector) {
        return _mm512_castsi512_ps(_Vector);
    }


    // ===========================================================
    // ===========================================================
    // ===========================================================


    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::SignCorrection(
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

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Increment(SimdType _Idx) {
        return _mm512_add_epi16(_Idx, _mm512_set1_epi16(1));
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Decrement(SimdType _Idx) {
        return _mm512_sub_epi16(_Idx, _mm512_set1_epi16(1));
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm512_add_epi16(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Load(const void* _Start) {
        return _mm512_loadu_epi16(_Start);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_min_epi16(_First, _Second);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_max_epi16(_First, _Second);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_min_epu16(_First, _Second);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_max_epu16(_First, _Second);
    }

    template <class _Functor_>
    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalFunc(
        const SimdType  current,
        _Functor_       functor) noexcept 
    {
        SimdType horizontalMinimumValues = current;
        return horizontalMinimumValues;
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_min_epi16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_max_epi16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm512_min_epu16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm512_max_epu16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SignedType NumberTraits16Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm512_cvtsi512_si32(current));
    }

    NODISCARD arch::zmmdouble NumberTraits16Bit::ToDouble(SimdType _Vector) {
        return _mm512_castsi512_pd(_Vector);
    }

    NODISCARD arch::zmmfloat NumberTraits16Bit::ToFloat(SimdType _Vector) {
        return _mm512_castsi512_ps(_Vector);
    }


    // ===========================================================
    // ===========================================================
    // ===========================================================


    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::SignCorrection(
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

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Increment(SimdType _Idx) {
        return _mm512_add_epi32(_Idx, _mm512_set1_epi32(1));
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Decrement(SimdType _Idx) {
        return _mm512_sub_epi32(_Idx, _mm512_set1_epi32(1));
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm512_add_epi32(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Load(const void* _Start) {
        return _mm512_loadu_epi32(_Start);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_min_epi32(_First, _Second);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_max_epi32(_First, _Second);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_min_epu32(_First, _Second);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_max_epu32(_First, _Second);
    }

    template <class _Functor_>
    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalFunc(
        const SimdType  current,
        _Functor_       functor) noexcept
    {
        SimdType horizontalMinimumValues = current;
        return horizontalMinimumValues;
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_min_epi32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm512_max_epi32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_min_epu32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm512_max_epu32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SignedType NumberTraits32Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm512_cvtsi512_si32(current));
    }

    NODISCARD arch::zmmdouble NumberTraits32Bit::ToDouble(SimdType _Vector) {
        return _mm512_castsi512_pd(_Vector);
    }

    NODISCARD arch::zmmfloat NumberTraits32Bit::ToFloat(SimdType _Vector) {
        return _mm512_castsi512_ps(_Vector);
    }


    // ===========================================================
    // ===========================================================
    // ===========================================================


    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][8] = {
            {0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF,
                0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF}, {} };
        return _mm512_sub_epi64(_Val, _mm512_load_si512(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Increment(SimdType _Idx) {
        return _mm512_add_epi64(_Idx, _mm512_set1_epi64(1));
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Decrement(SimdType _Idx) {
        return _mm512_sub_epi64(_Idx, _mm512_set1_epi64(1));
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm512_add_epi64(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Load(const void* _Start) {
        return _mm512_loadu_epi64(_Start);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_min_epi64(_First, _Second);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm512_max_epi64(_First, _Second);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_min_epu64(_First, _Second);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm512_max_epu64(_First, _Second);
    }

    template <class _Functor_>
    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalFunc(
        const SimdType  current, 
        _Functor_       functor) noexcept 
    {
        SimdType horizontalMinimumValues = current;
        return horizontalMinimumValues;
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_min_epi64(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm512_max_epi64(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm512_min_epu64(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm512_max_epu64(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits64Bit::SignedType NumberTraits64Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(GetVPos(current, 0));
    }

    NODISCARD NumberTraits64Bit::UnsignedType NumberTraits64Bit::GetVPos(
        const SimdType _Idx,
        const unsigned long _H_pos) noexcept
    {
        UnsignedType _Array[8];
        _mm512_storeu_si512(reinterpret_cast<SimdType*>(&_Array), _Idx);
        return _Array[_H_pos >> 3];
    }

    NODISCARD arch::zmmdouble NumberTraits64Bit::ToDouble(SimdType _Vector) {
        return _mm512_castsi512_pd(_Vector);
    }

    NODISCARD arch::zmmfloat NumberTraits64Bit::ToFloat(SimdType _Vector) {
        return _mm512_castsi512_ps(_Vector);
    }
} // namespace AVX512

__BASE_NAMESPACE_END

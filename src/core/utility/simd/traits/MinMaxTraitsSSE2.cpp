#include <src/core/utility/simd/traits/MinMaxTraitsSSE2.h>

__BASE_NAMESPACE_BEGIN

namespace SSE2 {
    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][16] = {
            {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, {} };
        return _mm_sub_epi8(_Val, _mm_load_si128(
            reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Increment(SimdType _Idx) {
        return _mm_add_epi8(_Idx, _mm_set1_epi8(1));
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Decrement(SimdType _Idx) {
        return _mm_sub_epi8(_Idx, _mm_set1_epi8(1));
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm_add_epi8(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Load(const void* _Start) {
        return _mm_loadu_epi8(_Start);
    }

    template <class _Functor_>
    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalFunc(
        const SimdType  current,
        _Functor_       functor) noexcept
    {
        const SimdType shuffledBytes = _mm_set_epi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
        const SimdType shuffledWords = _mm_set_epi8(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

        SimdType horizontalMinimumValues = current;

        horizontalMinimumValues = functor(
            horizontalMinimumValues, 
            _mm_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(1, 0, 3, 2)));

        horizontalMinimumValues = functor(
            horizontalMinimumValues, 
            _mm_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(2, 3, 0, 1)));

        horizontalMinimumValues = functor(
            horizontalMinimumValues, 
            _mm_shuffle_epi8(
                horizontalMinimumValues, shuffledWords));

        horizontalMinimumValues = functor(
            horizontalMinimumValues,
            _mm_shuffle_epi8(
                horizontalMinimumValues, shuffledBytes));

        return horizontalMinimumValues;
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm_min_epi8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm_max_epi8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm_min_epu8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm_max_epu8(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_min_epi8(_First, _Second);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_max_epi8(_First, _Second);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_min_epu8(_First, _Second);
    }

    NODISCARD NumberTraits8Bit::SimdType NumberTraits8Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_max_epu8(_First, _Second);
    }

    NODISCARD NumberTraits8Bit::SignedType NumberTraits8Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm_cvtsi128_si32(current));
    }

    NODISCARD arch::xmmdouble NumberTraits8Bit::ToDouble(SimdType _Vector) {
        return _mm_cvtepi32_pd(_Vector);
    }

    NODISCARD arch::xmmfloat NumberTraits8Bit::ToFloat(SimdType _Vector) {
        return _mm_cvtepi32_ps(_Vector);
    }


    // ======================================================================== 
    // ======================================================================== 
    // ======================================================================== 
    
  
    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][8] = {
            {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}, {} };

        return _mm_sub_epi16(
            _Val, _mm_load_si128(
                reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Increment(SimdType _Idx) {
        return _mm_add_epi16(_Idx, _mm_set1_epi16(1));
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Decrement(SimdType _Idx) {
        return _mm_sub_epi16(_Idx, _mm_set1_epi16(1));
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm_add_epi16(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Load(const void* _Start) {
        return _mm_loadu_epi16(_Start);
    }

    template <class _Functor_>
    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalFunc(
        const SimdType  current, 
        _Functor_       functor) noexcept
    {
        const SimdType shuffledWords = _mm_set_epi8(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

        SimdType horizontalMinimumValues = current;

        horizontalMinimumValues = functor(
            horizontalMinimumValues, 
            _mm_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(1, 0, 3, 2)));

        horizontalMinimumValues = functor(
            horizontalMinimumValues, 
            _mm_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(2, 3, 0, 1)));

        horizontalMinimumValues = functor(
            horizontalMinimumValues,
            _mm_shuffle_epi8(
                horizontalMinimumValues, shuffledWords));

        return horizontalMinimumValues;
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm_min_epi16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm_max_epi16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm_min_epu16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm_max_epu16(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_min_epi16(_First, _Second);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_max_epi16(_First, _Second);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_min_epu16(_First, _Second);
    }

    NODISCARD NumberTraits16Bit::SimdType NumberTraits16Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_max_epu16(_First, _Second);
    }

    NODISCARD NumberTraits16Bit::SignedType NumberTraits16Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm_cvtsi128_si32(current));
    }

    NODISCARD arch::xmmdouble NumberTraits16Bit::ToDouble(SimdType _Vector) {
        return _mm_cvtepi32_pd(_Vector);
    }

    NODISCARD arch::xmmfloat NumberTraits16Bit::ToFloat(SimdType _Vector) {
        return _mm_cvtepi32_ps(_Vector);
    }


    // ======================================================================== 
    // ======================================================================== 
    // ======================================================================== 


    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][4] = {
            { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF }, {} };

        return _mm_sub_epi32(
            _Val, _mm_load_si128(
                reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Increment(SimdType _Idx) {
        return _mm_add_epi32(_Idx, _mm_set1_epi32(1));
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Decrement(SimdType _Idx) {
        return _mm_sub_epi32(_Idx, _mm_set1_epi32(1));
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm_add_epi32(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Load(const void* _Start) {
        return _mm_loadu_epi32(_Start);
    }

    template <class _Functor_>
    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalFunc(
        const SimdType  current,
        _Functor_       functor) noexcept
    {
        SimdType horizontalMinimumValues = current;

        horizontalMinimumValues = functor(
            horizontalMinimumValues,
            _mm_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(1, 0, 3, 2)));

        horizontalMinimumValues = functor(
            horizontalMinimumValues,
            _mm_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(2, 3, 0, 1)));

        return horizontalMinimumValues;
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm_min_epi32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm_max_epi32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm_min_epu32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm_max_epu32(firstValue, secondValue);
        });
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_min_epi32(_First, _Second);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_max_epi32(_First, _Second);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_min_epu32(_First, _Second);
    }

    NODISCARD NumberTraits32Bit::SimdType NumberTraits32Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_max_epu32(_First, _Second);
    }

    NODISCARD NumberTraits32Bit::SignedType NumberTraits32Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm_cvtsi128_si32(current));
    }

    NODISCARD arch::xmmdouble NumberTraits32Bit::ToDouble(SimdType _Vector) {
        return _mm_cvtepi32_pd(_Vector);
    }

    NODISCARD arch::xmmfloat NumberTraits32Bit::ToFloat(SimdType _Vector) {
        return _mm_cvtepi32_ps(_Vector);
    }


    // ======================================================================== 
    // ======================================================================== 
    // ======================================================================== 


    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        SIMD_ALIGNAS(MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT) static constexpr UnsignedType _Sign_corrections[2][2] = {
            { 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF }, {} };

        return _mm_sub_epi64(
            _Val, _mm_load_si128(
                reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Increment(SimdType _Idx) {
        return _mm_add_epi64(_Idx, _mm_set1_epi64x(1));
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Decrement(SimdType _Idx) {
        return _mm_sub_epi64(_Idx, _mm_set1_epi64x(1));
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm_add_epi64(_FirstVector, _SecondVector);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Load(const void* _Start) {
        return _mm_loadu_epi64(_Start);
    }

    template <class _Functor_>
    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalFunc(
        const SimdType  current, 
        _Functor_       functor) noexcept 
    {
        SignedType horizontalMinimumFirst = GetAny(current);
        SignedType horizontalMinimumSecond = GetAny(_mm_bsrli_si128(current, 8));

        if (functor(horizontalMinimumSecond, horizontalMinimumFirst))
            horizontalMinimumFirst = horizontalMinimumSecond;

        return _mm_set1_epi64x(horizontalMinimumFirst);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SignedType _Lhs,
            SignedType _Rhs)
        {
            return _Lhs < _Rhs;
        });
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SignedType _Lhs,
            SignedType _Rhs) 
        {
            return _Lhs > _Rhs;
        });
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SignedType _Lhs, 
            SignedType _Rhs)
        {
            return _Lhs < _Rhs;
        });
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SignedType _Lhs, 
            SignedType _Rhs) 
        {
            return _Lhs > _Rhs;
        });
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_min_epi64(_First, _Second);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm_max_epi64(_First, _Second);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_min_epu64(_First, _Second);
    }

    NODISCARD NumberTraits64Bit::SimdType NumberTraits64Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm_max_epu64(_First, _Second);
    }

    NODISCARD NumberTraits64Bit::SignedType NumberTraits64Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(GetVPos(current, 0));
    }

    NODISCARD NumberTraits64Bit::UnsignedType NumberTraits64Bit::GetVPos(
        const SimdType _Idx,
        const unsigned long _H_pos) noexcept
    {
        UnsignedType _Array[2];
        _mm_storeu_si128(reinterpret_cast<SimdType*>(&_Array), _Idx);
        return _Array[_H_pos >> 3];
    }

    NODISCARD arch::xmmdouble NumberTraits64Bit::ToDouble(SimdType _Vector) {
        return _mm_cvtepi64_pd(_Vector);
    }

    NODISCARD arch::xmmfloat NumberTraits64Bit::ToFloat(SimdType _Vector) {
        return _mm_cvtepi64_ps(_Vector);
    }
} // namespace SSE2

__BASE_NAMESPACE_END
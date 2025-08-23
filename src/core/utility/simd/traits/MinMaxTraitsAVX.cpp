#include <src/core/utility/simd/traits/MinMaxTraitsAVX.h>

__BASE_NAMESPACE_BEGIN

namespace AVX {
      NumberTraits8Bit::SimdType NumberTraits8Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        base_aligned_type(32) static constexpr UnsignedType _Sign_corrections[2][32] = {
            {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, {} };
        return _mm256_sub_epi8(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::Increment(SimdType _Idx) {
        return _mm256_add_epi8(_Idx, _mm256_set1_epi8(1));
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::Decrement(SimdType _Idx) {
        return _mm256_sub_epi8(_Idx, _mm256_set1_epi8(1));
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm256_add_epi8(_FirstVector, _SecondVector);
    }

      NumberTraits8Bit::SimdType Load(const void* _Start) {
        return _mm256_loadu_epi8(_Start);
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_min_epi8(_First, _Second);
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_max_epi8(_First, _Second);
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_min_epu8(_First, _Second);
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_max_epu8(_First, _Second);
    }


    template <class _Functor_>
      NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalFunc(
        const SimdType  current, 
        _Functor_       functor) noexcept 
    {
        SimdType _H_min_val = current;
        return _H_min_val;
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm256_min_epi8(firstValue, secondValue);
        });
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [](
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm256_max_epi8(firstValue, secondValue);
        });
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [](
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm256_min_epu8(firstValue, secondValue);
        });
    }

      NumberTraits8Bit::SimdType NumberTraits8Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [](
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm256_max_epu8(firstValue, secondValue);
        });
    }

      NumberTraits8Bit::SignedType NumberTraits8Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm256_cvtsi256_si32(current));
    }

    
    // ===============================================================================
    // ===============================================================================
    // ===============================================================================


      NumberTraits16Bit::SimdType NumberTraits16Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        base_aligned_type(32)  static constexpr UnsignedType _Sign_corrections[2][16] = {
            {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
                0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF }, {} };
        return _mm256_sub_epi16(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::Increment(SimdType _Idx) {
        return _mm256_add_epi16(_Idx, _mm256_set1_epi16(1));
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::Decrement(SimdType _Idx) {
        return _mm256_sub_epi16(_Idx, _mm256_set1_epi16(1));
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm256_add_epi16(_FirstVector, _SecondVector);
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::Load(const void* _Start) {
        return _mm256_loadu_epi16(_Start);
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_min_epi16(_First, _Second);
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_max_epi16(_First, _Second);
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_min_epu16(_First, _Second);
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_max_epu16(_First, _Second);
    }


    template <class _Functor_>
      NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalFunc(
        const SimdType  current, 
        _Functor_       functor) noexcept
    {
        const SimdType shuffledBytes = _mm256_set_epi16(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
        const SimdType shuffledWords = _mm256_set_epi16(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

        SimdType horizontalMinimumValues = current;

        horizontalMinimumValues = functor(
            horizontalMinimumValues, 
            _mm256_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(1, 0, 3, 2)));

        horizontalMinimumValues = functor(
            horizontalMinimumValues,
            _mm256_shuffle_epi32(
                horizontalMinimumValues, _MM_SHUFFLE(2, 3, 0, 1)));

        return horizontalMinimumValues;
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm256_min_epi16(firstValue, secondValue);
        });
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm256_max_epi16(firstValue, secondValue);
        });
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm256_min_epu16(firstValue, secondValue);
        });
    }

      NumberTraits16Bit::SimdType NumberTraits16Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [](
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm256_max_epu16(firstValue, secondValue);
        });
    }

      NumberTraits16Bit::SignedType NumberTraits16Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm256_cvtsi256_si32(current));
    }

    // ===============================================================================
    // ===============================================================================
    // ===============================================================================

    
      NumberTraits32Bit::SimdType NumberTraits32Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        base_aligned_type(32) static constexpr UnsignedType _Sign_corrections[2][8] = {
            {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,
                0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF}, {} };
        return _mm256_sub_epi32(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

    NumberTraits32Bit::SimdType NumberTraits32Bit::Increment(SimdType _Idx) {
        return _mm256_add_epi32(_Idx, _mm256_set1_epi32(1));
    }

    NumberTraits32Bit::SimdType NumberTraits32Bit::Decrement(SimdType _Idx) {
        return _mm256_sub_epi32(_Idx, _mm256_set1_epi32(1));
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm256_add_epi32(_FirstVector, _SecondVector);
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::Load(const void* _Start) {
        return _mm256_loadu_epi32(_Start);
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_min_epi32(_First, _Second);
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_max_epi32(_First, _Second);
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_min_epu32(_First, _Second);
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_max_epu32(_First, _Second);
    }


    template <class _Functor_>
      NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalFunc(
        const SimdType  current,
        _Functor_       functor) noexcept 
    {
        SimdType horizontalMinimumValues = current;

        SimdType shuffled = _mm256_shuffle_epi32(horizontalMinimumValues, _MM_SHUFFLE(1, 0, 3, 2));
        horizontalMinimumValues = functor(horizontalMinimumValues, shuffled);

        shuffled = _mm256_shuffle_epi32(horizontalMinimumValues, _MM_SHUFFLE(2, 3, 0, 1));
        horizontalMinimumValues = functor(horizontalMinimumValues, shuffled);

        return horizontalMinimumValues;
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm256_min_epi32(firstValue, secondValue);
         });
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue) 
        {
            return _mm256_max_epi32(firstValue, secondValue);
        });
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm256_min_epu32(firstValue, secondValue);
        });
    }

      NumberTraits32Bit::SimdType NumberTraits32Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm256_max_epu32(firstValue, secondValue);
        });
    }

      NumberTraits32Bit::SignedType NumberTraits32Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(_mm256_cvtsi256_si32(current));
    }


    // ===============================================================================
    // ===============================================================================
    // ===============================================================================


      NumberTraits64Bit::SimdType NumberTraits64Bit::SignCorrection(
        const SimdType _Val,
        const bool _Sign)
    {
        base_aligned_type(32) static constexpr UnsignedType _Sign_corrections[2][4] = {
            {0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF}, {} };
        return _mm256_sub_epi64(_Val, _mm256_load_si256(reinterpret_cast<const SimdType*>(_Sign_corrections[_Sign])));
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::Increment(SimdType _Idx) {
        return _mm256_add_epi64(_Idx, _mm256_set1_epi64x(1));
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::Decrement(SimdType _Idx) {
        return _mm256_sub_epi64(_Idx, _mm256_set1_epi64x(1));
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::Add(
        SimdType _FirstVector,
        SimdType _SecondVector)
    {
        return _mm256_add_epi64(_FirstVector, _SecondVector);
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::Load(const void* _Start) {
        return _mm256_loadu_epi64(_Start);
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::Minimum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_min_epi64(_First, _Second);
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::Maximum(
        const SimdType _First,
        const SimdType _Second,
        SimdType) noexcept
    {
        return _mm256_max_epi64(_First, _Second);
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::MinimumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_min_epu64(_First, _Second);
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::MaximumUnsigned(
        const SimdType _First,
        const SimdType _Second) noexcept
    {
        return _mm256_max_epu64(_First, _Second);
    }

    template <class _Functor_>
      NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalFunc(
        const SimdType  current, 
        _Functor_       functor) noexcept 
    {
        SimdType horizontalMinimumValues = current;

        horizontalMinimumValues = functor(
            horizontalMinimumValues, 
            _mm256_shuffle_epi32(horizontalMinimumValues,
                _MM_SHUFFLE(1, 0, 3, 2)));

        horizontalMinimumValues = functor(
            horizontalMinimumValues,
            _mm256_shuffle_epi32(horizontalMinimumValues,
                _MM_SHUFFLE(2, 3, 0, 1)));

        return horizontalMinimumValues;
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMinimum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm256_min_epi64(firstValue, secondValue);
        });
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMaximum(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue, 
            SimdType secondValue)
        {
            return _mm256_max_epi64(firstValue, secondValue);
        });
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMinimumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue) 
        {
            return _mm256_min_epu64(firstValue, secondValue);
        });
    }

      NumberTraits64Bit::SimdType NumberTraits64Bit::HorizontalMaximumUnsigned(const SimdType current) noexcept {
        return HorizontalFunc(current, [] (
            SimdType firstValue,
            SimdType secondValue)
        {
            return _mm256_max_epu64(firstValue, secondValue);
        });
    }

      NumberTraits64Bit::SignedType NumberTraits64Bit::GetAny(const SimdType current) noexcept {
        return static_cast<SignedType>(GetVPos(current, 0));
    }

      NumberTraits64Bit::UnsignedType NumberTraits64Bit::GetVPos(
        const SimdType      _Idx,
        const unsigned long _H_pos) noexcept
    {
        UnsignedType _Array[4];
        _mm256_storeu_si256(reinterpret_cast<SimdType*>(&_Array), _Idx);

        return _Array[_H_pos >> 3];
    }

} // namespace AVX

__BASE_NAMESPACE_END

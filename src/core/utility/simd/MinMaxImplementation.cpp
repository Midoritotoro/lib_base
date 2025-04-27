#pragma once 

#include <base/core/utility/simd/SimdAlgorithm.h>


__BASE_NAMESPACE_BEGIN


template <
    class _Traits_,
    bool _Sign_,
    typename _Integer_,
    typename = std::enable_if_t<IsValidIntegerType<_Integer_>>>
inline DECLARE_NOALIAS void __CDECL MaximumIntegerImplementation(
    const void* _Start,
    const void* _End,
    _Integer_* _Out)
{
    const auto _Length = memory::ByteLength(_Start, _End);

    if (_Length <= 0)
        return;

    if (_Length < _Traits_::StepSizeInBytes) {
        Scalar::GetMaximum(_Start, _End, _Out);
        return;
    }

    using _Ty = std::conditional_t<_Sign_,
        typename _Traits_::SignedType,
        typename _Traits_::UnsignedType>;

    _Ty _MaximumValue;

    constexpr bool _SignCorrection = sizeof(_Ty) == 8 && !_Sign_;
    const sizetype _ByteSize = _Length & ~sizetype{ _Traits_::StepSizeInBytes - 1 };

    const void* _StopAt = _Start;
    memory::AdvanceBytes(_StopAt, _ByteSize);

    auto _CurrentValues = _Traits_::Load(_Start);

    if constexpr (_SignCorrection)
        _CurrentValues = _Traits_::SignCorrection(_CurrentValues, false);

    auto _CurrentMaximumValues = _CurrentValues;

    for (;;) {
        memory::AdvanceBytes(_Start, _Traits_::StepSizeInBytes);

        if (_Start != _StopAt) {
            _CurrentValues = _Traits_::Load(_Start);

            if constexpr (_SignCorrection)
                _CurrentValues = _Traits_::SignCorrection(_CurrentValues, false);

            if constexpr (_Sign_ || _SignCorrection)
                _CurrentMaximumValues = _Traits_::Maximum(_CurrentMaximumValues, _CurrentValues);
            else
                _CurrentMaximumValues = _Traits_::MaximumUnsigned(_CurrentMaximumValues, _CurrentValues);
        }
        else {
            // Конец

            if constexpr (_Sign_ || _SignCorrection) {
                const auto _HorizontalMaximum =
                    _Traits_::HorizontalMaximum(_CurrentMaximumValues); // Отсортированный вектор с максимальным элементом на первой позиции
                _MaximumValue = _Traits_::GetAny(_HorizontalMaximum);
            }
            else {
                const auto _HorizontalMaximum =
                    _Traits_::HorizontalMaximumUnsigned(_CurrentMaximumValues); // Отсортированный вектор с максимальным элементом на первой позиции
                _MaximumValue = _Traits_::GetAny(_HorizontalMaximum);
            }

            if constexpr (_SignCorrection) {
                constexpr _Ty _Correction = _Ty{ 1 } << (sizeof(_Ty) * 8 - 1);
                _MaximumValue += _Correction;
            }

            break;
        }
    }

    *_Out = _MaximumValue;
}

template <
    class _Traits_,
    bool _Sign_,
    typename _Integer_,
    typename = std::enable_if_t<IsValidIntegerType<_Integer_>>>
inline DECLARE_NOALIAS void __CDECL MinimumIntegerImplementation(
    const void* _Start,
    const void* _End,
    _Integer_* _Out)
{
    const auto _Length = memory::ByteLength(_Start, _End);

    if (_Length <= 0)
        return;

    if (_Length < 16) {
        Scalar::GetMinimum(_Start, _End, _Out);
        return;
    }

    using _Ty = std::conditional_t<_Sign_,
        typename _Traits_::SignedType,
        typename _Traits_::UnsignedType>;

    _Ty _MinimumValue;

    constexpr bool _SignCorrection = sizeof(_Ty) == 8 && !_Sign_;
    const sizetype _ByteSize = _Length & ~sizetype{ _Traits_::StepSizeInBytes - 1 };

    const void* _StopAt = _Start;
    memory::AdvanceBytes(_StopAt, _ByteSize);

    auto _CurrentValues = _Traits_::Load(_Start);

    if constexpr (_SignCorrection)
        _CurrentValues = _Traits_::SignCorrection(_CurrentValues, false);

    auto _CurrentMinimumValues = _CurrentValues;

    for (;;) {
        memory::AdvanceBytes(_Start, _Traits_::StepSizeInBytes);

        if (_Start != _StopAt) {
            _CurrentValues = _Traits_::Load(_Start);

            if constexpr (_SignCorrection)
                _CurrentValues = _Traits_::SignCorrection(_CurrentValues, false);

            if constexpr (_Sign_ || _SignCorrection)
                _CurrentMinimumValues = _Traits_::Minimum(_CurrentMinimumValues, _CurrentValues);
            else
                _CurrentMinimumValues = _Traits_::MinimumUnsigned(_CurrentMinimumValues, _CurrentValues);
        }
        else {
            // Конец

            if constexpr (_Sign_ || _SignCorrection) {
                const auto _HorizontalMinimum =
                    _Traits_::HorizontalMinimum(_CurrentMinimumValues); // Отсортированный вектор с минимальным элементом на первой позиции
                _MinimumValue = _Traits_::GetAny(_HorizontalMinimum);
            }
            else {
                const auto _HorizontalMinimum =
                    _Traits_::HorizontalMinimumUnsigned(_CurrentMinimumValues); // Отсортированный вектор с минимальным элементом на первой позиции
                _MinimumValue = _Traits_::GetAny(_HorizontalMinimum);
            }

            if constexpr (_SignCorrection) {
                constexpr _Ty _Correction = _Ty{ 1 } << (sizeof(_Ty) * 8 - 1);
                _MinimumValue += _Correction;
            }

            break;
        }
    }

    *_Out = _MinimumValue;
}


DECLARE_NOALIAS void __CDECL MinimumElement8Bit(
    const void* _Start,
    const void* _End,
    int8* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
            AVX512::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
            AVX::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
            SSE2::NumberTraits8Bit, true>(_Start, _End, _Out);

   
}


DECLARE_NOALIAS void __CDECL MinimumElement8BitUnsigned(
    const void* _Start,
    const void* _End,
    uint8* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits8Bit, false>(_Start, _End, _Out);
}


inline DECLARE_NOALIAS void __CDECL MinimumElement16Bit(
    const void* _Start,
    const void* _End,
    int16* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits16Bit, true>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MinimumElement32Bit(
    const void* _Start,
    const void* _End,
    int32* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits32Bit, true>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MinimumElement64Bit(
    const void* _Start,
    const void* _End,
    int64* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits64Bit, true>(_Start, _End, _Out);
}



inline DECLARE_NOALIAS void __CDECL MaximumElement8Bit(
    const void* _Start,
    const void* _End,
    int8* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits8Bit, true>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement16Bit(
    const void* _Start,
    const void* _End,
    int16* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits16Bit, true>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement32Bit(
    const void* _Start,
    const void* _End,
    int32* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits32Bit, true>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement64Bit(
    const void* _Start,
    const void* _End,
    int64* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits64Bit, true>(_Start, _End, _Out);
}

// -------------------------------------------------------------
inline DECLARE_NOALIAS void __CDECL MinimumElement16BitUnsigned(
    const void* _Start,
    const void* _End,
    uint16* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits16Bit, false>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MinimumElement32BitUnsigned(
    const void* _Start,
    const void* _End,
    uint32* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits32Bit, false>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MinimumElement64BitUnsigned(
    const void* _Start,
    const void* _End,
    uint64* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits64Bit, false>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement8BitUnsigned(
    const void* _Start,
    const void* _End,
    uint8* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits8Bit, false>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement16BitUnsigned(
    const void* _Start,
    const void* _End,
    uint16* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits16Bit, false>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement32BitUnsigned(
    const void* _Start,
    const void* _End,
    uint32* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits32Bit, false>(_Start, _End, _Out);
}

DECLARE_NOALIAS void __CDECL MaximumElement64BitUnsigned(
    const void* _Start,
    const void* _End,
    uint64* _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits64Bit, false>(_Start, _End, _Out);
}

__BASE_NAMESPACE_END
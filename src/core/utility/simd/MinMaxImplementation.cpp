#pragma once 

#include <src/core/utility/simd/SimdAlgorithm.h>

#include <src/core/utility/simd/traits/MinMaxTraitsSSE2.h>
#include <src/core/utility/simd/traits/MinMaxTraitsAVX.h>

#include <src/core/utility/simd/traits/MinMaxTraitsAVX512.h>
#include <src/core/utility/simd/traits/MinMaxTraits.h>

__BASE_NAMESPACE_BEGIN


template <
    class _Traits_,
    bool _Sign_,
    typename _Integer_,
    typename = std::enable_if_t<std::is_integral_v<_Integer_>>>
inline base_declare_const_function void base_cdecl MaximumIntegerImplementation(
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
            // The end

            if constexpr (_Sign_ || _SignCorrection) {
                const auto _HorizontalMaximum =
                    _Traits_::HorizontalMaximum(_CurrentMaximumValues); // A sorted vector with the maximum element at the first position
                _MaximumValue = _Traits_::GetAny(_HorizontalMaximum);
            }
            else {
                const auto _HorizontalMaximum =
                    _Traits_::HorizontalMaximumUnsigned(_CurrentMaximumValues); // A sorted vector with the maximum element at the first position
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
    typename = std::enable_if_t<std::is_integral_v<_Integer_>>>
inline base_declare_const_function void base_cdecl MinimumIntegerImplementation(
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
            // The end

            if constexpr (_Sign_ || _SignCorrection) {
                const auto _HorizontalMinimum =
                    _Traits_::HorizontalMinimum(_CurrentMinimumValues); // A sorted vector with the minimum element at the first position
                _MinimumValue = _Traits_::GetAny(_HorizontalMinimum);
            }
            else {
                const auto _HorizontalMinimum =
                    _Traits_::HorizontalMinimumUnsigned(_CurrentMinimumValues); // A sorted vector with the minimum element at the first position
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


base_declare_const_function void base_cdecl MinimumElement8Bit(
    const void* _Start,
    const void* _End,
    int8* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
            AVX512::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
            AVX::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
            SSE2::NumberTraits8Bit, true>(_Start, _End, _Out);
}

base_declare_const_function void base_cdecl MinimumElement8BitUnsigned(
    const void* _Start,
    const void* _End,
    uint8* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits8Bit, false>(_Start, _End, _Out);
}


inline base_declare_const_function void base_cdecl MinimumElement16Bit(
    const void* _Start,
    const void* _End,
    int16* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits16Bit, true>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MinimumElement32Bit(
    const void* _Start,
    const void* _End,
    int32* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits32Bit, true>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MinimumElement64Bit(
    const void* _Start,
    const void* _End,
    int64* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits64Bit, true>(_Start, _End, _Out);
}



inline base_declare_const_function void base_cdecl MaximumElement8Bit(
    const void* _Start,
    const void* _End,
    int8* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits8Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits8Bit, true>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MaximumElement16Bit(
    const void* _Start,
    const void* _End,
    int16* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits16Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits16Bit, true>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MaximumElement32Bit(
    const void* _Start,
    const void* _End,
    int32* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits32Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits32Bit, true>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MaximumElement64Bit(
    const void* _Start,
    const void* _End,
    int64* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits64Bit, true>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits64Bit, true>(_Start, _End, _Out);
}

// -------------------------------------------------------------
inline base_declare_const_function void base_cdecl MinimumElement16BitUnsigned(
    const void* _Start,
    const void* _End,
    uint16* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits16Bit, false>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MinimumElement32BitUnsigned(
    const void* _Start,
    const void* _End,
    uint32* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits32Bit, false>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MinimumElement64BitUnsigned(
    const void* _Start,
    const void* _End,
    uint64* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
        AVX512::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
        AVX::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
        SSE2::NumberTraits64Bit, false>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MaximumElement8BitUnsigned(
    const void* _Start,
    const void* _End,
    uint8* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits8Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits8Bit, false>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MaximumElement16BitUnsigned(
    const void* _Start,
    const void* _End,
    uint16* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits16Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits16Bit, false>(_Start, _End, _Out);
}

inline base_declare_const_function void base_cdecl MaximumElement32BitUnsigned(
    const void* _Start,
    const void* _End,
    uint32* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits32Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits32Bit, false>(_Start, _End, _Out);
}

base_declare_const_function void base_cdecl MaximumElement64BitUnsigned(
    const void* _Start,
    const void* _End,
    uint64* _Out)
{
    if (arch::ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
        AVX512::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
        AVX::NumberTraits64Bit, false>(_Start, _End, _Out);
    else if (arch::ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
        SSE2::NumberTraits64Bit, false>(_Start, _End, _Out);
}

__BASE_NAMESPACE_END
#pragma once 

#include <base/core/math/MinMaxTraits.h>

__BASE_NAMESPACE_BEGIN

template <
    class _Traits_,
    bool _Sign_,
    typename _Integer_,
    typename = std::enable_if_t<IsValidIntegerType<_Integer_>>>
inline DECLARE_NOALIAS void __CDECL MaximumIntegerImplementation(
    const void* _Start,
    const void* _End,
    _Integer_*  _Out)
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

    _Ty _Cur_max_val; // initialized in both of the branches below

    constexpr bool _Sign_correction = sizeof(_Ty) == 8 && !_Sign_;
    const sizetype _Byte_size = _Length & ~sizetype{ _Traits_::StepSizeInBytes - 1 };

    const void* _Stop_at = _Start;
    memory::AdvanceBytes(_Stop_at, _Byte_size);

    auto _Cur_vals = _Traits_::Load(_Start);

    if constexpr (_Sign_correction)
        _Cur_vals = _Traits_::SignCorrection(_Cur_vals, false);

    auto _Cur_vals_max = _Cur_vals; // vector of vertical maximum values

    for (;;) {
        memory::AdvanceBytes(_Start, _Traits_::StepSizeInBytes);

        if (_Start != _Stop_at) {
            // This is the main part, finding vertical minimum/maximum

            _Cur_vals = _Traits_::Load(_Start);

            if constexpr (_Sign_correction)
                _Cur_vals = _Traits_::SignCorrection(_Cur_vals, false);
            
            if constexpr (_Sign_ || _Sign_correction) 
                _Cur_vals_max = _Traits_::Maximum(_Cur_vals_max, _Cur_vals); // Update the current maximum
            else
                _Cur_vals_max = _Traits_::MaximumUnsigned(_Cur_vals_max, _Cur_vals); // Update the current maximum
        }
        else {
            // Reached end. Compute horizontal min and/or max.
            if constexpr (_Sign_ || _Sign_correction) {
                const auto _H_max =
                    _Traits_::HorizontalMaximum(_Cur_vals_max); // Vector populated by the largest element
                _Cur_max_val = _Traits_::GetAny(_H_max); // Get any element of it
            }
            else {
                const auto _H_max =
                    _Traits_::HorizontalMaximumUnsigned(_Cur_vals_max); // Vector populated by the largest element
                _Cur_max_val = _Traits_::GetAny(_H_max); // Get any element of it
            }

            if constexpr (_Sign_correction) {
                constexpr _Ty _Correction = _Ty{ 1 } << (sizeof(_Ty) * 8 - 1);
                _Cur_max_val += _Correction;
            }

            break;
        }
    }

    *_Out = _Cur_max_val;
}

template <
    class _Traits_,
    bool _Sign_,
    typename _Integer_,
    typename = std::enable_if_t<IsValidIntegerType<_Integer_>>>
inline DECLARE_NOALIAS void __CDECL MinimumIntegerImplementation(
    const void* _Start,
    const void* _End,
    _Integer_*  _Out)
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

    _Ty _Cur_min_val; // initialized in both of the branches below

    constexpr bool _Sign_correction = sizeof(_Ty) == 8 && !_Sign_;
    const sizetype _Byte_size = _Length & ~sizetype{ _Traits_::StepSizeInBytes - 1 };

    const void* _Stop_at = _Start;
    memory::AdvanceBytes(_Stop_at, _Byte_size);

    auto _Cur_vals = _Traits_::Load(_Start);

    if constexpr (_Sign_correction)
        _Cur_vals = _Traits_::SignCorrection(_Cur_vals, false);

    auto _Cur_vals_min = _Cur_vals; // vector of vertical minimum values

    for (;;) {
        memory::AdvanceBytes(_Start, _Traits_::StepSizeInBytes);

        if (_Start != _Stop_at) {
            _Cur_vals = _Traits_::Load(_Start);

            if constexpr (_Sign_correction)
                _Cur_vals = _Traits_::SignCorrection(_Cur_vals, false);

            if constexpr (_Sign_ || _Sign_correction)
                _Cur_vals_min = _Traits_::Minimum(_Cur_vals_min, _Cur_vals); // Update the current minimum
            else
                _Cur_vals_min = _Traits_::MinimumUnsigned(_Cur_vals_min, _Cur_vals); // Update the current minimum
        }
        else {
            // Reached end. Compute horizontal min and/or max.

            if constexpr (_Sign_ || _Sign_correction) {
                const auto _H_min =
                    _Traits_::HorizontalMinimum(_Cur_vals_min); // Vector populated by the smallest element
                _Cur_min_val = _Traits_::GetAny(_H_min); // Get any element of it
            }
            else {
                const auto _H_min =
                    _Traits_::HorizontalMinimumUnsigned(_Cur_vals_min); // Vector populated by the smallest element
                _Cur_min_val = _Traits_::GetAny(_H_min); // Get any element of it
            }

            if constexpr (_Sign_correction) {
                constexpr _Ty _Correction = _Ty{ 1 } << (sizeof(_Ty) * 8 - 1);
                _Cur_min_val += _Correction;
            }

            break;
        }
    }

    *_Out = _Cur_min_val;
}

// -------------------------------------------------

inline DECLARE_NOALIAS void __CDECL MinimumElement8Bit(
    const void* _Start,
    const void* _End,
    int8*       _Out)
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

inline DECLARE_NOALIAS void __CDECL MinimumElement16Bit(
    const void* _Start,
    const void* _End,
    int16*      _Out)
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
    int32*      _Out)
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
    int64*      _Out)
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
    int8*       _Out)
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
    int16*      _Out)
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
    int32*      _Out)
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
    int64*      _Out)
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

inline DECLARE_NOALIAS void __CDECL MinimumElement8BitUnsigned(
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

inline DECLARE_NOALIAS void __CDECL MaximumElement64BitUnsigned(
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
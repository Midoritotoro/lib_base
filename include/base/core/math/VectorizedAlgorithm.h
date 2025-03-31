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

    if (_Length < 16) {
        Scalar::GetMaximum((_Integer_*)_Start, (_Integer_*)_End, _Out);
        return;
    }

    using _Ty = std::conditional_t<_Sign_, 
        typename _Traits_::SignedType,
        typename _Traits_::UnsignedType>;

    _Ty _Cur_min_val; // initialized in both of the branches below
    _Ty _Cur_max_val; // initialized in both of the branches below

    constexpr bool _Sign_correction = sizeof(_Ty) == 8 && !_Sign_;
    const sizetype _Sse_byte_size = _Length & ~sizetype{ 0xF };

    const void* _Stop_at = _Start;
    memory::AdvanceBytes(_Stop_at, _Sse_byte_size);

    auto _Cur_vals = _Traits_::Load(_Start);

    if constexpr (_Sign_correction) {
        _Cur_vals = _Traits_::SignCorrection(_Cur_vals, false);
    }

    auto _Cur_vals_min = _Cur_vals; // vector of vertical minimum values
    auto _Cur_vals_max = _Cur_vals; // vector of vertical maximum values

    return;
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
    return;
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
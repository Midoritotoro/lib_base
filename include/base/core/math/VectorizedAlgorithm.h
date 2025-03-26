#pragma once 

#include <base/core/math/MinMaxTraits.h>

__BASE_NAMESPACE_BEGIN

template <
    class _Traits_,
    typename _Integer_,
    bool _Sign_,
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
        Scalar::GetMaximum(_Start, _End, _Out);
        return;
    }

    using _Ty = std::conditional_t<_Sign, typename _Traits_::SignedType, typename _Traits::_Unsigned_t>;

    _Ty _Cur_min_val; // initialized in both of the branches below
    _Ty _Cur_max_val; // initialized in both of the branches below
}

template <
    class _Traits_,
    typename _Integer_,
    bool _Sign_,
    typename = std::enable_if_t<IsValidIntegerType<_Integer_>>>
inline DECLARE_NOALIAS void __CDECL MinimumIntegerImplementation(
    const void* _Start,
    const void* _End,
    _Integer_*  _Out)
{

}

inline DECLARE_NOALIAS void __CDECL MinimumElement8Bit(
    const void* _Start,
    const void* _End,
    int8*       _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
            AVX512::NumberTraits8Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
            AVX::NumberTraits8Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
            SSE2::NumberTraits8Bit>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MinimumElement16Bit(
    const void* _Start,
    const void* _End,
    int16*      _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
            AVX512::NumberTraits16Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
            AVX::NumberTraits16Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
            SSE2::NumberTraits16Bit>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MinimumElement32Bit(
    const void* _Start,
    const void* _End,
    int32*      _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
            AVX512::NumberTraits32Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
            AVX::NumberTraits32Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
            SSE2::NumberTraits32Bit>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MinimumElement64Bit(
    const void* _Start,
    const void* _End,
    int64*      _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MinimumIntegerImplementation<
            AVX512::NumberTraits64Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MinimumIntegerImplementation<
            AVX::NumberTraits64Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MinimumIntegerImplementation<
            SSE2::NumberTraits64Bit>(_Start, _End, _Out);
}


inline DECLARE_NOALIAS void __CDECL MaximumElement8Bit(
    const void* _Start,
    const void* _End,
    int8*       _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
            AVX512::NumberTraits8Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
            AVX::NumberTraits8Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
            SSE2::NumberTraits8Bit>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement16Bit(
    const void* _Start,
    const void* _End,
    int16*      _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
            AVX512::NumberTraits16Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
            AVX::NumberTraits16Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
            SSE2::NumberTraits16Bit>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement32Bit(
    const void* _Start,
    const void* _End,
    int32*      _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
            AVX512::NumberTraits32Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
            AVX::NumberTraits32Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
            SSE2::NumberTraits32Bit>(_Start, _End, _Out);
}

inline DECLARE_NOALIAS void __CDECL MaximumElement64Bit(
    const void* _Start,
    const void* _End,
    int64*      _Out)
{
    if (ProcessorFeatures::AVX512F())
        return MaximumIntegerImplementation<
            AVX512::NumberTraits64Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::AVX())
        return MaximumIntegerImplementation<
            AVX::NumberTraits64Bit>(_Start, _End, _Out);
    else if (ProcessorFeatures::SSE2())
        return MaximumIntegerImplementation<
            SSE2::NumberTraits64Bit>(_Start, _End, _Out);
}



__BASE_NAMESPACE_END
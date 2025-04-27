#pragma once 

#include <base/core/utility/simd/SimdAlgorithm.h>


__BASE_NAMESPACE_BEGIN

inline DECLARE_NOALIAS void __CDECL MinimumElement8Bit(
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
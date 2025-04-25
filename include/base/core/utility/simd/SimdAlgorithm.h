#pragma once 

#include <base/core/arch/Platform.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

extern "C" {
    // The "noalias" attribute tells the compiler optimizer that pointers going into these hand-vectorized algorithms
    // won't be stored beyond the lifetime of the function, and that the function will only reference arrays denoted by
    // those pointers. The optimizer also assumes in that case that a pointer parameter is not returned to the caller via
    // the return value, so functions using "noalias" must usually return void. This attribute is valuable because these
    // functions are in native code objects that the compiler cannot analyze. In the absence of the noalias attribute, the
    // compiler has to assume that the denoted arrays are "globally address taken", and that any later calls to
    // unanalyzable routines may modify those arrays.

    DECLARE_NOALIAS void __CDECL ReverseCopyTriviallyCopyable8Bit(
        const void* _First, 
        const void* _Last, 
        void* _Dest) noexcept;
    DECLARE_NOALIAS void __CDECL ReverseCopyTriviallyCopyable16Bit(
        const void* _First,
        const void* _Last,
        void* _Dest) noexcept;

    DECLARE_NOALIAS void __CDECL ReverseCopyTriviallyCopyable32Bit(
        const void* _First, 
        const void* _Last, 
        void* _Dest) noexcept;
    DECLARE_NOALIAS void __CDECL ReverseCopyTriviallyCopyable64Bit(
        const void* _First, 
        const void* _Last, 
        void* _Dest) noexcept;

    // ================================================================================

    const void* STDCALL FindLastTrivial8Bit(
        const void* _First, 
        const void* _Last,
        uint8_t _Val) noexcept;
    const void* STDCALL FindLastTrivial16Bit(
        const void* _First, 
        const void* _Last, 
        uint16_t _Val) noexcept;

    // ================================================================================

    const void* STDCALL FindLastTrivial32Bit(
        const void* _First, 
        const void* _Last, 
        uint32_t _Val) noexcept;
    const void* STDCALL FindLastTrivial64Bit(
        const void* _First, 
        const void* _Last, 
        uint64_t _Val) noexcept;
    
    // ================================================================================
    const void* STDCALL FindFirstOfTrivial8Bit(
        const void* _First1, 
        const void* _Last1,
        const void* _First2,
        const void* _Last2) noexcept;
    const void* STDCALL FindFirstOfTrivial16Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;

    const void* STDCALL FindFirstOfTrivial32Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;
    const void* STDCALL FindFirstOfTrivial64Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;


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

    //DECLARE_NOALIAS  Min_max_1i STDCALL MinMax8Bit(
    //    const void* _First, 
    //    const void* _Last) noexcept;
    //DECLARE_NOALIAS _Min_max_1u STDCALL __std_minmax_1u(
    //    const void* _First,
    //    const void* _Last) noexcept;

    //DECLARE_NOALIAS _Min_max_2i STDCALL __std_minmax_2i(
    //    const void* _First, 
    //    const void* _Last) noexcept;
    //DECLARE_NOALIAS _Min_max_2u STDCALL __std_minmax_2u(
    //    const void* _First,
    //    const void* _Last) noexcept;

    //DECLARE_NOALIAS _Min_max_4i STDCALL __std_minmax_4i(
    //    const void* _First, 
    //    const void* _Last) noexcept;
    //DECLARE_NOALIAS _Min_max_4u STDCALL __std_minmax_4u(
    //    const void* _First, 
    //    const void* _Last) noexcept;

    //DECLARE_NOALIAS _Min_max_8i STDCALL __std_minmax_8i(
    //    const void* _First, 
    //    const void* _Last) noexcept;
    //DECLARE_NOALIAS _Min_max_8u STDCALL __std_minmax_8u(
    //    const void* _First, 
    //    const void* _Last) noexcept;

    //DECLARE_NOALIAS _Min_max_f STDCALL __std_minmax_f(
    //    const void* _First, 
    //    const void* _Last) noexcept;
    //DECLARE_NOALIAS _Min_max_d STDCALL __std_minmax_d(
    //    const void* _First, 
    //    const void* _Last) noexcept;

    DECLARE_NOALIAS void STDCALL Replace32Bit(
        void*       _First,
        void*       _Last,
        uint32_t    _OldValue,
        uint32_t    _New_val) noexcept;
    DECLARE_NOALIAS void STDCALL Replace64Bit(
        void* _First, 
        void* _Last,
        uint64_t _Old_val,
        uint64_t _New_val) noexcept;
} // extern "C"

__BASE_CONTAINER_NAMESPACE_END
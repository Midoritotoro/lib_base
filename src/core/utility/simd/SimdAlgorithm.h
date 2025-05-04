#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/MinMax.h>

__BASE_NAMESPACE_BEGIN

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
        void*       _Dest) noexcept;
    DECLARE_NOALIAS void __CDECL ReverseCopyTriviallyCopyable16Bit(
        const void* _First,
        const void* _Last,
        void*       _Dest) noexcept;

    // ================================================================================

    DECLARE_NOALIAS void __CDECL ReverseCopyTriviallyCopyable32Bit(
        const void* _First, 
        const void* _Last, 
        void*       _Dest) noexcept;
    DECLARE_NOALIAS void __CDECL ReverseCopyTriviallyCopyable64Bit(
        const void* _First, 
        const void* _Last, 
        void*       _Dest) noexcept;

    // ================================================================================

    DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial8Bit(
        const void* _First, 
        const void* _Last,
        uint8       _Val) noexcept;
    DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial16Bit(
        const void* _First, 
        const void* _Last, 
        uint16      _Val) noexcept;

    // ================================================================================

    DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial32Bit(
        const void* _First, 
        const void* _Last, 
        uint32      _Val) noexcept;
    DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial64Bit(
        const void* _First, 
        const void* _Last, 
        uint64      _Val) noexcept;
    
    // ================================================================================

    DECLARE_NOALIAS NODISCARD const void* STDCALL FindFirstOfTrivial8Bit(
        const void* _First1, 
        const void* _Last1,
        const void* _First2,
        const void* _Last2) noexcept;
    DECLARE_NOALIAS NODISCARD const void* STDCALL FindFirstOfTrivial16Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;

    // ================================================================================

    DECLARE_NOALIAS NODISCARD const void* STDCALL FindFirstOfTrivial32Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;
    DECLARE_NOALIAS NODISCARD const void* STDCALL FindFirstOfTrivial64Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;

    DECLARE_NOALIAS void __CDECL MinimumElement8Bit(
        const void* _Start,
        const void* _End,
        int8*       _Out);
    DECLARE_NOALIAS void __CDECL MinimumElement8BitUnsigned(
        const void* _Start,
        const void* _End,
        uint8*      _Out);

    // ================================================================================

    DECLARE_NOALIAS void __CDECL MinimumElement16Bit(
        const void* _Start,
        const void* _End,
        int16*      _Out);
    DECLARE_NOALIAS void __CDECL MinimumElement16BitUnsigned(
        const void* _Start,
        const void* _End,
        uint16*     _Out);

    // ================================================================================

    DECLARE_NOALIAS void __CDECL MinimumElement32Bit(
        const void* _Start,
        const void* _End,
        int32*      _Out);
    DECLARE_NOALIAS void __CDECL MinimumElement32BitUnsigned(
        const void* _Start,
        const void* _End,
        uint32*     _Out);

    // ================================================================================

    DECLARE_NOALIAS void __CDECL MinimumElement64Bit(
        const void* _Start,
        const void* _End,
        int64*      _Out);
    DECLARE_NOALIAS void __CDECL MinimumElement64BitUnsigned(
        const void* _Start,
        const void* _End,
        uint64*     _Out);

    // ================================================================================

    DECLARE_NOALIAS void __CDECL MaximumElement8Bit(
        const void* _Start,
        const void* _End,
        int8*       _Out);
    DECLARE_NOALIAS void __CDECL MaximumElement8BitUnsigned(
        const void* _Start,
        const void* _End,
        uint8*      _Out);
            
    // ================================================================================

    DECLARE_NOALIAS void __CDECL MaximumElement16Bit(
        const void* _Start,
        const void* _End,
        int16*      _Out);
    DECLARE_NOALIAS void __CDECL MaximumElement16BitUnsigned(
        const void* _Start,
        const void* _End,
        uint16*     _Out);

    // ================================================================================

    DECLARE_NOALIAS void __CDECL MaximumElement32Bit(
        const void* _Start,
        const void* _End,
        int32*      _Out);
    DECLARE_NOALIAS void __CDECL MaximumElement32BitUnsigned(
        const void* _Start,
        const void* _End,
        uint32*     _Out);

    // ================================================================================

    DECLARE_NOALIAS void __CDECL MaximumElement64Bit(
        const void* _Start,
        const void* _End,
        int64*      _Out);
    DECLARE_NOALIAS void __CDECL MaximumElement64BitUnsigned(
        const void* _Start,
        const void* _End,
        uint64*     _Out);

    // ================================================================================

    DECLARE_NOALIAS NODISCARD MinMaxInt8Bit STDCALL MinimumAndMaximumElement8Bit(
        const void* _First, 
        const void* _Last) noexcept;
    DECLARE_NOALIAS NODISCARD MinMaxUInt8Bit STDCALL MinimumAndMaximumElement8BitUnsigned(
        const void* _First,
        const void* _Last) noexcept;

    // ================================================================================

    DECLARE_NOALIAS NODISCARD MinMaxInt16Bit STDCALL MinimumAndMaximumElement16Bit(
        const void* _First, 
        const void* _Last) noexcept;
    DECLARE_NOALIAS NODISCARD MinMaxUInt16Bit STDCALL MinimumAndMaximumElement16BitUnsigned(
        const void* _First,
        const void* _Last) noexcept;

    // ================================================================================

    DECLARE_NOALIAS NODISCARD MinMaxInt32Bit STDCALL MinimumAndMaximumElement32Bit(
        const void* _First, 
        const void* _Last) noexcept;
    DECLARE_NOALIAS NODISCARD MinMaxInt32Bit STDCALL MinimumAndMaximumElement32BitUnsigned(
        const void* _First, 
        const void* _Last) noexcept;

    // ================================================================================

    DECLARE_NOALIAS NODISCARD MinMaxInt64Bit STDCALL MinimumAndMaximumElement64Bit(
        const void* _First, 
        const void* _Last) noexcept;
    DECLARE_NOALIAS NODISCARD MinMaxInt64Bit STDCALL MinimumAndMaximumElement64BitUnsigned(
        const void* _First, 
        const void* _Last) noexcept;

    // ================================================================================

    DECLARE_NOALIAS NODISCARD MinMaxFloat STDCALL MinimumAndMaximumElementFloat(
        const void* _First, 
        const void* _Last) noexcept;
    DECLARE_NOALIAS NODISCARD MinMaxDouble STDCALL MinimumAndMaximumElementDouble(
        const void* _First, 
        const void* _Last) noexcept;

    // ================================================================================

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

    // ================================================================================

    DECLARE_NOALIAS void __CDECL SwapRangesTriviallySwappable(
        void* firstPointer1,
        void* lastPointer1,
        void* firstPointer2) noexcept;
} // extern "C"

template <class _Type_>
DECLARE_NOALIAS CONSTEXPR_CXX20 NODISCARD const void* FindVectorized(
    const void* firstPointer,
    const void* lastPointer,
    const _Type_& value) noexcept;

template <class _Type_>
DECLARE_NOALIAS CONSTEXPR_CXX20 NODISCARD std::size_t CountVectorized(
    const void* firstPointer,
    const void* lastPointer,
    const _Type_& value) noexcept;

__BASE_NAMESPACE_END
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

    base_declare_const_function void base_cdecl ReverseTriviallySwappable8Bit(
        const void* _First,
        const void* _Last,
        void* _Dest) noexcept;
    base_declare_const_function void base_cdecl ReverseTriviallySwappable16Bit(
        const void* _First,
        const void* _Last,
        void* _Dest) noexcept;

    // ================================================================================

    base_declare_const_function void base_cdecl ReverseTriviallySwappable32Bit(
        const void* _First,
        const void* _Last,
        void* _Dest) noexcept;
    base_declare_const_function void base_cdecl ReverseTriviallySwappable64Bit(
        const void* _First,
        const void* _Last,
        void* _Dest) noexcept;

    // ================================================================================

    base_declare_const_function void base_cdecl ReverseCopyTriviallyCopyable8Bit(
        const void* _First, 
        const void* _Last, 
        void*       _Dest) noexcept;
    base_declare_const_function void base_cdecl ReverseCopyTriviallyCopyable16Bit(
        const void* _First,
        const void* _Last,
        void*       _Dest) noexcept;

    // ================================================================================

    base_declare_const_function void base_cdecl ReverseCopyTriviallyCopyable32Bit(
        const void* _First, 
        const void* _Last, 
        void*       _Dest) noexcept;
    base_declare_const_function void base_cdecl ReverseCopyTriviallyCopyable64Bit(
        const void* _First, 
        const void* _Last, 
        void*       _Dest) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard const void* base_stdcall FindLastTrivial8Bit(
        const void* _First, 
        const void* _Last,
        uint8       _Val) noexcept;
    base_declare_const_function base_nodiscard const void* base_stdcall FindLastTrivial16Bit(
        const void* _First, 
        const void* _Last, 
        uint16      _Val) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard const void* base_stdcall FindLastTrivial32Bit(
        const void* _First, 
        const void* _Last, 
        uint32      _Val) noexcept;
    base_declare_const_function base_nodiscard const void* base_stdcall FindLastTrivial64Bit(
        const void* _First, 
        const void* _Last, 
        uint64      _Val) noexcept;
    
    // ================================================================================

    base_declare_const_function base_nodiscard const void* base_stdcall FindFirstOfTrivial8Bit(
        const void* _First1, 
        const void* _Last1,
        const void* _First2,
        const void* _Last2) noexcept;
    base_declare_const_function base_nodiscard const void* base_stdcall FindFirstOfTrivial16Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard const void* base_stdcall FindFirstOfTrivial32Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;
    base_declare_const_function base_nodiscard const void* base_stdcall FindFirstOfTrivial64Bit(
        const void* _First1,
        const void* _Last1, 
        const void* _First2, 
        const void* _Last2) noexcept;

    // ==================================================================================

    base_declare_const_function base_nodiscard const void* FindSubRange8Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
    base_declare_const_function base_nodiscard const void* FindSubRange16Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;

    // ==================================================================================

    base_declare_const_function base_nodiscard const void* FindSubRange32Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
    base_declare_const_function base_nodiscard const void* FindSubRange64Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;

    base_declare_const_function void base_cdecl MinimumElement8Bit(
        const void* _Start,
        const void* _End,
        int8*       _Out);
    base_declare_const_function void base_cdecl MinimumElement8BitUnsigned(
        const void* _Start,
        const void* _End,
        uint8*      _Out);

    // ================================================================================

    base_declare_const_function void base_cdecl MinimumElement16Bit(
        const void* _Start,
        const void* _End,
        int16*      _Out);
    base_declare_const_function void base_cdecl MinimumElement16BitUnsigned(
        const void* _Start,
        const void* _End,
        uint16*     _Out);

    // ================================================================================

    base_declare_const_function void base_cdecl MinimumElement32Bit(
        const void* _Start,
        const void* _End,
        int32*      _Out);
    base_declare_const_function void base_cdecl MinimumElement32BitUnsigned(
        const void* _Start,
        const void* _End,
        uint32*     _Out);

    // ================================================================================

    base_declare_const_function void base_cdecl MinimumElement64Bit(
        const void* _Start,
        const void* _End,
        int64*      _Out);
    base_declare_const_function void base_cdecl MinimumElement64BitUnsigned(
        const void* _Start,
        const void* _End,
        uint64*     _Out);

    // ================================================================================

    base_declare_const_function void base_cdecl MaximumElement8Bit(
        const void* _Start,
        const void* _End,
        int8*       _Out);
    base_declare_const_function void base_cdecl MaximumElement8BitUnsigned(
        const void* _Start,
        const void* _End,
        uint8*      _Out);
            
    // ================================================================================

    base_declare_const_function void base_cdecl MaximumElement16Bit(
        const void* _Start,
        const void* _End,
        int16*      _Out);
    base_declare_const_function void base_cdecl MaximumElement16BitUnsigned(
        const void* _Start,
        const void* _End,
        uint16*     _Out);

    // ================================================================================

    base_declare_const_function void base_cdecl MaximumElement32Bit(
        const void* _Start,
        const void* _End,
        int32*      _Out);
    base_declare_const_function void base_cdecl MaximumElement32BitUnsigned(
        const void* _Start,
        const void* _End,
        uint32*     _Out);

    // ================================================================================

    base_declare_const_function void base_cdecl MaximumElement64Bit(
        const void* _Start,
        const void* _End,
        int64*      _Out);
    base_declare_const_function void base_cdecl MaximumElement64BitUnsigned(
        const void* _Start,
        const void* _End,
        uint64*     _Out);

    // ================================================================================

    base_declare_const_function base_nodiscard MinMaxInt8Bit base_stdcall MinimumAndMaximumElement8Bit(
        const void* _First, 
        const void* _Last) noexcept;
    base_declare_const_function base_nodiscard MinMaxUInt8Bit base_stdcall MinimumAndMaximumElement8BitUnsigned(
        const void* _First,
        const void* _Last) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard MinMaxInt16Bit base_stdcall MinimumAndMaximumElement16Bit(
        const void* _First, 
        const void* _Last) noexcept;
    base_declare_const_function base_nodiscard MinMaxUInt16Bit base_stdcall MinimumAndMaximumElement16BitUnsigned(
        const void* _First,
        const void* _Last) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard MinMaxInt32Bit base_stdcall MinimumAndMaximumElement32Bit(
        const void* _First, 
        const void* _Last) noexcept;
    base_declare_const_function base_nodiscard MinMaxInt32Bit base_stdcall MinimumAndMaximumElement32BitUnsigned(
        const void* _First, 
        const void* _Last) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard MinMaxInt64Bit base_stdcall MinimumAndMaximumElement64Bit(
        const void* _First, 
        const void* _Last) noexcept;
    base_declare_const_function base_nodiscard MinMaxInt64Bit base_stdcall MinimumAndMaximumElement64BitUnsigned(
        const void* _First, 
        const void* _Last) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard MinMaxFloat base_stdcall MinimumAndMaximumElementFloat(
        const void* _First, 
        const void* _Last) noexcept;
    base_declare_const_function base_nodiscard MinMaxDouble base_stdcall MinimumAndMaximumElementDouble(
        const void* _First, 
        const void* _Last) noexcept;

    // ================================================================================

    base_declare_const_function void base_stdcall Replace32Bit(
        void*       _First,
        void*       _Last,
        uint32_t    _OldValue,
        uint32_t    _New_val) noexcept;
    base_declare_const_function void base_stdcall Replace64Bit(
        void* _First, 
        void* _Last,
        uint64_t _Old_val,
        uint64_t _New_val) noexcept;

    // ================================================================================

    base_declare_const_function void base_cdecl SwapRangesTriviallySwappable(
        void* firstPointer1,
        void* lastPointer1,
        void* firstPointer2) noexcept;

    // ================================================================================


    base_declare_const_function base_nodiscard std::size_t Count8Bit(
        const void* firstPointer,
        const void* lastPointer,
        uint8       value) noexcept;
    base_declare_const_function base_nodiscard std::size_t Count16Bit(
        const void* firstPointer,
        const void* lastPointer,
        uint16      value) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard std::size_t Count32Bit(
        const void* firstPointer,
        const void* lastPointer,
        uint32      value) noexcept;
    base_declare_const_function base_nodiscard std::size_t Count64Bit(
        const void* firstPointer,
        const void* lastPointer,
        uint64      value) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard std::size_t CountSubRange8Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
    base_declare_const_function base_nodiscard std::size_t CountSubRange16Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard std::size_t CountSubRange32Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
    base_declare_const_function base_nodiscard std::size_t CountSubRange64Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;

    // ================================================================================

    base_declare_const_function base_nodiscard const void* FindLastSubRange8Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
    base_declare_const_function base_nodiscard const void* FindLastSubRange16Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
    
    // ================================================================================

    base_declare_const_function base_nodiscard const void* FindLastSubRange32Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
    base_declare_const_function base_nodiscard const void* FindLastSubRange64Bit(
        const void* firstMainRangePointer,
        const void* lastMainRangePointer,
        const void* firstSubRangePointer,
        const void* lastSubRangePointer) noexcept;
} // extern "C"

template <class _Type_>
base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* FindVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept;

base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* FindSubRangeVectorized(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept;

template <class _Type_>
base_declare_const_function base_constexpr_cxx20 base_nodiscard std::size_t CountVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept;

base_declare_const_function base_constexpr_cxx20 base_nodiscard std::size_t CountSubRangeVectorized(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept;

template <class _Type_>
base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* FindLastVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept;

base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* FindLastSubRangeVectorized(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept;

template <class _Type_>
base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* ContainsVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept;

template <class _Type_>
base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* ContainsSubRangeVectorized(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept;

template <size_t typeSizeInBytes>
base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* ReverseVectorized(
    void* firstPointer,
    void* lastPointer) noexcept;

template <size_t typeSizeInBytes>
base_declare_const_function base_constexpr_cxx20 base_nodiscard const void* ReverseCopyVectorized(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept;

__BASE_NAMESPACE_END
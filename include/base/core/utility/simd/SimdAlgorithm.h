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

   /* _Min_max_element_t STDCALL MinMaxElement8Bit(
        const void* _First, 
        const void* _Last,
        bool _Signed) noexcept;
    _Min_max_element_t STDCALL MinMaxElement16Bit(
        const void* _First,
        const void* _Last, 
        bool _Signed) noexcept;

    _Min_max_element_t STDCALL MinMaxElement32Bit(
        const void* _First,
        const void* _Last,
        bool _Signed) noexcept;
    _Min_max_element_t STDCALL MinMaxElement64Bit(
        const void* _First, 
        const void* _Last, 
        bool _Signed) noexcept;

    _Min_max_element_t STDCALL MinMaxElementFloat(
        const void* _First,
        const void* _Last, 
        bool _Unused) noexcept;
    _Min_max_element_t STDCALL MinMaxElementDouble(
        const void* _First, 
        const void* _Last, 
        bool _Unused) noexcept;*/

    const void* STDCALL FindLastTrivial8Bit(
        const void* _First, 
        const void* _Last,
        uint8_t _Val) noexcept;
    const void* STDCALL FindLastTrivial16Bit(
        const void* _First, 
        const void* _Last, 
        uint16_t _Val) noexcept;

    const void* STDCALL FindLastTrivial32Bit(
        const void* _First, 
        const void* _Last, 
        uint32_t _Val) noexcept;
    const void* STDCALL FindLastTrivial64Bit(
        const void* _First, 
        const void* _Last, 
        uint64_t _Val) noexcept;

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
        void* _First,
        void* _Last,
        uint32_t _Old_val, 
        uint32_t _New_val) noexcept;
    DECLARE_NOALIAS void STDCALL Replace64Bit(
        void* _First, 
        void* _Last,
        uint64_t _Old_val,
        uint64_t _New_val) noexcept;
} // extern "C"

__BASE_CONTAINER_NAMESPACE_END
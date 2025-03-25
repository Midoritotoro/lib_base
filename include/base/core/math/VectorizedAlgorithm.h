#pragma once 

#include <base/core/math/MinMaxTraits.h>

__BASE_NAMESPACE_BEGIN




class FakeTraits {};


template <
    typename Integer,
    std::enable_if_t<IsValidIntegerType<Integer>>>
inline DECLARE_NOALIAS void __CDECL MaxIntegerImplementation(
    const void* _Start,
    const void* _End,
    Integer* _Out)
{
    
}
template <
    typename Integer,
    std::enable_if_t<IsValidIntegerType<Integer>>>
inline DECLARE_NOALIAS void __CDECL MinIntegerImplementation(
    const void* _Start,
    const void* _End,
    Integer*    _Out)
{

}

inline DECLARE_NOALIAS void __CDECL MinimumElement8Bit(
    const void* _Start,
    const void* _End,
    int8*       _Out)
{
    const auto _Length = memory::ByteLength(_Start, _End);
    if (_Length < 16)


/*    if (ProcessorFeatures::AVX2()) {

    }
    else if (ProcessorFeatures::AVX()) {

    }
    else */if (ProcessorFeatures::SSE2()) {
            using _Traits_ = SSE2::NumberTraits8Bit;

            const auto _Loaded = _Traits_::Load(_Start);
        }
}


__BASE_NAMESPACE_END
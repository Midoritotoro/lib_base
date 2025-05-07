#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>

__BASE_NAMESPACE_BEGIN

template <class _BidIt>
_CONSTEXPR20 void reverse(
    const _BidIt _First, 
    const _BidIt _Last) 
{ // reverse elements in [_First, _Last)
    _Adl_verify_range(_First, _Last);
    auto _UFirst = _Get_unwrapped(_First);
    auto _ULast = _Get_unwrapped(_Last);
#if _USE_STD_VECTOR_ALGORITHMS
    using _Elem = remove_reference_t<_Iter_ref_t<decltype(_UFirst)>>;
    constexpr bool _Allow_vectorization = conjunction_v<bool_constant<_Iterator_is_contiguous<decltype(_UFirst)>>,
        _Is_trivially_swappable<_Elem>, negation<is_volatile<_Elem>>>;
    constexpr size_t _Nx = sizeof(_Elem);

#pragma warning(suppress : 6326) // Potential comparison of a constant with another constant
    if constexpr (_Allow_vectorization && _Nx <= 8 && (_Nx & (_Nx - 1)) == 0) {
#if _HAS_CXX20
        if (!_STD is_constant_evaluated())
#endif // _HAS_CXX20
        {
            if constexpr (_Nx == 1) {
                __std_reverse_trivially_swappable_1(_To_address(_UFirst), _To_address(_ULast));
            }
            else if constexpr (_Nx == 2) {
                __std_reverse_trivially_swappable_2(_To_address(_UFirst), _To_address(_ULast));
            }
            else if constexpr (_Nx == 4) {
                __std_reverse_trivially_swappable_4(_To_address(_UFirst), _To_address(_ULast));
            }
            else {
                __std_reverse_trivially_swappable_8(_To_address(_UFirst), _To_address(_ULast));
            }

            return;
        }
    }
#endif // _USE_STD_VECTOR_ALGORITHMS

    for (; _UFirst != _ULast && _UFirst != --_ULast; ++_UFirst) {
        _STD iter_swap(_UFirst, _ULast);
    }
}

#if _HAS_CXX17
template <class _ExPo, class _BidIt, _Enable_if_execution_policy_t<_ExPo> = 0>
void reverse(_ExPo&&, _BidIt _First, _BidIt _Last) noexcept /* terminates */ {
    // reverse elements in [_First, _Last)
    // not parallelized as benchmarks show it isn't worth it
    return _STD reverse(_First, _Last);
}
#endif // _HAS_CXX17

__BASE_NAMESPACE_END

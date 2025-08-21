#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

#if base_has_cxx20

template <
    class _FirstInputIterator_,
    class _SecondInputIterator_, 
    class _CompareFunction_>
base_nodiscard constexpr auto lexicographical_compare_three_way(
    const _FirstInputIterator_      firstIterator1,
    const _FirstInputIterator_      lastIterator1,
    const _SecondInputIterator_     firstIterator2,
    const _SecondInputIterator_     lastIterator2,
    _CompareFunction_               compareFunction) -> decltype(compareFunction(*firstIterator1, *firstIterator2))
{
    static_assert(
        _Classify_category<decltype(_Comp(*_First1, *_First2))> != _Comparison_category::_Comparison_category_none,
        "The result of the comparator must have comparison category type (N4988 [alg.three.way]/2).");

    _STD _Adl_verify_range(_First1, _Last1);
    _STD _Adl_verify_range(_First2, _Last2);
    auto _UFirst1 = _STD _Get_unwrapped(_First1);
    const auto _ULast1 = _STD _Get_unwrapped(_Last1);
    auto _UFirst2 = _STD _Get_unwrapped(_First2);
    const auto _ULast2 = _STD _Get_unwrapped(_Last2);

    using _Memcmp_pred = _Lex_compare_three_way_memcmp_classify<decltype(_UFirst1), decltype(_UFirst2), _Cmp>;
    if constexpr (!is_void_v<_Memcmp_pred>) {
        if (!_STD is_constant_evaluated()) {
            const auto _Num1 = static_cast<size_t>(_ULast1 - _UFirst1);
            const auto _Num2 = static_cast<size_t>(_ULast2 - _UFirst2);
            const size_t _Num = (_STD min)(_Num1, _Num2);
#if _USE_STD_VECTOR_ALGORITHMS
            const auto _First1_ptr = _STD to_address(_UFirst1);
            const auto _First2_ptr = _STD to_address(_UFirst2);
            const size_t _Pos = _Mismatch_vectorized<sizeof(*_First1_ptr)>(_First1_ptr, _First2_ptr, _Num);
            if (_Pos == _Num1) {
                return _Pos == _Num2 ? strong_ordering::equal : strong_ordering::less;
            }
            else if (_Pos == _Num2) {
                return strong_ordering::greater;
            }
            else {
                const auto _Val1 = _First1_ptr[_Pos];
                const auto _Val2 = _First2_ptr[_Pos];
                __assume(_Val1 != _Val2); // avoid one comparison
                return _Comp(_Val1, _Val2);
            }
#else // ^^^ _USE_STD_VECTOR_ALGORITHMS / !_USE_STD_VECTOR_ALGORITHMS vvv
            const int _Ans = _STD _Memcmp_count(_UFirst1, _UFirst2, _Num);
            if (_Ans == 0) {
                return _Num1 <=> _Num2;
            }
            else {
                return _Memcmp_pred{}(_Ans, 0);
            }
#endif // ^^^ !_USE_STD_VECTOR_ALGORITHMS ^^^
        }
    }

    for (;;) {
        if (_UFirst1 == _ULast1) {
            return _UFirst2 == _ULast2 ? strong_ordering::equal : strong_ordering::less;
        }

        if (_UFirst2 == _ULast2) {
            return strong_ordering::greater;
        }

        if (const auto _CmpResult = _Comp(*_UFirst1, *_UFirst2); _CmpResult != 0) {
            return _CmpResult;
        }

        ++_UFirst1;
        ++_UFirst2;
    }
}
}

template <
    class _FirstInputIterator_,
    class _SecondInputIterator_>
base_nodiscard constexpr auto lexicographical_compare_three_way(
    const _FirstInputIterator_  firstIterator1,
    const _FirstInputIterator_  lastIterator1,
    const _SecondInputIterator_ firstIterator2,
    const _SecondInputIterator_ lastIterator2)
{

}

__BASE_ALGORITHM_NAMESPACE_END

#endif // base_has_cxx20

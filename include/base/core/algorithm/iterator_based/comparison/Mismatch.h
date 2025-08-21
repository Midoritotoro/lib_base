#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator1_,
	class _Iterator2_, 
	class _Predicate_>
base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	_Iterator1_			firstIterator1,
	const _Iterator1_	lastIterator1,
	_Iterator2_			firstIterator2,
	_Predicate_			predicate)
{

}

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator1_, 
	class _ForwardIterator2_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_Predicate_			predicate) noexcept
{

}

template <
	class _Iterator1_, 
	class _Iterator2_>
base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	const _Iterator1_ firstIterator1,
	const _Iterator1_ lastIterator1,
	const _Iterator2_ firstIterator2)
{

}

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator1_, 
	class _ForwardIterator2_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&&			executionPolicy,
	const _ForwardIterator1_	firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	const _ForwardIterator2_	firstIterator2) noexcept
{

}

template <
	class _Iterator1_,
	class _Iterator2_,
	class _Predicate_>
base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	_Iterator1_ firstIterator1,
	_Iterator1_ lastIterator1,
	_Iterator2_ firstIterator2,
	_Iterator2_ lastIterator2,
	_Predicate_ predicate)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_, 
	class _ForwardIterator2_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator2_	lastIterator2,
	_Predicate_			predicate) noexcept
{

}

template <
	class _Iterator1_,
	class _Iterator2_>
base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	_Iterator1_ firstIterator1,
	_Iterator1_ lastIterator1,
	_Iterator2_ firstIterator2,
	_Iterator2_ lastIterator2)
{
#if !defined(NDEBUG)
	VerifyRange(firstIterator1, lastIterator1);
	VerifyRange(firstIterator2, lastIterator2);
#endif // !defined(NDEBUG)

    if constexpr (type_traits::is_iterator_random_ranges_v<_InIt1> && type_traits::is_iterator_random_ranges_v<_InIt2>) {
        using _CT         = _Common_diff_t<_InIt1, _InIt2>;
        const _CT _Count1 = _ULast1 - _UFirst1;
        const _CT _Count2 = _ULast2 - _UFirst2;
        const auto _Count = static_cast<_Iter_diff_t<_InIt1>>((_STD min)(_Count1, _Count2));
        _ULast1           = _UFirst1 + _Count;
#if _USE_STD_VECTOR_ALGORITHMS
        if constexpr (_Equal_memcmp_is_safe<decltype(_UFirst1), decltype(_UFirst2), _Pr>) {
            if (!_STD _Is_constant_evaluated()) {
                constexpr size_t _Elem_size = sizeof(_Iter_value_t<_InIt1>);

                const size_t _Pos = _STD _Mismatch_vectorized<_Elem_size>(
                    _STD _To_address(_UFirst1), _STD _To_address(_UFirst2), static_cast<size_t>(_Count));

                _UFirst1 += static_cast<_Iter_diff_t<_InIt1>>(_Pos);
                _UFirst2 += static_cast<_Iter_diff_t<_InIt2>>(_Pos);

                _STD _Seek_wrapped(_First2, _UFirst2);
                _STD _Seek_wrapped(_First1, _UFirst1);
                return {_First1, _First2};
            }
        }
#endif // ^^^ _USE_STD_VECTOR_ALGORITHMS ^^^
        while (_UFirst1 != _ULast1 && _Pred(*_UFirst1, *_UFirst2)) {
            ++_UFirst1;
            ++_UFirst2;
        }
    } else {
        while (_UFirst1 != _ULast1 && _UFirst2 != _ULast2 && _Pred(*_UFirst1, *_UFirst2)) {
            ++_UFirst1;
            ++_UFirst2;
        }
    }

    _STD _Seek_wrapped(_First2, _UFirst2);
    _STD _Seek_wrapped(_First1, _UFirst1);
    return {_First1, _First2};
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_ firstIterator1,
	_ForwardIterator1_ lastIterator1,
	_ForwardIterator2_ firstIterator2,
	_ForwardIterator2_ lastIterator2) noexcept
{

}

__BASE_NAMESPACE_END

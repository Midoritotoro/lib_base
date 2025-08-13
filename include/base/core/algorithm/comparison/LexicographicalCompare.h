#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

template <
	class _FirstInputIterator_,
	class _SecondInputIterator_,
	class _Function_>
base_nodiscard base_constexpr_cxx20 bool lexicographical_compare(
	const _FirstInputIterator_	firstIterator1,
	const _FirstInputIterator_	lastIterator1,
	const _SecondInputIterator_ firstIterator2,
	const _SecondInputIterator_ lastIterator2,
	_Function_					function)
{

}

template <
	class _FirstInputIterator_,
	class _SecondInputIterator_>
base_nodiscard base_constexpr_cxx20 bool lexicographical_compare(
	const _FirstInputIterator_	firstIterator1,
	const _FirstInputIterator_	lastIterator1,
	const _SecondInputIterator_ firstIterator2,
	const _SecondInputIterator_ lastIterator2) 
{
	// order [firstIterator1, lastIterator1) vs. [firstIterator2, lastIterator2)
	return lexicographical_compare(
		firstIterator1, lastIterator1,
		firstIterator2, lastIterator2, std::less<>{}
	);
}

#if _HAS_CXX17
_EXPORT_STD template <class _ExPo, class _FwdIt1, class _FwdIt2, class _Pr, _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD bool lexicographical_compare(_ExPo&&, const _FwdIt1 _First1, const _FwdIt1 _Last1, const _FwdIt2 _First2,
	const _FwdIt2 _Last2, _Pr _Pred) noexcept /* terminates */ {
	// order [_First1, _Last1) vs. [_First2, _Last2)
	// not parallelized at present, parallelism expected to be feasible in a future release
	_REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
	_REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
	return _STD lexicographical_compare(_First1, _Last1, _First2, _Last2, _STD _Pass_fn(_Pred));
}

_EXPORT_STD template <class _ExPo, class _FwdIt1, class _FwdIt2, _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD bool lexicographical_compare(_ExPo&&, const _FwdIt1 _First1, const _FwdIt1 _Last1, const _FwdIt2 _First2,
	const _FwdIt2 _Last2) noexcept /* terminates */ {
	// order [_First1, _Last1) vs. [_First2, _Last2)
	// not parallelized at present, parallelism expected to be feasible in a future release
	_REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
	_REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
	return _STD lexicographical_compare(_First1, _Last1, _First2, _Last2);
}
#endif // _HAS_CXX17


//template <
//	class _ExecutionPolicy_,
//	class _FirstInputIterator_,
//	class _SecondInputIterator_,
//	class _Function_,
//	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
//base_nodiscard base_constexpr_cxx20 bool lexicographical_compare(
//	_ExecutionPolicy_&&			executionPolicy,
//	const _FirstInputIterator_	firstIterator1,
//	const _FirstInputIterator_	lastIterator1,
//	const _SecondInputIterator_ firstIterator2,
//	const _SecondInputIterator_ lastIterator2,
//	_Function_					function)
//{
//
//}

__BASE_ALGORITHM_NAMESPACE_END


#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

#include <base/core/type_traits/IteratorCategory.h>
#include <base/core/memory/ToAddress.h>


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
#if !defined(NDEBUG)
	VerifyRange(firstIterator1, lastIterator1);
	VerifyRange(firstIterator2, lastIterator2);
#endif // !defined(NDEBUG)


	using _MemcmpFunction_ = type_traits::_Lexicographical_compare_memcmp_classify<
		_FirstInputIterator_, _SecondInputIterator_, _Function_>;

	if constexpr (!std::is_void_v<_MemcmpFunction_>) {
#if base_has_cxx20
		if (!type_traits::is_constant_evaluated())
#endif // base_has_cxx20
		{
			const auto firstRangeLength		= IteratorsDifference(firstIterator1, lastIterator1);
			const auto secondRangeLength	= IteratorsDifference(firstIterator2, lastIterator2);

			const auto minimum = (std::min)(firstRangeLength, secondRangeLength);

			const auto first1Pointer = memory::ToAddress(firstIterator1);
			const auto first2Pointer = memory::ToAddress(firstIterator2);

			const size_t mismatchPosition = 0;
			// const size_t mismatchPosition = mismatch<sizeof(*first1Pointer)>(first1Pointer, first2Pointer, minimum);

			if (mismatchPosition == secondRangeLength)
				return false;
			else if (mismatchPosition == firstRangeLength)
				return true;
			else
				return function(first1Pointer[mismatchPosition], first2Pointer[mismatchPosition]);
		}
	}

	for (; firstIterator1 != lastIterator1 && firstIterator2 != lastIterator2; ++firstIterator1, (void) ++firstIterator2) {
		if (function(*firstIterator1, *firstIterator2))
			return true;
		
		else if (function(*firstIterator2, *firstIterator1))
			return false;
	}

	return firstIterator1 == lastIterator1 && firstIterator2 != lastIterator2;
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
	return base::algorithm::lexicographical_compare(
		firstIterator1, lastIterator1,
		firstIterator2, lastIterator2, std::less<>{}
	);
}

template <
	class _ExecutionPolicy_,
	class _FirstForwardIterator_,
	class _SecondForwardIterator_,
	class _Function_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard bool lexicographical_compare(
	_ExecutionPolicy_&&,
	const _FirstForwardIterator_	firstIterator1,
	const _FirstForwardIterator_	lastIterator1,
	const _SecondForwardIterator_	firstIterator2,
	const _SecondForwardIterator_	lastIterator2,
	_Function_						function) noexcept
{
	__base_require_parallel_iterator(_FirstForwardIterator_);
	__base_require_parallel_iterator(_SecondForwardIterator_);

	return base::algorithm::lexicographical_compare(
		firstIterator1, lastIterator1, firstIterator2,
		lastIterator2, base::type_traits::passFunction(function)
	);
}

template <
	class _ExecutionPolicy_,
	class _FirstForwardIterator_,
	class _SecondForwardIterator_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard bool lexicographical_compare(
	_ExecutionPolicy_&&,
	const _FirstForwardIterator_	firstIterator1,
	const _FirstForwardIterator_	lastIterator1,
	const _SecondForwardIterator_	firstIterator2,
	const _SecondForwardIterator_	lastIterator2) noexcept
{
	__base_require_parallel_iterator(_FirstForwardIterator_);
	__base_require_parallel_iterator(_SecondForwardIterator_);

	return base::algorithm::lexicographical_compare(
		firstIterator1, lastIterator1, 
		firstIterator2, lastIterator2
	);
}

__BASE_ALGORITHM_NAMESPACE_END


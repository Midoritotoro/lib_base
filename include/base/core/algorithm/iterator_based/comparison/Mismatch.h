#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

#include <base/core/type_traits/CanMemcmpElements.h>
#include <src/core/utility/simd/SimdAlgorithm.h>


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
#if !defined(NDEBUG)
	VerifyRange(firstIterator1, lastIterator1);
	VerifyRange(firstIterator2, lastIterator2);
#endif // !defined(NDEBUG)

    if constexpr (type_traits::is_iterator_random_ranges_v<_Iterator1_> && type_traits::is_iterator_random_ranges_v<_Iterator2_>) {
		const auto firstRangeLength		= IteratorsDifference(firstIterator1, lastIterator1);
		const auto secondRangeLength	= IteratorsDifference(firstIterator2, lastIterator2);

		const auto minimum = (std::min)(firstRangeLength, secondRangeLength);
		lastIterator1 = firstIterator1 + minimum;

		if constexpr (type_traits::equal_memcmp_is_safe_v<_Iterator1_, _Iterator2_, _Predicate_>) {
#if base_has_cxx20
            if (!type_traits::is_constant_evaluated()) 
#endif
			{
				constexpr auto singleElementSize = sizeof(type_traits::IteratorValueType<_Iterator1_>);

                const auto position = std::_Mismatch_vectorized<singleElementSize>(
                    memory::ToAddress(firstIterator1), memory::ToAddress(firstIterator2), static_cast<size_t>(minimum));

				firstIterator1 += position;
				firstIterator2 += position;

                return { firstIterator1, firstIterator2 };
            }
        }

        while (firstIterator1 != lastIterator1 && predicate(*firstIterator1, *firstIterator2))
            ++firstIterator1, ++firstIterator2;  
    } 
	else
        while (firstIterator1 != lastIterator1 && firstIterator2 != lastIterator2 && predicate(*firstIterator1, *firstIterator2))
            ++firstIterator1, ++firstIterator2;   
    

    return { firstIterator1, firstIterator2 };
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

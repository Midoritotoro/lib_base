#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <src/core/utility/algorithm/IteratorWrap.h>

#include <src/core/memory/FillMemsetSafety.h>
#include <src/core/memory/Fill.h>

#include <base/core/utility/Execution.h>
#include <src/core/memory/IteratorsDifference.h>

__BASE_NAMESPACE_BEGIN

template <
	class _ForwardIterator_,
	class _Type_>
CONSTEXPR_CXX20 void fill(
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{
	VerifyRange(firstIterator, lastIterator);
		
	const auto difference	= static_cast<IteratorDifferenceType<_ForwardIterator_>>(
		memory::IteratorsDifference(firstIterator, lastIterator));

#if BASE_HAS_CXX20
	if (is_constant_evaluated() == false)
#endif
	{
		if constexpr (memory::IsFillMemsetSafe<_ForwardIterator_, _Type_>)
			return memory::FillMemset(std::move(firstIterator), value, difference);
		
		else if (memory::IsFillZeroMemsetSafe<_ForwardIterator_, _Type_>)
			if (memory::IsAllBitsZero(value))
				return memory::MemsetZero(std::move(firstIterator), difference);

	}

	for (; firstIterator < lastIterator; unused(++firstIterator))
		*firstIterator = value;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void fill(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{
	// It makes no sense to parallelize using ExecutionPolicy
	unused(executionPolicy);

	return fill(
		std::move(firstIterator), 
		std::move(lastIterator),
		value);
}

template <
	class _OutputIterator_,
	class _DifferenceType_,
	class _Type_>
CONSTEXPR_CXX20 _OutputIterator_ fillN(
	_OutputIterator_		destinationIterator,
	const _DifferenceType_	count,
	const _Type_&			value) noexcept
{
	if (count <= 0)
		return destinationIterator;

	const auto destinationEnd = static_cast<size_t>(destinationIterator + count);

#if BASE_HAS_CXX20
	if (is_constant_evaluated() == false) 
#endif
	{
		if constexpr (memory::IsFillMemsetSafe<_OutputIterator_, _Type_>) {
			memory::FillMemset(destinationIterator, value, static_cast<size_t>(count));
			memory::RewindIterator(destinationIterator, destinationEnd);

			return destinationIterator;
		} 

		else if (memory::IsFillZeroMemsetSafe<_OutputIterator_, _Type_>) {
			if (memory::IsAllBitsZero(value)) {
				memory::MemsetZero(destinationIterator, static_cast<size_t>(count));
				memory::RewindIterator(destinationIterator, destinationEnd);

				return destinationIterator;
			}
		}

	}
	
	for (auto current = 0; current < count; ++current, unused(++destinationIterator))
		*destinationIterator = value;

	memory::RewindIterator(destinationIterator, destinationEnd);
	return destinationIterator;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _DifferenceType_,
	class _Type_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator_ fillN(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	destinationIterator,
	_DifferenceType_	count,
	const _Type_&		value) noexcept
{
	// It makes no sense to parallelize using ExecutionPolicy
	unused(executionPolicy);

	return fillN(
		std::move(destinationIterator),
		count, value);
}

__BASE_NAMESPACE_END

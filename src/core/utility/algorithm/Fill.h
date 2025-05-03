#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <src/core/utility/algorithm/IteratorWrap.h>

#include <src/core/memory/FillMemsetSafety.h>
#include <src/core/memory/Fill.h>

#include <src/core/memory/IteratorsDifference.h>

__BASE_NAMESPACE_BEGIN

template <
	class _ForwardIterator_,
	class _Type_>
CONSTEXPR_CXX20 void fill(
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator,
	const _Type_&			value)
{
	VerifyRange(firstIterator, lastIterator);

	const auto start		= UnwrapIterator(firstIterator);
	const auto end			= UnwrapIterator(lastIterator);
		
	const auto difference	= static_cast<IteratorDifferenceType<_ForwardIterator_>>(
		memory::IteratorsDifference(start, end));

#if BASE_HAS_CXX20
	if (is_constant_evaluated() == false)
#endif
	{
		if constexpr (memory::IsFillMemsetSafe<decltype(start), _Type_>) {
			return memory::FillMemset(start, value, difference);
		}
		else if (memory::IsFillZeroMemsetSafe<decltype(start), _Type_>) {
			if (memory::IsAllBitsZero(value)) {
				return memory::MemsetZero(start, difference);
			}
		}

	}

	for (auto current = start; current < end; ++current)
		*current = value;

	return;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_>
void fill(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{
	// It makes no sense to parallelize using ExecutionPolicy
	UNUSED(executionPolicy);

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
	const _DifferenceType_	_Count,
	const _Type_&			value)
{
	const auto destination		= UnwrapIterator(destinationIterator);
	const auto destinationEnd	= destination + _Count;

	if (_Count <= 0)
		return destinationIterator;

#if BASE_HAS_CXX20
	if (is_constant_evaluated() == false) 
#endif
	{
		if constexpr (memory::IsFillMemsetSafe<decltype(destination), _Type_>) {
			memory::FillMemset(destination, value, static_cast<size_t>(_Count));
			memory::RewindIterator(destinationIterator, destinationEnd);

			return destinationIterator;
		} 

		else if (memory::IsFillZeroMemsetSafe<decltype(destination), _Type_>) {
			if (memory::IsAllBitsZero(value)) {
				memory::MemsetZero(destination, static_cast<size_t>(_Count));
				memory::RewindIterator(destinationIterator, destinationEnd);

				return destinationIterator;
			}
		}

	}
	
	for (auto current = 0; current < _Count; ++current, ++destination)
		*destination = value;

	memory::RewindIterator(destinationIterator, destinationEnd);
	return destinationIterator;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _DifferenceType_,
	class _Type_>
_ForwardIterator_ fillN(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	destinationIterator,
	_DifferenceType_	_Count,
	const _Type_&		value) noexcept
{
	// It makes no sense to parallelize using ExecutionPolicy
	UNUSED(executionPolicy);

	return fillN(
		std::move(destinationIterator),
		_Count, value);
}

__BASE_NAMESPACE_END

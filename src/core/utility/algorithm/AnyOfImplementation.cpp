#include <base/core/utility/Algorithm.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 bool anyOf(
	const _Iterator_	firstIterator,
	const _Iterator_	lastIterator,
	_Predicate_			predicate)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD bool anyOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}


__BASE_NAMESPACE_END
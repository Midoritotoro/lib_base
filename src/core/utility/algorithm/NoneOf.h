#include <src/core/utility/algorithm/AlgorithmDebug.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 bool noneOf(
	const _Iterator_	firstIterator,
	const _Iterator_	lastIterator,
	_Predicate_			predicate)
{
	VerifyRange(firstIterator, lastIterator);

	auto first		= memory::CheckedToChar(firstIterator);
	const auto last = memory::CheckedToConstChar(lastIterator);

	for (; first != last; ++first) {
		if (_Pred(*first)) {
			return false;
		}
	}

	return true;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD bool noneOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}

__BASE_NAMESPACE_END
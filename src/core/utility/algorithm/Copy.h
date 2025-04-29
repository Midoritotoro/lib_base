#include <base/core/utility/Algorithm.h>
#include <base/core/memory/MemoryUtility.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _OutputIterator_,
	class _Predicate_>
CONSTEXPR_CXX20 _OutputIterator_ copyIf(
	_Iterator_			firstIterator,
	_Iterator_			lastIterator,
	_OutputIterator_	destinationIterator,
	_Predicate_			predicate)
{
	VerifyRange(firstIterator, lastIterator);

	const auto first	= UnwrapIterator(firstIterator);
	const auto last		= UnwrapIterator(lastIterator);

	auto destination	= UnwrapIterator(destinationIterator);
	
	for (; first != last; ++first) {
		if (predicate(*first)) {
			*destination = *first;
			++destination;
		}
	}

	memory::RewindIterator(destinationIterator, destination);
	return destinationIterator;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_>
_ForwardIterator2_ copyIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	destinationIterator,
	_Predicate_			predicate) noexcept
{

}

template <
	class _BidirectionalIterator1_,
	class _BidirectionalIterator2_>
CONSTEXPR_CXX20 _BidirectionalIterator2_ copyBackward(
	_BidirectionalIterator1_ firstIterator,
	_BidirectionalIterator1_ lastIterator,
	_BidirectionalIterator2_ destinationIterator)
{
	VerifyRange(firstIterator, lastIterator);

	const auto first	= UnwrapIterator(firstIterator);
	const auto last		= UnwrapIterator(lastIterator);

	const auto destination =
#ifdef CPP_MSVC
		std::_Get_unwrapped_n(
			destinationIterator,
			-std::_Idl_distance<_BidirectionalIterator1_>(first, last));
#else
		UnwrapIterator(destinationIterator);
#endif

	memory::RewindIterator(
		destinationIterator,
		memory::CopyBackwardUnchecked(_UFirst, _ULast, _UDest));

	return destinationIterator;
}


__BASE_NAMESPACE_END

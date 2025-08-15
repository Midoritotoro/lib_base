#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

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

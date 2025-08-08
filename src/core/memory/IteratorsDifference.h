#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <class _ContiguousIterator_>
// Tries to get the difference between contiguous iterators
inline base_nodiscard base_constexpr_cxx20 sizetype IteratorsDifference(
    _ContiguousIterator_ firstIterator,
    _ContiguousIterator_ lastIterator) noexcept
{
    if constexpr (std::is_pointer_v<_ContiguousIterator_>)
        return static_cast<sizetype>(lastIterator - firstIterator);

    return static_cast<sizetype>(
        CheckedToConstChar(lastIterator) - CheckedToConstChar(firstIterator));
}

__BASE_MEMORY_NAMESPACE_END

#pragma once 

#include <base/core/utility/TypeTraits.h>
#include <base/core/utility/Execution.h>

#ifdef base_cpp_msvc
    #include <xutility>
#endif

__BASE_NAMESPACE_BEGIN

template <class _Iterator_>
  constexpr decltype(auto) UnwrapIterator(_Iterator_&& iterator) {
    if constexpr (std::is_pointer_v<std::decay_t<_Iterator_>>)
        return (iterator + 0);
#ifdef base_cpp_msvc
    // For debugging
    else if constexpr (std::_Unwrappable_v<_Iterator_>)
        return static_cast<_Iterator_&&>(iterator)._Unwrapped();
#endif
    else
        return static_cast<_Iterator_&&>(iterator);
}

template <class _Iterator_>
using UnwrappedType = std::remove_cvref_t<
    decltype(UnwrapIterator(
        std::declval<_Iterator_>()))>;

__BASE_NAMESPACE_END

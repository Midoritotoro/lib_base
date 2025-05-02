#pragma once 

#include <base/core/memory/MemoryTypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <
    class _ForwardIterator_,
    class _Type_,
    bool = IteratorIsContiguous<_ForwardIterator_>>
constexpr bool IsFillMemsetSafe = std::conjunction_v<
    std::is_scalar<_Type_>,
    IsCharacterOrByteOrBool<
        unwrap_enum_t<
            std::remove_reference_t<
                std::iter_reference_t<_ForwardIterator_>>>>,
    std::negation<
        std::is_volatile<
            std::remove_reference_t<
                std::iter_reference_t<_ForwardIterator_>>>>, 
    std::is_assignable<
        std::iter_reference_t<_ForwardIterator_>,
    const _Type_ &>>;

template <
    class _ForwardIterator_,
    class _Type_>
constexpr bool IsFillMemsetSafe<_ForwardIterator_, _Type_, false> = false;

template <
    class _ForwardIterator_,
    class _Type_,
    bool = IteratorIsContiguous<_ForwardIterator_>>
constexpr bool IsFillZeroMemsetSafe =
    std::conjunction_v<
        std::is_scalar<_Type_>, 
        std::is_scalar<
            std::iter_value_t<_ForwardIterator_>>,
        std::negation<
            std::is_member_pointer<
                std::iter_value_t<_ForwardIterator_>>>,
        std::negation<
            std::is_volatile<
                std::remove_reference_t<
                    std::iter_reference_t<_ForwardIterator_>>>>, 
        std::is_assignable<std::iter_reference_t<_ForwardIterator_>, 
    const _Type_&>>;

template <
    class _ForwardIterator_,
    class _Type_>
constexpr bool IsFillZeroMemsetSafe<_ForwardIterator_, _Type_, false> = false;

__BASE_MEMORY_NAMESPACE_END

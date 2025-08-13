#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

template <class _Type_>
base_constexpr_cxx20 inline bool IsAllBitsZero(const _Type_& value) {
    static_assert(std::is_scalar_v<_Type_> && !std::is_member_pointer_v<_Type_>);

    if constexpr (std::is_same_v<_Type_, std::nullptr_t>)
        return true;

    constexpr auto zero = _Type_{};
    return memcmp(&value, &zero, sizeof(_Type_)) == 0;
}

template <
    class _ForwardIterator_,
    class _Type_,
    bool = IteratorIsContiguous<_ForwardIterator_>>
constexpr bool IsFillMemsetSafe = std::conjunction_v<
    std::is_scalar<_Type_>,
    IsCharacterOrByteOrBool<
        unwrap_enum_t<
            std::remove_reference_t<
                IteratorReferenceType<_ForwardIterator_>>>>,
    std::negation<
        std::is_volatile<
            std::remove_reference_t<
                IteratorReferenceType<_ForwardIterator_>>>>,
    std::is_assignable<
        IteratorReferenceType<_ForwardIterator_>,
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
            IteratorValueType<_ForwardIterator_>>,
        std::negation<
            std::is_member_pointer<
                IteratorValueType<_ForwardIterator_>>>,
        std::negation<
            std::is_volatile<
                std::remove_reference_t<
                    IteratorReferenceType<_ForwardIterator_>>>>,
        std::is_assignable<IteratorReferenceType<_ForwardIterator_>,
    const _Type_&>>;

template <
    class _ForwardIterator_,
    class _Type_>
constexpr bool IsFillZeroMemsetSafe<_ForwardIterator_, _Type_, false> = false;

__BASE_TYPE_TRAITS_NAMESPACE_END

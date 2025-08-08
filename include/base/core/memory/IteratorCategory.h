#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <
    class _Source_,
    class _Destination_,
    class _SourceReference_,
    class _DestinationReference_>
struct TrivialCategory {
    using UnwrappedSource       = unwrap_enum_t<_Source_>;
    using UnwrappedDestination  = unwrap_enum_t<_Destination_>;

    static constexpr bool SameSizeAndCompatible =
        sizeof(_Source_) == sizeof(_Destination_)
        // If UnwrappedDestination is bool, UnwrappedSource also needs to be bool
        // Conversion from non-bool => non-bool | bool => bool | bool => non-bool is fine.
        // Conversion from non-bool => bool is not fine.
        && std::is_same_v<bool, UnwrappedSource> 
            >= std::is_same_v<bool, UnwrappedDestination>
        && (std::is_same_v<UnwrappedSource, UnwrappedDestination> 
        || (std::is_integral_v<UnwrappedSource> 
                && std::is_integral_v<UnwrappedDestination>)
        || (std::is_floating_point_v<UnwrappedSource> 
            && std::is_floating_point_v<UnwrappedDestination>));

    static constexpr bool BitcopyConstructible =
        SameSizeAndCompatible 
        && std::is_trivially_constructible_v<
            _Destination_, _SourceReference_>;

    static constexpr bool BitcopyAssignable =
        SameSizeAndCompatible 
        && std::is_trivially_assignable_v<
            _DestinationReference_, _SourceReference_>;
};

template <
    class _Source_,
    class _Destination_, 
    class _SourceReference_,
    class _DestinationReference_>
struct TrivialCategory<
    _Source_*, _Destination_*,
    _SourceReference_, _DestinationReference_>
{
    static constexpr bool BitcopyConstructible =
        IsPointerAddressConvertible<_Source_, _Destination_> 
        && std::is_trivially_constructible_v<_Destination_*, _SourceReference_>;

    static constexpr bool BitcopyAssignable =
        IsPointerAddressConvertible<_Source_, _Destination_>
        && std::is_trivially_assignable_v<_DestinationReference_, _SourceReference_>;
};

struct FalseTrivialCategory {
    static constexpr bool BitcopyConstructible = false;
    static constexpr bool BitcopyAssignable = false;
};

template <
    class _SourceIterator_, 
    class _DestinationIterator_,
    bool AreContiguous = IteratorsAreContiguous<
            _SourceIterator_, _DestinationIterator_> 
            && !IsIteratorVolatile<_SourceIterator_>
            && !IsIteratorVolatile<_DestinationIterator_>>
    struct IteratorMoveCategory : 
        TrivialCategory<
            IteratorValueType<_SourceIterator_>, 
            IteratorValueType<_DestinationIterator_>,
    std::remove_reference_t<
        IteratorReferenceType<_SourceIterator_>>&&,
        IteratorReferenceType<_DestinationIterator_>
    > 
{};

template <
    class _SourceIterator_,
    class _DestinationIterator_>
struct IteratorMoveCategory<
    std::move_iterator<_SourceIterator_>, _DestinationIterator_, false> : 
        IteratorMoveCategory<_SourceIterator_, _DestinationIterator_> {
};

template <
    class _SourceIterator_,
    class _DestinationIterator_,
    bool AreContiguous = IteratorsAreContiguous<
    _SourceIterator_, _DestinationIterator_>
    && !IsIteratorVolatile<_SourceIterator_>
    && !IsIteratorVolatile<_DestinationIterator_>>
    struct IteratorCopyCategory :
        TrivialCategory <
            IteratorValueType<_SourceIterator_>,
            IteratorValueType<_DestinationIterator_>,
            IteratorReferenceType<_SourceIterator_>,
            IteratorReferenceType<_DestinationIterator_>
        > 
{};

template <
    class _SourceIterator_,
    class _DestinationIterator_>
struct IteratorCopyCategory<
    _SourceIterator_, _DestinationIterator_, false> : 
        FalseTrivialCategory
{};

template <
    class _SourceIterator_,
    class _DestinationIterator_>
struct IteratorCopyCategory<
    std::move_iterator<_SourceIterator_>, _DestinationIterator_, false> : 
        IteratorMoveCategory<_SourceIterator_, _DestinationIterator_>
{};

template <
    class _InputIterator_,
    class _Sentinel_,
    class _OutIterator_>
using SentinelCopyCategory = std::conditional_t<
#if base_has_cxx20
    std::is_same_v<
        _Sentinel_, _InputIterator_>
    || std::sized_sentinel_for<
        _Sentinel_, _InputIterator_>,
#else
    std::is_same_v<_Sentinel_, _InputIterator_>,
#endif
    IteratorCopyCategory<
        _InputIterator_, _OutIterator_>,
    FalseTrivialCategory>;

__BASE_MEMORY_NAMESPACE_END

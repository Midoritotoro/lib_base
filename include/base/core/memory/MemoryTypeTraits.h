#pragma once 

#include <base/core/utility/TypeTraits.h>
#include <base/core/arch/KeywordSupport.h>

__BASE_MEMORY_NAMESPACE_BEGIN


//#if BASE_HAS_CXX20
//
//    template <typename _Allocator_>
//    struct AllocatorPointerTraits {
//        using type = typename std::allocator_traits<_Allocator_>::pointer;
//    };
//
//    template <typename _Allocator_>
//        requires HasPointerType<_Allocator_>
//    struct AllocatorPointerTraits<_Allocator_> {
//        using type = typename _Allocator_::pointer;
//    };
//
//    template <typename _Allocator_>
//    struct AllocatorConstPointerTraits {
//        using type = typename std::allocator_traits<_Allocator_>::const_pointer;
//    };
//
//    template <typename _Allocator_>
//        requires HasConstPointerType<_Allocator_>
//    struct AllocatorConstPointerTraits<_Allocator_> {
//        using type = typename _Allocator_::const_pointer;
//    };
//
//    template <typename _Allocator_>
//    struct AllocatorValueTraits {
//        using type = typename std::allocator_traits<_Allocator_>::value_type;
//    };
//
//    template <typename _Allocator_>
//        requires HasValueType<_Allocator_>
//    struct AllocatorValueTraits<_Allocator_> {
//        using type = typename _Allocator_::value_type;
//    };
//
//    template <typename _Allocator_>
//    struct AllocatorSizeTraits {
//        using type = typename std::allocator_traits<_Allocator_>::size_type;
//    };
//
//    template <typename _Allocator_>
//        requires HasSizeType<_Allocator_>
//    struct AllocatorSizeTraits<_Allocator_> {
//        using type = typename _Allocator_::size_type;
//    };
//
//    template <typename _Allocator_>
//    using AllocatorPointerType      = typename AllocatorPointerTraits<_Allocator_>::type;
//
//    template <class _Allocator_>
//    using AllocatorConstPointerType = typename AllocatorConstPointerTraits<_Allocator_>::type;
//
//    template <class _Allocator_>
//    using AllocatorSizeType         = typename AllocatorSizeTraits<_Allocator_>::type;
//
//    template <class _Allocator_>
//    using AllocatorValueType        = typename AllocatorValueTraits<_Allocator_>::type;
//
//#else 

    template <class _Allocator_>
    using AllocatorPointerType      = typename std::allocator_traits<_Allocator_>::pointer;

    template <class _Allocator_>
    using AllocatorConstPointerType = typename std::allocator_traits<_Allocator_>::const_pointer;

    template <class _Allocator_>
    using AllocatorSizeType         = typename std::allocator_traits<_Allocator_>::size_type;

    template <class _Allocator_>
    using AllocatorValueType        = typename std::allocator_traits<_Allocator_>::value_type;

    //#endif

#if defined(OS_WIN) && defined(CPP_MSVC)
    template <
        typename _Type_,
        class   _Allocator_>
    constexpr bool CanDestroyRange  = !std::conjunction_v<
        std::is_trivially_destructible<_Type_>,
        std::_Uses_default_destroy<_Allocator_, _Type_*>>;
#else 
    template <
        typename _Type_,
        class   _Allocator_>
    constexpr bool CanDestroyRange = !std::is_trivially_destructible_v<_Type_>;
#endif

template <
    typename _Type_,
    class _Allocator_>
constexpr bool IsNoThrowMoveConstructible =
    #if defined(OS_WIN) && defined(CPP_MSVC)
		    std::conjunction_v<
			    std::is_nothrow_move_constructible<_Type_>,
			    std::_Uses_default_construct<_Allocator_, _Type_*, _Type_>>;
    #else
		    std::is_nothrow_move_constructible_v<_Allocator_>;
    #endif

template <
    class _Source_,
    class _Destination_,
    class _SourceReference_,
    class _DestinationReference_>
struct TrivialCategory {
    using UnwrappedSource = std::_Unwrap_enum_t<_Source_>;
    using UnwrappedDestination = _Unwrap_enum_t<_Destination_>;

    static constexpr bool SameSizeAndCompatible =
        sizeof(_Source_) == sizeof(_Destination_)
        // If UnwrappedDestination is bool, UnwrappedSource also needs to be bool
        // Conversion from non-bool => non-bool | bool => bool | bool => non-bool is fine.
        // Conversion from non-bool => bool is not fine.
        && is_same_v<bool, UnwrappedSource> >= is_same_v<bool, UnwrappedDestination>
        && (is_same_v<UnwrappedSource, UnwrappedDestination> || (is_integral_v<UnwrappedSource> && is_integral_v<UnwrappedDestination>)
            || (is_floating_point_v<UnwrappedSource> && is_floating_point_v<UnwrappedDestination>));

    static constexpr bool BitcopyConstructible =
        SameSizeAndCompatible && is_trivially_constructible_v<_Destination_, _SourceReference_>;

    static constexpr bool BitcopyAssignable =
        SameSizeAndCompatible && is_trivially_assignable_v<_DestinationReference_, _SourceReference_>;
};

template <class _Source_, class _Destination_, class _SourceReference_, class _DestinationReference_>
struct TrivialCategory<_Source_*, _Destination_*, _SourceReference_, _DestinationReference_> {
    static constexpr bool BitcopyConstructible =
        _Is_pointer_address_convertible<_Source_, _Destination_>&& is_trivially_constructible_v<_Destination_*, _SourceReference_>;

    static constexpr bool BitcopyAssignable =
        _Is_pointer_address_convertible<_Source_, _Destination_>&& is_trivially_assignable_v<_DestinationReference_, _SourceReference_>;
};

struct FalseTrivialCategory {
    static constexpr bool BitcopyConstructible = false;
    static constexpr bool BitcopyAssignable = false;
};

template <class _SourceIterator_, class _DestinationIterator_,
    bool AreContiguous = _Iterators_are_contiguous<_SourceIterator_, _DestinationIterator_> && !_Iterator_is_volatile<_SourceIterator_>
    && !_Iterator_is_volatile<_DestinationIterator_>>
    struct IteratorMoveCategory : TrivialCategory<_Iter_value_t<_SourceIterator_>, _Iter_value_t<_DestinationIterator_>,
    remove_reference_t<_Iter_ref_t<_SourceIterator_>>&&, _Iter_ref_t<_DestinationIterator_>> {};

template <class _SourceIterator_, class _DestinationIterator_>
struct IteratorMoveCategory<move_iterator<_SourceIterator_>, _DestinationIterator_, false> : IteratorMoveCategory<_SourceIterator_, _DestinationIterator_> {};

template <class _SourceIterator_, class _DestinationIterator_,
    bool AreContiguous = _Iterators_are_contiguous<_SourceIterator_, _DestinationIterator_> && !_Iterator_is_volatile<_SourceIterator_>
    && !_Iterator_is_volatile<_DestinationIterator_>>
    struct IteratorCopyCategory
    : TrivialCategory<_Iter_value_t<_SourceIterator_>, _Iter_value_t<_DestinationIterator_>, _Iter_ref_t<_SourceIterator_>, _Iter_ref_t<_DestinationIterator_>> {};

template <class _SourceIterator_, class _DestinationIterator_>
struct IteratorCopyCategory<_SourceIterator_, _DestinationIterator_, false> : FalseTrivialCategory {};

template <class _SourceIterator_, class _DestinationIterator_>
struct IteratorCopyCategory<move_iterator<_SourceIterator_>, _DestinationIterator_, false> : IteratorMoveCategory<_SourceIterator_, _DestinationIterator_> {};
__BASE_MEMORY_NAMESPACE_END
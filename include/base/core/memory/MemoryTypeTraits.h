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
    class _Destination_>
// checks the convertibility of _Source_ to _Destination_
constexpr bool IsPointerAddressConvertible = std::is_void_v<_Source_>
    || std::is_void_v<_Destination_>
    // is_same_v is required for function pointers to work
    || std::is_same_v<
        std::remove_cv_t<_Source_>, 
        std::remove_cv_t<_Destination_>>
#ifdef __cpp_lib_is_pointer_interconvertible
    || std::is_pointer_interconvertible_base_of_v<_Destination_, _Source_>
#endif
    ;

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

template <
    class _Allocator_,
    class _ValueType_>
using RebindAllocator = 
    typename std::allocator_traits<_Allocator_>
        ::template rebind_alloc<_ValueType_>;

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
            std::iter_value_t<_SourceIterator_>, 
            std::iter_value_t<_DestinationIterator_>,
    std::remove_reference_t<
        std::iter_reference_t<_SourceIterator_>>&&,
        std::iter_reference_t<_DestinationIterator_>
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
        TrivialCategory<
            std::iter_value_t<_SourceIterator_>, 
            std::iter_value_t<_DestinationIterator_>, 
            std::iter_reference_t<_SourceIterator_>, 
            std::iter_reference_t<_DestinationIterator_>
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
#if BASE_HAS_CXX20
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
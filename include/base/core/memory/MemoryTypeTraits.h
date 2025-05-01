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
    typename    _Type_,
    class       _Allocator_>
constexpr bool IsNoThrowMoveConstructible =
    #if defined(OS_WIN) && defined(CPP_MSVC)
		    std::conjunction_v<
			    std::is_nothrow_move_constructible<_Type_>,
			    std::_Uses_default_construct<_Allocator_, _Type_*, _Type_>>;
    #else
		    std::is_nothrow_move_constructible_v<_Allocator_>;
    #endif

template <
    typename    _Type_,
    class       _Allocator_,
    class ...   _Args_>
constexpr bool IsNoThrowMoveConstructibleArgs =
    #if defined(OS_WIN) && defined(CPP_MSVC)
		    std::conjunction_v<
			    std::is_nothrow_move_constructible<_Type_>,
			    std::_Uses_default_construct<_Allocator_, _Type_*, _Args_...>>;
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
    std::size_t A,
    std::size_t B>
struct MinimumSize: 
    std::integral_constant<std::size_t, (A < B) ? A : B> 
{};

template <
    std::size_t A,
    std::size_t B>
struct MaximumSize: 
    std::integral_constant<std::size_t, (A > B) ? A : B> 
{};

template <class _Type_>
struct MaximumObjects: 
    std::integral_constant<std::size_t,
        ~static_cast<std::size_t>(0) / sizeof(_Type_)> 
{};


__BASE_MEMORY_NAMESPACE_END
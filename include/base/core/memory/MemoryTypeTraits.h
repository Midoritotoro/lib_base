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

__BASE_MEMORY_NAMESPACE_END
#pragma once 

#include <type_traits>

#include <base/core/BaseNamespace.h>

#include <base/core/compatibility/CxxVersionDetection.h>
#include <base/core/compatibility/FunctionAttributes.h>

#include <base/core/compatibility/Nodiscard.h>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

template <class _Function_>
// pass function object by value as a reference
// FunctionReference is an aggregate so it can be enregistered, unlike reference_wrapper
struct FunctionReference {
    _Function_& _function;

    template <class ... _Args_>
    constexpr decltype(auto) operator()(_Args_&& ... values) 
        noexcept(std::is_nothrow_invocable_v<_Function_&, _Args_...>)
    {
        if constexpr (std::is_member_pointer_v<_Function_>)
            return std::invoke(_function, std::forward<_Args_>(values)...);
        else
            return _function(std::forward<_Args_>(values)...);
    }
};

template <class _Function_>
base_nodiscard constexpr auto passFunction(_Function_& function) noexcept
{
    constexpr bool passByValue = std::conjunction_v<std::bool_constant<sizeof(_Function_) <= sizeof(void*)>,
        std::is_trivially_copy_constructible<_Function_>, std::is_trivially_destructible<_Function_>>;

    if constexpr (passByValue)
        return function;
    else
        return FunctionReference{ function }; // pass functor by "reference"
}

__BASE_TYPE_TRAITS_NAMESPACE_END

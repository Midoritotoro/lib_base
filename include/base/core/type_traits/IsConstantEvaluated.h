#pragma once 

#include <base/core/BaseNamespace.h>
#include <type_traits>

__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

#ifdef __cpp_lib_is_constant_evaluated
using std::is_constant_evaluated;
#define SUPPORTS_IS_CONSTANT_EVALUATED
#else
constexpr bool is_constant_evaluated() noexcept
{
#if 0
	return false;
#elif __has_builtin(__builtin_is_constant_evaluated)
# define SUPPORTS_IS_CONSTANT_EVALUATED
	return __builtin_is_constant_evaluated();
#else
	return false;
#endif
}
#endif // __cpp_lib_is_constant_evaluated

__BASE_TYPE_TRAITS_NAMESPACE_END

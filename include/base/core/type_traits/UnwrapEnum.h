#pragma once 

#include <type_traits>
#include <base/core/BaseNamespace.h>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

template <
	class	_Element_,
	bool	_IsEnum_ = std::is_enum_v<_Element_>>
	struct _Unwrap_enum {
	// if _Element_ is an enum, gets its underlying type;
	// otherwise leaves _Element_ unchanged
	using type = std::underlying_type_t<_Element_>;
};

template <class _Element_>
struct _Unwrap_enum<_Element_, false> {
	// passthrough non-enum type
	using type = _Element_;
};

template <class _Element_>
using unwrap_enum_t = typename _Unwrap_enum<_Element_>::type;

__BASE_TYPE_TRAITS_NAMESPACE_END

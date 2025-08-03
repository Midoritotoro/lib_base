#pragma once 

#include <src/core/memory/AddressOf.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if BASE_HAS_CXX20
	template <
		class		_Type_,
		class...	_Types_>
	requires requires(
		_Type_*       location,
		_Types_&&...  args)
	{
		::new (static_cast<void*>(location))
			_Type_(std::forward<_Types_>(args)...);
	}
	constexpr _Type_* ConstructAt(
		_Type_* const	location,
		_Types_&&...	args) noexcept(
			noexcept(
				::new(static_cast<void*>(location))
				_Type_(std::forward<_Types_>(args)...)))
	{
		return ::new (static_cast<void*>(location))
				_Type_(std::forward<_Types_>(args)...);
	}
#endif

template <
    class		_Type_, 
    class...	_Types_>
CONSTEXPR_CXX20 inline void ConstructInPlace(
    _Type_&         object, 
    _Types_&&...    args) noexcept(
        std::is_nothrow_constructible_v<_Type_, _Types_...>) 
{
#if BASE_HAS_CXX20
    if (is_constant_evaluated())
        ConstructAt(AddressOf(object), std::forward<_Types_>(args)...);
    else
#endif
        ::new (static_cast<void*>(AddressOf(object)))
            _Type_(std::forward<_Types_>(args)...);
}

__BASE_MEMORY_NAMESPACE_END

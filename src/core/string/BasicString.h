#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/string/StringIterator.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	class _Char_,
	class _Traits_,
	class _Allocator_ = std::allocator<_Char_>>
class BasicString 
{
public:
	using value_type		= _Char_;

	using reference			= _Char_&;
	using const_reference	= const _Char_&;

	using pointer			= _Char_*;
	using const_pointer		= const _Char_*;

	using size_type			= sizetype;
	using difference_type	= ptrdiff;

	using iterator			= StringIterator<
		BasicString<_Char_, _Traits_, _Allocator_>>;
	using const_iterator	= StringConstIterator<
		BasicString<_Char_, _Traits_, _Allocator_>>;


};

__BASE_STRING_NAMESPACE_END
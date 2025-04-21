#pragma once 

#include <base/core/utility/Algorithm.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

template <class _Type_>
class VectorValue {
public:
	using value_type		= typename _Type_::value_type;
	using size_type			= typename _Type_::size_type;

	using difference_type	= typename _Type_::difference_type;
	using pointer			= typename _Type_::pointer;

	using const_pointer		= typename _Type_::const_pointer;

	using reference			= value_type&;
	using const_reference	= const value_type&;

	CONSTEXPR_CXX20 inline VectorValue() noexcept
	{}

	CONSTEXPR_CXX20 inline VectorValue(
		pointer start, 
		pointer end,
		pointer current) noexcept
	: 
		_start(start),
		_end(end),
		_current(_current) 
	{}

	CONSTEXPR_CXX20 inline void swap(VectorValue& other) noexcept {
		std::swap(_start, other._start);
		std::swap(_end, other._end);
		std::swap(_current, other._current);
	}

	CONSTEXPR_CXX20 inline void takeContents(VectorValue& other) noexcept {
		_start = other._start;
		_end = other._end;
		_current = other._current;

		other._start = nullptr;
		other._end = nullptr;
		other._current = nullptr;
	}

	pointer _start		= nullptr;
	pointer _end		= nullptr;

	pointer _current	= nullptr;
};

__BASE_CONTAINER_NAMESPACE_END
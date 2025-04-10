#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/utility/TypeTraits.h>


__BASE_NAMESPACE_BEGIN

template <
	typename _TypeFrom_,
	typename _TypeTo_,
	typename = std::enable_if_t<
		std::is_integral_v<_TypeFrom_> 
		&& std::is_same_v<_TypeFrom_, _TypeTo_>>>
constexpr inline NODISCARD _TypeTo_ ConvertSize(
	const _TypeFrom_	_From,
	const _TypeTo_		_To) noexcept
{
	return _From;
}

template <
	typename _TypeFrom_,
	typename _TypeTo_,
	typename = std::enable_if_t<
		std::is_integral_v<_TypeFrom_>
		&& std::is_integral_v<_TypeTo_>
		&& !std::is_same_v<_TypeFrom_, _TypeTo_>>>
constexpr inline NODISCARD _TypeTo_ ConvertSize(
	const _TypeFrom_	_From,
	const _TypeTo_		_To) noexcept
{
	if constexpr (is_signed_type_v<_TypeFrom_> && is_signed_type_v<_TypeTo_>) {
		
	} 
	else if (is_unsigned_type_v<_TypeFrom_> && is_unsigned_type_v<_TypeTo_>) {

	}
	else if (is_signed_type_v<_TypeFrom_> && is_unsigned_type_v<_TypeTo_>) {

	}
	else /* is_unsigned_type_v<_TypeFrom_> && is_signed_type_v<_TypeTo_> */ {

	}
}

__BASE_NAMESPACE_END
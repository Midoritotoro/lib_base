#pragma once 

#include <base/core/arch/Platform.h>

__BASE_NAMESPACE_BEGIN

template <class _Type_>
constexpr inline   _Type_ MinimumIntegralLimit() noexcept {
	// same as (numeric_limits<_Ty>::min)(), less throughput cost
	static_assert(std::is_integral_v<_Type_>);

	if constexpr (std::is_unsigned_v< _Type_>)
		return 0;
	
	constexpr auto _UnsignedMax = static_cast<std::make_unsigned_t<_Type_>>(-1);
	return static_cast<_Type_>((_UnsignedMax >> 1) + 1); // well-defined, N4950 [conv.integral]/3
}

template <class _Type_>
constexpr inline   _Type_ MaximumIntegralLimit() noexcept { 
	// same as (numeric_limits<_Ty>::max)(), less throughput cost
	static_assert(std::is_integral_v<_Type_>);

	if constexpr (std::is_unsigned_v<_Type_>)
		return static_cast<_Type_>(-1);
	
	constexpr auto _UnsignedMax = static_cast<std::make_unsigned_t<_Type_>>(-1);
	return static_cast<_Type_>(_UnsignedMax >> 1);
}

// false if the _TypeFrom_ type cannot be converted to _TypeTo without _From data loss
template <
	typename _TypeTo_,
	typename _TypeFrom_,
	typename = std::enable_if_t<
		std::is_integral_v<_TypeFrom_>
		&& std::is_integral_v<_TypeTo_>
		&& !std::is_same_v<_TypeFrom_, _TypeTo_>>>
constexpr inline   bool ConvertIntegral(
	const _TypeFrom_	_From,
	_TypeTo_&			_To) noexcept
{
	if constexpr (std::is_same_v<_TypeFrom_, _TypeTo_>) {
		_To = _From;
		return true;
	}

	constexpr auto _ToMaximumLimit = MaximumIntegralLimit<_TypeTo_>();
	constexpr auto _ToMinimumLimit = MinimumIntegralLimit<_TypeTo_>();

	if constexpr (std::is_signed_v<_TypeFrom_> && std::is_signed_v<_TypeTo_>)
		if constexpr (sizeof(_TypeFrom_) > sizeof(_TypeTo_))
			if (_From > _ToMaximumLimit || _From < _ToMinimumLimit)
				return false;
	
	else if (std::is_unsigned_v<_TypeFrom_> && std::is_unsigned_v<_TypeTo_>)
		if constexpr (sizeof(_TypeFrom_) > sizeof(_TypeTo_))
			if (_From > _ToMaximumLimit)
				return false;
	
	else if (std::is_signed_v<_TypeFrom_> && std::is_unsigned_v<_TypeTo_>)
		if constexpr (sizeof(_TypeFrom_) > sizeof(_TypeTo_))
			if (_From < _ToMinimumLimit)
				return false;
	
	else /* std::is_unsigned_v<_TypeFrom_> && std::is_signed_v<_TypeTo_> */
		if constexpr (sizeof(_TypeFrom_) > sizeof(_TypeTo_))
			if (_From > _ToMaximumLimit)
				return false;

	_To = static_cast<_TypeTo_>(_From);

	return true;
}

__BASE_NAMESPACE_END
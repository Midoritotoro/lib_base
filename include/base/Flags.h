#pragma once


#include "base/Platform.h"
#include <type_traits>


namespace core {

	template <typename EnumType>
	class Flags;

	template <typename ExtendedEnum>
	struct extended_flags;

	template <typename ExtendedEnum>
	using extended_flags_t = typename extended_flags<ExtendedEnum>::type;

	namespace details {

		struct flags_zero_helper_struct {
		};

		using flags_zero_helper = void(core::details::flags_zero_helper_struct::*)();

		template <typename ExtendedEnum,
			typename Enum = typename core::extended_flags<ExtendedEnum>::type>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto extendedFlagConvert(ExtendedEnum value) 
		{
			return static_cast<Enum>(value);
		}

		template <typename ExtendedEnum,
			typename Enum = typename core::extended_flags<ExtendedEnum>::type>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto extendedFlagsConvert(ExtendedEnum value)
		{
			return Flags<Enum>(extendedFlagConvert(value));
		}

	} // namespace details

	template <typename EnumType>
	class Flags {
	public:
		using Enum = EnumType;
		using Type = std::underlying_type_t<Enum>;

		constexpr Flags() = default;

		constexpr Flags(details::flags_zero_helper) noexcept 
		{}

		constexpr Flags(Enum value) noexcept
			: _value(static_cast<Type>(value)) {
		}

		always_inline static constexpr Flags fromRaw(Type value) noexcept {
			return Flags(static_cast<Enum>(value));
		}

		always_inline constexpr auto value() const noexcept {
			return _value;
		}

		always_inline constexpr operator Type() const noexcept {
			return value();
		}

		always_inline constexpr auto& operator|=(Flags b) noexcept {
			_value |= b.value();
			return *this;
		}

		always_inline constexpr auto& operator&=(Flags b) noexcept {
			_value &= b.value();
			return *this;
		}

		always_inline constexpr auto& operator^=(Flags b) noexcept {
			_value ^= b.value();
			return *this;
		}

		always_inline constexpr auto operator~() const noexcept {
			return fromRaw(~value());
		}

		always_inline constexpr auto operator|(Flags b) const noexcept {
			return (Flags(*this) |= b);
		}

		always_inline constexpr auto operator&(Flags b) const noexcept {
			return (Flags(*this) &= b);
		}

		always_inline constexpr auto operator^(Flags b) const noexcept {
			return (Flags(*this) ^= b);
		}

		always_inline constexpr auto operator|(Enum b) const noexcept {
			return (Flags(*this) |= b);
		}

		always_inline constexpr auto operator&(Enum b) const noexcept {
			return (Flags(*this) &= b);
		}

		always_inline constexpr auto operator^(Enum b) const noexcept {
			return (Flags(*this) ^= b);
		}

		always_inline constexpr auto operator==(Enum b) const noexcept {
			return (value() == static_cast<Type>(b));
		}

		always_inline constexpr auto operator!=(Enum b) const noexcept {
			return !(*this == b);
		}

		always_inline constexpr auto operator<(Enum b) const noexcept {
			return value() < static_cast<Type>(b);
		}

		always_inline constexpr auto operator>(Enum b) const noexcept {
			return (b < *this);
		}

		always_inline constexpr auto operator<=(Enum b) const noexcept {
			return !(b < *this);
		}

		always_inline constexpr auto operator>=(Enum b) const noexcept {
			return !(*this < b);
		}
	private:
		Type _value = 0;
	};

	template <typename Enum>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto makeFlags(Enum value) noexcept
	{
		return Flags<Enum>(value);
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t < is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto operator|(Enum a, Flags<Enum> b) noexcept
	{
		return b | a;
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t < is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto operator&(Enum a, Flags<Enum> b) noexcept
	{
		return b & a;
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t < is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto operator^(Enum a, Flags<Enum> b) noexcept
	{
		return b ^ a;
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator|(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return a | details::extendedFlagsConvert(b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]]
		constexpr auto operator|(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return b | a;
	}

	template <typename ExtendedEnum,
		typename = extended_flags_t<ExtendedEnum>>
		LIB_BASE always_inline [[nodiscard]]
			constexpr auto operator&(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return a & details::extendedFlagsConvert(b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator&(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return b & a;
	}

	template <typename ExtendedEnum,
		typename = extended_flags_t<ExtendedEnum>>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto operator^(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return a ^ details::extendedFlagsConvert(b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator^(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return b ^ a;
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto& operator&=(Flags<extended_flags_t<ExtendedEnum>>& a, ExtendedEnum b)
	{
		return (a &= details::extendedFlagsConvert(b));
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto& operator|=(Flags<extended_flags_t<ExtendedEnum>>& a, ExtendedEnum b)
	{
		return (a |= details::extendedFlagsConvert(b));
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto& operator^=(Flags<extended_flags_t<ExtendedEnum>>& a, ExtendedEnum b)
	{
		return (a ^= details::extendedFlagsConvert(b));
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator==(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return a == details::extendedFlagsConvert(b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator==(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return (b == a);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator!=(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return !(a == b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator!=(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return !(a == b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator<(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return a < details::extendedFlagsConvert(b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator<(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return details::extendedFlagsConvert(a) < b;
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator>(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return (b < a);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator>(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return (b < a);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator<=(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return !(b < a);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator<=(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return !(b < a);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator>=(Flags<extended_flags_t<ExtendedEnum>> a, ExtendedEnum b)
	{
		return !(a < b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator>=(ExtendedEnum a, Flags<extended_flags_t<ExtendedEnum>> b)
	{
		return !(a < b);
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t < is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto operator!(Enum a) noexcept
	{
		return !makeFlags(a);
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t < is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]]
			constexpr auto operator~(Enum a) noexcept
	{
		return ~makeFlags(a);
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t<is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]] 
			constexpr auto operator|(Enum a, Enum b) noexcept
	{
		return makeFlags(a) | b;
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t < is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]]
			constexpr auto operator|(Enum a, details::flags_zero_helper) noexcept
	{
		return makeFlags(a);
	}

	template <typename Enum,
		typename = std::enable_if_t<std::is_enum<Enum>::value>,
		typename = std::enable_if_t < is_flag_type(Enum{}) >>
		LIB_BASE always_inline [[nodiscard]]
			constexpr auto operator|(details::flags_zero_helper, Enum b) noexcept
	{
		return makeFlags(b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]]
		constexpr auto operator|(ExtendedEnum a, ExtendedEnum b)
	{
		return details::extendedFlagsConvert(a) | b;
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator|(ExtendedEnum a, typename extended_flags<ExtendedEnum>::type b)
	{
		return details::extendedFlagsConvert(a) | b;
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator|(typename extended_flags<ExtendedEnum>::type a, ExtendedEnum b)
	{
		return b | a;
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator|(details::flags_zero_helper, ExtendedEnum b)
	{
		return 0 | details::extendedFlagConvert(b);
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator|(ExtendedEnum a, details::flags_zero_helper)
	{
		return details::extendedFlagConvert(a) | 0;
	}

	template <typename ExtendedEnum,
		typename = typename extended_flags<ExtendedEnum>::type>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator~(ExtendedEnum b)
	{
		return ~details::extendedFlagConvert(b);
	}

} // namespace core

template <typename Enum,
	typename = std::enable_if_t<std::is_enum<Enum>::value>,
	typename = std::enable_if_t < is_flag_type(Enum{}) >>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator!(Enum a) noexcept
{
	return !core::makeFlags(a);
}

template <typename Enum,
	typename = std::enable_if_t<std::is_enum<Enum>::value>,
	typename = std::enable_if_t < is_flag_type(Enum{}) >>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator~(Enum a) noexcept
{
	return ~core::makeFlags(a);
}

template <typename Enum,
	typename = std::enable_if_t<std::is_enum<Enum>::value>,
	typename = std::enable_if_t < is_flag_type(Enum{}) >>
	LIB_BASE always_inline [[nodiscard]]
		constexpr auto operator|(Enum a, Enum b) noexcept
{
	return core::makeFlags(a) | b;
}

template <typename Enum,
	typename = std::enable_if_t<std::is_enum<Enum>::value>,
	typename = std::enable_if_t < is_flag_type(Enum{}) >>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator|(Enum a, core::details::flags_zero_helper) noexcept
{
	return core::makeFlags(a);
}

template <typename Enum,
	typename = std::enable_if_t<std::is_enum<Enum>::value>,
	typename = std::enable_if_t < is_flag_type(Enum{}) >>
	LIB_BASE always_inline [[nodiscard]] 
		constexpr auto operator|(core::details::flags_zero_helper, Enum b) noexcept
{
	return core::makeFlags(b);
}

template <typename ExtendedEnum,
	typename = typename core::extended_flags<ExtendedEnum>::type>
LIB_BASE always_inline [[nodiscard]]
	constexpr auto operator|(ExtendedEnum a, ExtendedEnum b)
{
	return core::details::extendedFlagsConvert(a) | b;
}

template <typename ExtendedEnum,
	typename = typename core::extended_flags<ExtendedEnum>::type>
LIB_BASE always_inline [[nodiscard]]
	constexpr auto operator|(ExtendedEnum a, typename core::extended_flags<ExtendedEnum>::type b)
{
	return core::details::extendedFlagsConvert(a) | b;
}

template <typename ExtendedEnum,
	typename = typename core::extended_flags<ExtendedEnum>::type>
LIB_BASE always_inline [[nodiscard]]
	constexpr auto operator|(typename core::extended_flags<ExtendedEnum>::type a, ExtendedEnum b)
{
	return b | a;
}

template <typename ExtendedEnum,
	typename = typename core::extended_flags<ExtendedEnum>::type>
LIB_BASE always_inline [[nodiscard]]
	constexpr auto operator|(core::details::flags_zero_helper, ExtendedEnum b)
{
	return 0 | core::details::extendedFlagConvert(b);
}

template <typename ExtendedEnum,
	typename = typename core::extended_flags<ExtendedEnum>::type>
LIB_BASE always_inline [[nodiscard]]
	constexpr auto operator|(ExtendedEnum a, core::details::flags_zero_helper)
{
	return core::details::extendedFlagConvert(a) | 0;
}

template <typename ExtendedEnum,
	typename = typename core::extended_flags<ExtendedEnum>::type>
LIB_BASE always_inline [[nodiscard]]
	constexpr auto operator~(ExtendedEnum b)
{
	return ~core::details::extendedFlagsConvert(b);
}

#define DECLARE_FLAGS(_Flags, _Enum) \
	typedef core::Flags<_Enum> _Flags;

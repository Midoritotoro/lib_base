#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/core/string/StringConfig.h>

__BASE_STRING_NAMESPACE_BEGIN

static class StringConverter {
public:
	template <class _String_>
	static inline constexpr bool IsSupportedString = 
		IsAnyOf_v<
			_String_, std::string, std::wstring,
			std::u8string, std::u16string, std::u32string>;

	template <
		class _StringType_,
		std::enable_if_t<IsSupportedString<_StringType_>>>
	NODISCARD std::wstring convertToStdWString(const _StringType_& string);

	template <
		class _StringType_,
		std::enable_if_t<IsSupportedString<_StringType_>>>
	NODISCARD std::string convertToStdString(const _StringType_& string);

	template <
		class _StringType_,
		std::enable_if_t<IsSupportedString<_StringType_>>>
	NODISCARD std::u8string convertToStdUtf8String(const _StringType_& string);

	template <
		class _StringType_,
		std::enable_if_t<IsSupportedString<_StringType_>>>
	NODISCARD std::u16string convertToStdUtf16String(const _StringType_& string);

	template <
		class _StringType_,
		std::enable_if_t<IsSupportedString<_StringType_>>>
	NODISCARD std::u32string convertToStdUtf32String(const _StringType_& string);
private:
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdStringToStdWString(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf8StringToStdWString(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf16StringToStdWString(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf32StringToStdWString(const std::u32string& string);

	CONSTEXPR_CXX20 NODISCARD std::string convertStdWStringToStdString(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf8StringToStdString(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf16StringToStdString(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf32StringToStdString(const std::u32string& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdWStringToStdUtf8String(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdStringToStdUtf8String(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf16StringToStdUtf8String(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf32StringToStdUtf8String(const std::u32string& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdWStringToStdUtf16String(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf8StringToStdUtf16String(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf16StringToStdUtf16String(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf32StringToStdUtf16String(const std::u32string& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdWStringToStdUtf32String(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf8StringToStdUtf32String(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf16StringToStdUtf32String(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf32StringToStdUtf32String(const std::u16string& string);
};

__BASE_STRING_NAMESPACE_END
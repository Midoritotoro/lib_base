#include <src/core/string/StringConverter.h>

__BASE_STRING_NAMESPACE_BEGIN


template <
	class _StringType_, 
	std::enable_if_t<StringConverter::IsSupportedString<_StringType_>>>
std::wstring StringConverter::convertToStdWString(const _StringType_& string) {
	if constexpr (std::is_same_v <_StringType_, std::wstring>)
		return string;

	else if (std::is_same_v<_StringType_, std::string>)
		return convertStdStringToStdWString(string);

	else if (std::is_same_v<_StringType_, std::u8string>)
		return convertStdUtf8StringToStdWString(string);

	else if (std::is_same_v<_StringType_, std::u16string>)
		return convertStdUtf16StringToStdWString(string);

	else if (std::is_same_v<_StringType_, std::u32string>)
		return convertStdUtf32StringToStdWString(string);

	AssertUnreachable();
	return {};
}

template <
	class _StringType_,
	std::enable_if_t<StringConverter::IsSupportedString<_StringType_>>>
std::string StringConverter::convertToStdString(const _StringType_& string) {
	if constexpr (std::is_same_v <_StringType_, std::string>)
		return string;

	else if (std::is_same_v<_StringType_, std::wstring>)
		return convertStdWStringToStdString(string);

	else if (std::is_same_v<_StringType_, std::u8string>)
		return convertStdUtf8StringToStdString(string);

	else if (std::is_same_v<_StringType_, std::u16string>)
		return convertStdUtf16StringToStdString(string);

	else if (std::is_same_v<_StringType_, std::u32string>)
		return convertStdUtf32StringToStdString(string);

	AssertUnreachable();
	return {};
}

template <
	class _StringType_,
	std::enable_if_t<StringConverter::IsSupportedString<_StringType_>>>
std::u8string StringConverter::convertToStdUtf8String(const _StringType_& string) {
	if constexpr (std::is_same_v<_StringType_, std::u8string>)
		return string;

	else if (std::is_same_v<_StringType_, std::wstring>)
		return convertStdWStringToStdUtf8String(string);

	else if (std::is_same_v<_StringType_, std::string>)
		return convertStdStringToStdUtf8String(string);

	else if (std::is_same_v<_StringType_, std::u16string>)
		return convertStdUtf16StringToStdUtf8String(string);

	else if (std::is_same_v<_StringType_, std::u32string>)
		return convertStdUtf32StringToStdUtf8String(string);

	AssertUnreachable();
	return {};
}

template <
	class _StringType_,
	std::enable_if_t<StringConverter::IsSupportedString<_StringType_>>>
std::u16string StringConverter::convertToStdUtf16String(const _StringType_& string) {
	if constexpr (std::is_same_v<_StringType_, std::u16string>)
		return string;

	else if (std::is_same_v<_StringType_, std::wstring>)
		return convertStdWStringToStdUtf16String(string);

	else if (std::is_same_v<_StringType_, std::string>)
		return convertStdStringToStdUtf16String(string);

	else if (std::is_same_v<_StringType_, std::u8string>)
		return convertStdUtf8StringToStdUtf16String(string);

	else if (std::is_same_v<_StringType_, std::u32string>)
		return convertStdUtf32StringToStdUtf16String(string);

	AssertUnreachable();
	return {};
}

template <
	class _StringType_,
	std::enable_if_t<StringConverter::IsSupportedString<_StringType_>>>
std::u32string StringConverter::convertToStdUtf32String(const _StringType_& string) {
	if constexpr (std::is_same_v<_StringType_, std::u32string>)
		return string;

	else if (std::is_same_v<_StringType_, std::wstring>)
		return convertStdWStringToStdUtf32String(string);

	else if (std::is_same_v<_StringType_, std::string>)
		return convertStdStringToStdUtf32String(string);

	else if (std::is_same_v<_StringType_, std::u8string>)
		return convertStdUtf8StringToStdUtf32String(string);

	else if (std::is_same_v<_StringType_, std::u16string>)
		return convertStdUtf16StringToStdUtf32String(string);

	AssertUnreachable();
	return {};
}

CONSTEXPR_CXX20 std::wstring StringConverter::convertStdStringToStdWString(const std::string& string) {

}

CONSTEXPR_CXX20 std::wstring StringConverter::convertStdUtf8StringToStdWString(const std::u8string& string) {

}

CONSTEXPR_CXX20 std::wstring StringConverter::convertStdUtf16StringToStdWString(const std::u16string& string) {

}

CONSTEXPR_CXX20 std::wstring StringConverter::convertStdUtf32StringToStdWString(const std::u32string& string) {

}

CONSTEXPR_CXX20 std::string StringConverter::convertStdWStringToStdString(const std::wstring& string) {

}

CONSTEXPR_CXX20 std::string StringConverter::convertStdUtf8StringToStdString(const std::u8string& string) {

}

CONSTEXPR_CXX20 std::string StringConverter::convertStdUtf16StringToStdString(const std::u16string& string) {

}

CONSTEXPR_CXX20 std::string StringConverter::convertStdUtf32StringToStdString(const std::u32string& string) {

}

CONSTEXPR_CXX20 std::u8string StringConverter::convertStdWStringToStdUtf8String(const std::wstring& string) {

}

CONSTEXPR_CXX20 std::u8string StringConverter::convertStdStringToStdUtf8String(const std::string& string) {

}

CONSTEXPR_CXX20 std::u8string StringConverter::convertStdUtf16StringToStdUtf8String(const std::u16string& string) {

}

CONSTEXPR_CXX20 std::u8string StringConverter::convertStdUtf32StringToStdUtf8String(const std::u32string& string) {

}

CONSTEXPR_CXX20 std::u16string StringConverter::convertStdWStringToStdUtf16String(const std::wstring& string) {

}

CONSTEXPR_CXX20 std::u16string StringConverter::convertStdUtf8StringToStdUtf16String(const std::string& string) {

}

CONSTEXPR_CXX20 std::u16string StringConverter::convertStdUtf16StringToStdUtf16String(const std::u8string& string) {

}

CONSTEXPR_CXX20 std::u16string StringConverter::convertStdUtf32StringToStdUtf16String(const std::u32string& string) {

}

CONSTEXPR_CXX20 std::u32string StringConverter::convertStdWStringToStdUtf32String(const std::wstring& string) {

}

CONSTEXPR_CXX20 std::u32string StringConverter::convertStdUtf8StringToStdUtf32String(const std::string& string) {

}

CONSTEXPR_CXX20 std::u32string StringConverter::convertStdUtf16StringToStdUtf32String(const std::u8string& string) {

}

CONSTEXPR_CXX20 std::u32string StringConverter::convertStdUtf32StringToStdUtf32String(const std::u16string& string) {

}

__BASE_STRING_NAMESPACE_END
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
	// Simd

	// =================================================================================================================
	//													std::wstring
	// =================================================================================================================

	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf8StringToStdWStringScalar(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf8StringToStdWStringSse2(const std::u8string& string);

	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf8StringToStdWStringAvx2(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf8StringToStdWStringAvx512F(const std::u8string& string);
	
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf16StringToStdWStringScalar(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf16StringToStdWStringSse2(const std::u16string& string);
	
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf16StringToStdWStringAvx2(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf16StringToStdWStringAv512F(const std::u16string& string);

	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf32StringToStdWStringScalar(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf32StringToStdWStringSse2(const std::u32string& string);
	
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf32StringToStdWStringAvx2(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::wstring convertStdUtf32StringToStdWStringAvx512F(const std::u32string& string);

	// =================================================================================================================
	//													std::string
	// =================================================================================================================

	CONSTEXPR_CXX20 NODISCARD std::string convertStdWStringToStdStringScalar(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdWStringToStdStringSse2(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::string convertStdWStringToStdStringAvx2(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdWStringToStdStringAvx512F(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf8StringToStdStringScalar(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf8StringToStdStringSse2(const std::u8string& string);

	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf8StringToStdStringAvx2(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf8StringToStdStringAvx512F(const std::u8string& string);

	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf16StringToStdScalar(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf16StringToStdStringSse2(const std::u16string& string);
	
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf16StringToStdStringAxv2(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf16StringToStdStringAvx512F(const std::u16string& string);

	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf32StringToStdStringScalar(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf32StringToStdStringSse2(const std::u32string& string);

	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf32StringToStdStringAvx2(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::string convertStdUtf32StringToStdStringAvx512F(const std::u32string& string);

	// =================================================================================================================
	//													std::u8string
	// =================================================================================================================

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdWStringToStdUtf8StringScalar(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdWStringToStdUtf8StringSse2(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdWStringToStdUtf8StringAvx2(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdWStringToStdUtf8StringAvx512F(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdStringToStdUtf8StringScalar(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdStringToStdUtf8StringSse2(const std::string& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdStringToStdUtf8StringAvx2(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdStringToStdUtf8StringAvx512F(const std::string& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf16StringToStdUtf8StringScalar(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf16StringToStdUtf8StringSse2(const std::u16string& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf16StringToStdUtf8StringAvx2(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf16StringToStdUtf8StringAvx512F(const std::u16string& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf32StringToStdUtf8StringScalar(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf32StringToStdUtf8StringSse2(const std::u32string& string);

	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf32StringToStdUtf8StringAvx2(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::u8string convertStdUtf32StringToStdUtf8StringAvx512F(const std::u32string& string);

	// =================================================================================================================
	//													std::u16string
	// =================================================================================================================

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdWStringToStdUtf16StringScalar(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdWStringToStdUtf16StringSse2(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdWStringToStdUtf16StringAvx2(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdWStringToStdUtf16StringAvx512F(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf8StringToStdUtf16StringScalar(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf8StringToStdUtf16StringSse2(const std::string& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf8StringToStdUtf16StringAvx2(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf8StringToStdUtf16StringAvx512F(const std::string& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf16StringToStdUtf16StringScalar(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf16StringToStdUtf16StringSse2(const std::u8string& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf16StringToStdUtf16StringAvx2(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf16StringToStdUtf16StringAvx512F(const std::u8string& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf32StringToStdUtf16StringScalar(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf32StringToStdUtf16StringSse2(const std::u32string& string);

	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf32StringToStdUtf16StringAvx2(const std::u32string& string);
	CONSTEXPR_CXX20 NODISCARD std::u16string convertStdUtf32StringToStdUtf16StringAvx512(const std::u32string& string);

	// =================================================================================================================
	//													std::u32string
	// =================================================================================================================

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdWStringToStdUtf32StringScalar(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdWStringToStdUtf32StringSse2(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdWStringToStdUtf32StringAvx2(const std::wstring& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdWStringToStdUtf32StringAvx512F(const std::wstring& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdStringToStdUtf32StringScalar(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdStringToStdUtf32StringSse2(const std::string& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdStringToStdUtf32StringAvx2(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdStringToStdUtf32StringAvx512F(const std::string& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf8StringToStdUtf32StringScalar(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf8StringToStdUtf32StringSse2(const std::u8string& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf8StringToStdUtf32StringAvx2(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf8StringToStdUtf32StringAvx512F(const std::u8string& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf16StringToStdUtf32StringScalar(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf16StringToStdUtf32StringSse2(const std::u16string& string);

	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf16StringToStdUtf32StringAvx2(const std::u16string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf16StringToStdUtf32StringAvx512F(const std::u16string& string);



	// =================================================================================================================
	//														General
	// =================================================================================================================

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
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdStringToStdUtf32String(const std::string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf8StringToStdUtf32String(const std::u8string& string);
	CONSTEXPR_CXX20 NODISCARD std::u32string convertStdUtf16StringToStdUtf32String(const std::u16string& string);
};

__BASE_STRING_NAMESPACE_END
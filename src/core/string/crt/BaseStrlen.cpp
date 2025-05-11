#include <src/core/string/crt/BaseStrlen.h>
#include <base/core/arch/ProcessorFeatures.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t CDECL __base_strlenScalar(const char* string) noexcept {
	const char* eos = string;
	while (*eos++);

	return(eos - string - 1);

}

DECLARE_NOALIAS std::size_t CDECL __base_strlenSse2(const char* string) noexcept {

}

DECLARE_NOALIAS std::size_t CDECL __base_strlenAvx(const char* string) noexcept {

}

DECLARE_NOALIAS std::size_t CDECL __base_strlenAvx512(const char* string) noexcept {

}

// ===================================================================================

DECLARE_NOALIAS std::size_t CDECL __base_wcslenScalar(const wchar_t* string) noexcept {

}

DECLARE_NOALIAS std::size_t CDECL __base_wcslenSse2(const wchar_t* string) noexcept {

}

DECLARE_NOALIAS std::size_t CDECL __base_wcslenAvx(const wchar_t* string) noexcept {

}

DECLARE_NOALIAS std::size_t CDECL __base_wcslenAvx512(const wchar_t* string) noexcept {

}

// ===================================================================================

DECLARE_NOALIAS std::size_t	CDECL __base_strlen(const char* string) noexcept {
	if (ProcessorFeatures::AVX512F())
		return __base_strlenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_strlenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_strlenSse2(string);

	return __base_strlenScalar(string);
}

DECLARE_NOALIAS std::size_t CDECL __base_wcslen(const wchar_t* string) noexcept  {
	if (ProcessorFeatures::AVX512F())
		return __base_wcslenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_wcslenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_wcslenSse2(string);

	return __base_wcslenScalar(string);
}

__BASE_STRING_NAMESPACE_END
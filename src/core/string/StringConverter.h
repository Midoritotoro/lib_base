#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/core/string/StringConfig.h>

__BASE_STRING_NAMESPACE_BEGIN

class StringConverter {
public:
private:
#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	NODISCARD int ConvertWCharToUnicode(
		char* buffer,
		size_t bufferlen,
		const wchar_t* input)
	{

	}

	NODISCARD int ConvertUnicodeToWChar(
		wchar_t* buffer,
		size_t bufferlen,
		const char* input)
	{

	}
#endif	
};

__BASE_STRING_NAMESPACE_END
#include <base/core/string/String.h>

#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/wcs/BaseWcslen.h>
#include <src/core/string/crt/c32/BaseC32len.h>

#include <src/core/string/crt/BaseAnyStrlen.h>
#include <src/core/string/crt/BaseAnyStrcmp.h>

void TestStringLengths() {
	// ==============================================================================================
	//										strlen
	// ==============================================================================================

	// Length 0
	static const char strlenLen0Str[] = "";
	size_t strlenLen0Result = base::string::__base_strlen(strlenLen0Str);
	Assert(strlenLen0Result == 0 && strlenLen0Result == strlen(strlenLen0Str));

	// Length 5
	static const char strlenLen5Str[] = "abcde";
	size_t strlenLen5Result = base::string::__base_strlen(strlenLen5Str);
	Assert(strlenLen5Result == 5 && strlenLen5Result == strlen(strlenLen5Str));

	// Length 64
	static const char strlenLen64Str[] = "abcdefghijklmnopqrstuvwfsefsfxyzabcdefghijklmnopqrstuvwxyzabcdef";
	size_t strlenLen64Result = base::string::__base_strlen(strlenLen64Str);
	Assert(strlenLen64Result == 64 && strlenLen64Result == strlen(strlenLen64Str));

	// Length 65
	static const char strlenLen65Str[] = "abcdeffghifffjklmnogspqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefg";
	size_t strlenLen65Result = base::string::__base_strlen(strlenLen65Str);
	Assert(strlenLen65Result == 65 && strlenLen65Result == strlen(strlenLen65Str));



	// ==============================================================================================
	//										wcslen
	// ==============================================================================================

	// Length 0
	static const wchar_t wcslenLen0Str[] = L"";
	size_t wcslenLen0Result = base::string::__base_wcslen(wcslenLen0Str);
	Assert(wcslenLen0Result == 0 && wcslenLen0Result == wcslen(wcslenLen0Str));

	// Length 5
	static const wchar_t wcslenLen5Str[] = L"abcde";
	size_t wcslenLen5Result = base::string::__base_wcslen(wcslenLen5Str);
	Assert(wcslenLen5Result == 5 && wcslenLen5Result == wcslen(wcslenLen5Str));

	// Length 64
	static const wchar_t wcslenLen64Str[] = L"abcdrriefghijklmnopqrstuvwxyzabcdefghwetijklmnopqrstuvwxyzabcdef";
	size_t wcslenLen64Result = base::string::__base_wcslen(wcslenLen64Str);
	Assert(wcslenLen64Result == 64 && wcslenLen64Result == wcslen(wcslenLen64Str));

	// Length 65
	static const wchar_t wcslenLen65Str[] = L"abcdefghwtwegsijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefg";
	size_t wcslenLen65Result = base::string::__base_wcslen(wcslenLen65Str);
	Assert(wcslenLen65Result == 65 && wcslenLen65Result == wcslen(wcslenLen65Str));



	// ==============================================================================================
	//										c32len
	// ==============================================================================================

	// Length 0
	static const char32_t c32lenLen0Str[] = U"";
	size_t c32lenLen0Result = base::string::__base_c32len(c32lenLen0Str);
	Assert(wcslenLen0Result == 0);

	// Length 5
	static const char32_t c32lenLen5Str[] = U"abcde";
	size_t c32lenLen5Result = base::string::__base_c32len(c32lenLen5Str);
	Assert(c32lenLen5Result == 5);

	// Length 64
	static const char32_t c32lenLen64Str[] = U"abcdef52353ghifjklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdef";
	size_t c32lenLen64Result = base::string::__base_c32len(c32lenLen64Str);
	Assert(c32lenLen64Result == 64);

	// Length 65
	static const char32_t c32lenLen65Str[] = U"abcdefggu34yyhijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefg";
	size_t c32lenLen65Result = base::string::__base_c32len(c32lenLen65Str);
	Assert(c32lenLen65Result == 65);
}

int main(int argc, char** argv) {
	TestStringLengths();
	return 0;
}
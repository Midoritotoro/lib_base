#include <base/core/string/String.h>

#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/cs/BaseStrcmp.h>

#include <src/core/string/crt/cs/BaseStrstr.h>
#include <src/core/string/crt/cs/BaseStrstrn.h>


#include <src/core/string/crt/wcs/BaseWcscmp.h>
#include <src/core/string/crt/wcs/BaseWcslen.h>

#include <src/core/string/crt/wcs/BaseWcsstr.h>
#include <src/core/string/crt/wcs/BaseWcsstrn.h>


#include <src/core/string/crt/c32/BaseC32cmp.h>
#include <src/core/string/crt/c32/BaseC32len.h>

#include <src/core/string/crt/c32/BaseC32str.h>
#include <src/core/string/crt/c32/BaseC32strn.h>


int main(int argc, char** argv) {
	// Strcmp 

    static const char abcde[] = "abcde";
    static const char abcdx[] = "abcdx";

	Assert(base::string::__base_strcmp(&abcde[0], "abcde") == 0);
	Assert(base::string::__base_strcmp(abcde, abcdx) == strcmp(abcde, abcdx));

	static const wchar_t abcde_W[] = L"abcde";
	static const wchar_t abcdx_W[] = L"abcdx";

	Assert(base::string::__base_wcscmp(&abcde_W[0], L"abcde") == 0);
	Assert(base::string::__base_wcscmp(abcde_W, abcdx_W) == wcscmp(abcde_W, abcdx_W));

	static const char32_t abcde_32[] = U"abcde";
	static const char32_t abcdx_32[] = U"abcdx";

	Assert(base::string::__base_c32cmp(&abcde_32[0], U"abcde") == 0);

	// Strstr

	static const char strFind[]			= "Some test string";
	static const char strFindShort[]	= "Some string";

	Assert(base::string::__base_strstr(strFind, "Some") == strstr(strFind, "Some"));

	static const wchar_t strFind_W[]		= L"Some test string";
	static const wchar_t strFindShort_W[]	= L"Some string";

	//Assert(base::string::__base_wcsstr(strFind_W, L"Some") == wcsstr(strFind_W, L"Some"));

	//static const char32_t strFind_32[]		= U"Some test string";
	//static const char32_t strFindShort_32[] = U"Some string";

	// Assert(base::string::__base_c32str(strFind_32, U"Some") == &strFind_32[0]);

	return 0;
}
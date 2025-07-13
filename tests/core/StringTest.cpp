#include <base/core/string/String.h>

#include <src/core/string/crt/BaseStrlen.h>
#include <src/core/string/crt/BaseStrcmp.h>

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

	return 0;
}
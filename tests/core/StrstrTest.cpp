#include <src/core/string/crt/cs/BaseStrstr.h>
#include <src/core/string/crt/wcs/BaseWcsstr.h>
#include <src/core/string/crt/c32/BaseC32str.h>

#include <src/core/string/crt/c32/BaseC32len.h>
#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/wcs/BaseWcslen.h>



const char32_t* findChar32String(const char32_t* haystack, const char32_t* needle) {
	if (!haystack || !needle) return nullptr;

	size_t needleLen = base::string::__base_c32len(needle);

	if (needleLen == 0) return haystack;

	size_t haystackLen = base::string::__base_c32len(haystack);

	for (size_t i = 0; i <= haystackLen - needleLen; ++i) {
		bool match = true;
		for (size_t j = 0; j < needleLen; ++j) {
			if (haystack[i + j] != needle[j]) {
				match = false;
				break;
			}
		}
		if (match) {
			return haystack + i;
		}
	}
	return nullptr;  // Not found
}

void TestStrstr() {
	// ==============================================================================================
	//                        strstr Tests
	// ==============================================================================================
	// 
	// Basic test
	static const char strstrHaystack[] = "This is a test string";
	static const char strstrNeedle[] = "test";

	const char* strstrResult = base::string::__base_strstr(strstrHaystack, strstrNeedle);
	Assert(strstrResult != nullptr && strstrResult == strstr(strstrHaystack, strstrNeedle));

	// Needle at the beginning
	static const char strstrHaystackBeginning[] = "test string";
	static const char strstrNeedleBeginning[] = "test";

	const char* strstrResultBeginning = base::string::__base_strstr(strstrHaystackBeginning, strstrNeedleBeginning);
	Assert(strstrResultBeginning != nullptr && strstrResultBeginning == strstr(strstrHaystackBeginning, strstrNeedleBeginning));

	// Needle at the end
	static const char strstrHaystackEnd[] = "string test";
	static const char strstrNeedleEnd[] = "test";

	const char* strstrResultEnd = base::string::__base_strstr(strstrHaystackEnd, strstrNeedleEnd);
	Assert(strstrResultEnd != nullptr && strstrResultEnd == strstr(strstrHaystackEnd, strstrNeedleEnd));

	// Needle not found
	static const char strstrHaystackNotFound[] = "This is a string";
	static const char strstrNeedleNotFound[] = "test";

	const char* strstrResultNotFound = base::string::__base_strstr(strstrHaystackNotFound, strstrNeedleNotFound);
	Assert(strstrResultNotFound == nullptr && strstrResultNotFound == strstr(strstrHaystackNotFound, strstrNeedleNotFound));

	// Empty needle
	// If the needle is empty, then strstr must return haystack
	static const char strstrHaystackEmptyNeedle[] = "Some string";
	static const char strstrNeedleEmpty[] = "";

	const char* strstrResultEmptyNeedle = base::string::__base_strstr(strstrHaystackEmptyNeedle, strstrNeedleEmpty);
	Assert(strstrResultEmptyNeedle != nullptr && strstrResultEmptyNeedle == strstrHaystackEmptyNeedle && strstrResultEmptyNeedle == strstr(strstrHaystackEmptyNeedle, strstrNeedleEmpty));

	// ==============================================================================================
	//                        wcsstr Tests
	// ==============================================================================================

	//// Basic test
	//static const wchar_t wcsstrHaystack[] = L"This is a test string";
	//static const wchar_t wcsstrNeedle[] = L"test";
	//const wchar_t* wcsstrResult = base::string::__base_wcsstr(wcsstrHaystack, wcsstrNeedle);
	//Assert(wcsstrResult != nullptr && wcsstrResult == wcsstr(wcsstrHaystack, wcsstrNeedle));

	//// Needle at the beginning
	//static const wchar_t wcsstrHaystackBeginning[] = L"test string";
	//static const wchar_t wcsstrNeedleBeginning[] = L"test";
	//const wchar_t* wcsstrResultBeginning = base::string::__base_wcsstr(wcsstrHaystackBeginning, wcsstrNeedleBeginning);
	//Assert(wcsstrResultBeginning != nullptr && wcsstrResultBeginning == wcsstr(wcsstrHaystackBeginning, wcsstrNeedleBeginning));

	//// Needle at the end
	//static const wchar_t wcsstrHaystackEnd[] = L"string test";
	//static const wchar_t wcsstrNeedleEnd[] = L"test";
	//const wchar_t* wcsstrResultEnd = base::string::__base_wcsstr(wcsstrHaystackEnd, wcsstrNeedleEnd);
	//Assert(wcsstrResultEnd != nullptr && wcsstrResultEnd == wcsstr(wcsstrHaystackEnd, wcsstrNeedleEnd));

	//// Needle not found
	//static const wchar_t wcsstrHaystackNotFound[] = L"This is a string";
	//static const wchar_t wcsstrNeedleNotFound[] = L"test";
	//const wchar_t* wcsstrResultNotFound = base::string::__base_wcsstr(wcsstrHaystackNotFound, wcsstrNeedleNotFound);
	//Assert(wcsstrResultNotFound == nullptr && wcsstrResultNotFound == wcsstr(wcsstrHaystackNotFound, wcsstrNeedleNotFound));

	//// Empty needle
	//static const wchar_t wcsstrHaystackEmptyNeedle[] = L"Some string";
	//static const wchar_t wcsstrNeedleEmpty[] = L"";
	//const wchar_t* wcsstrResultEmptyNeedle = base::string::__base_wcsstr(wcsstrHaystackEmptyNeedle, wcsstrNeedleEmpty);
	//Assert(wcsstrResultEmptyNeedle != nullptr && wcsstrResultEmptyNeedle == wcsstrHaystackEmptyNeedle);

	//// ==============================================================================================
	////                        c32strstr Tests (Using Helper Function)
	//// ==============================================================================================

	//// Basic test
	//static const char32_t c32strHaystack[] = U"This is a test string";
	//static const char32_t c32strNeedle[] = U"test";
	//const char32_t* c32strResult = base::string::__base_c32str(c32strHaystack, c32strNeedle);
	//Assert(c32strResult != nullptr && c32strResult == findChar32String(c32strHaystack, c32strNeedle));

	//// Needle at the beginning
	//static const char32_t c32strHaystackBeginning[] = U"test string";
	//static const char32_t c32strNeedleBeginning[] = U"test";
	//const char32_t* c32strResultBeginning = base::string::__base_c32str(c32strHaystackBeginning, c32strNeedleBeginning);
	//Assert(c32strResultBeginning != nullptr && c32strResultBeginning == findChar32String(c32strHaystackBeginning, c32strNeedleBeginning));

	//// Needle at the end
	//static const char32_t c32strHaystackEnd[] = U"string test";
	//static const char32_t c32strNeedleEnd[] = U"test";
	//const char32_t* c32strResultEnd = base::string::__base_c32str(c32strHaystackEnd, c32strNeedleEnd);
	//Assert(c32strResultEnd != nullptr && c32strResultEnd == findChar32String(c32strHaystackEnd, c32strNeedleEnd));

	//// Needle not found
	//static const char32_t c32strHaystackNotFound[] = U"This is a string";
	//static const char32_t c32strNeedleNotFound[] = U"test";
	//const char32_t* c32strResultNotFound = base::string::__base_c32str(c32strHaystackNotFound, c32strNeedleNotFound);
	//Assert(c32strResultNotFound == nullptr && c32strResultNotFound == findChar32String(c32strHaystackNotFound, c32strNeedleNotFound));

	//// Empty needle
	//static const char32_t c32strHaystackEmptyNeedle[] = U"Some string";
	//static const char32_t c32strNeedleEmpty[] = U"";
	//const char32_t* c32strResultEmptyNeedle = base::string::__base_c32str(c32strHaystackEmptyNeedle, c32strNeedleEmpty);
	//Assert(c32strResultEmptyNeedle != nullptr && c32strResultEmptyNeedle == c32strHaystackEmptyNeedle);
}


int main(int argc, char** argv) {
	TestStrstr();
	return 0;
}
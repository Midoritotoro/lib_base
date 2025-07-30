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

#include <src/core/string/crt/BaseAnyStrlen.h>
#include <src/core/string/crt/BaseAnyStrcmp.h>

void TestStringComparisons() {
	// ==============================================================================================
	//                        strcmp - Length 15
	// ==============================================================================================



	static const char len15Abcde[] = "abcdefghijklmno";
	static const char len15Abcdx[] = "abcdefghijklmnx";
	static const char len15AbcdeEq[] = "abcdefghijklmno";

	int strcmpLen15Less = base::string::__base_strcmp(len15Abcde, len15Abcdx);
	int strcmpLen15Greater = base::string::__base_strcmp(len15Abcdx, len15Abcde);
	int strcmpLen15Equal = base::string::__base_strcmp(len15Abcde, len15AbcdeEq);

	Assert(strcmpLen15Less == -1 && strcmpLen15Less == strcmp(len15Abcde, len15Abcdx));
	Assert(strcmpLen15Greater == 1 && strcmpLen15Greater == strcmp(len15Abcdx, len15Abcde));
	Assert(strcmpLen15Equal == 0 && strcmpLen15Equal == strcmp(len15Abcde, len15AbcdeEq));



	// ==============================================================================================
	//                        strcmp - Length 16
	// ==============================================================================================



	static const char len16Abcde[] = "abcdefghijklmnop";
	static const char len16Abcdx[] = "abcdefghijklmnoq";
	static const char len16AbcdeEq[] = "abcdefghijklmnop";

	int strcmpLen16Less = base::string::__base_strcmp(len16Abcde, len16Abcdx);
	int strcmpLen16Greater = base::string::__base_strcmp(len16Abcdx, len16Abcde);
	int strcmpLen16Equal = base::string::__base_strcmp(len16Abcde, len16AbcdeEq);

	Assert(strcmpLen16Less == -1 && strcmpLen16Less == strcmp(len16Abcde, len16Abcdx));
	Assert(strcmpLen16Greater == 1 && strcmpLen16Greater == strcmp(len16Abcdx, len16Abcde));
	Assert(strcmpLen16Equal == 0 && strcmpLen16Equal == strcmp(len16Abcde, len16AbcdeEq));




	// ==============================================================================================
	//                        strcmp - Length 17
	// ==============================================================================================



	static const char len17Abcde[] = "abcdefghijklmnopq";
	static const char len17Abcdx[] = "abcdefghijklmnopqr";
	static const char len17AbcdeEq[] = "abcdefghijklmnopq";

	int strcmpLen17Less = base::string::__base_strcmp(len17Abcde, len17Abcdx);
	int strcmpLen17Greater = base::string::__base_strcmp(len17Abcdx, len17Abcde);
	int strcmpLen17Equal = base::string::__base_strcmp(len17Abcde, len17AbcdeEq);

	Assert(strcmpLen17Less == -1 && strcmpLen17Less == strcmp(len17Abcde, len17Abcdx));
	Assert(strcmpLen17Greater == 1 && strcmpLen17Greater == strcmp(len17Abcdx, len17Abcde));
	Assert(strcmpLen17Equal == 0 && strcmpLen17Equal == strcmp(len17Abcde, len17AbcdeEq));



	// ==============================================================================================
	//                        strcmp - Length 31
	// ==============================================================================================



	static const char len31Abcde[] = "abcdefghijklmnopqrstuvwxyzabcde";
	static const char len31Abcdx[] = "abcdefghijklmnopqrstuvwxyzabcdx";
	static const char len31AbcdeEq[] = "abcdefghijklmnopqrstuvwxyzabcde";

	int strcmpLen31Less = base::string::__base_strcmp(len31Abcde, len31Abcdx);
	int strcmpLen31Greater = base::string::__base_strcmp(len31Abcdx, len31Abcde);
	int strcmpLen31Equal = base::string::__base_strcmp(len31Abcde, len31AbcdeEq);

	Assert(strcmpLen31Less == -1 && strcmpLen31Less == strcmp(len31Abcde, len31Abcdx));
	Assert(strcmpLen31Greater == 1 && strcmpLen31Greater == strcmp(len31Abcdx, len31Abcde));
	Assert(strcmpLen31Equal == 0 && strcmpLen31Equal == strcmp(len31Abcde, len31AbcdeEq));



	// ==============================================================================================
	//                        strcmp - Length 32
	// ==============================================================================================



	static const char len32Abcde[] = "abcdefghijklmnopqrstuvwxyzabcdef";
	static const char len32Abcdx[] = "abcdefghijklmnopqrstuvwxyzabcdex";
	static const char len32AbcdeEq[] = "abcdefghijklmnopqrstuvwxyzabcdef";

	int strcmpLen32Less = base::string::__base_strcmp(len32Abcde, len32Abcdx);
	int strcmpLen32Greater = base::string::__base_strcmp(len32Abcdx, len32Abcde);
	int strcmpLen32Equal = base::string::__base_strcmp(len32Abcde, len32AbcdeEq);

	Assert(strcmpLen32Less == -1 && strcmpLen32Less == strcmp(len32Abcde, len32Abcdx));
	Assert(strcmpLen32Greater == 1 && strcmpLen32Greater == strcmp(len32Abcdx, len32Abcde));
	Assert(strcmpLen32Equal == 0 && strcmpLen32Equal == strcmp(len32Abcde, len32AbcdeEq));



	// ==============================================================================================
	//                        strcmp - Length 33
	// ==============================================================================================



	static const char len33Abcde[] = "abcdefghijklmnopqrstuvwxyzabcdefg";
	static const char len33Abcdx[] = "abcdefghijklmnopqrstuvwxyzabcdefh";
	static const char len33AbcdeEq[] = "abcdefghijklmnopqrstuvwxyzabcdefg";

	int strcmpLen33Less = base::string::__base_strcmp(len33Abcde, len33Abcdx);
	int strcmpLen33Greater = base::string::__base_strcmp(len33Abcdx, len33Abcde);
	int strcmpLen33Equal = base::string::__base_strcmp(len33Abcde, len33AbcdeEq);

	Assert(strcmpLen33Less == -1 && strcmpLen33Less == strcmp(len33Abcde, len33Abcdx));
	Assert(strcmpLen33Greater == 1 && strcmpLen33Greater == strcmp(len33Abcdx, len33Abcde));
	Assert(strcmpLen33Equal == 0 && strcmpLen33Equal == strcmp(len33Abcde, len33AbcdeEq));



	// ==============================================================================================
	//                        strcmp - Length 63
	// ==============================================================================================



	static const char len63Abcde[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcde";
	static const char len63Abcdx[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdx";
	static const char len63AbcdeEq[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcde";

	int strcmpLen63Less = base::string::__base_strcmp(len63Abcde, len63Abcdx);
	int strcmpLen63Greater = base::string::__base_strcmp(len63Abcdx, len63Abcde);
	int strcmpLen63Equal = base::string::__base_strcmp(len63Abcde, len63AbcdeEq);

	Assert(strcmpLen63Less == -1 && strcmpLen63Less == strcmp(len63Abcde, len63Abcdx));
	Assert(strcmpLen63Greater == 1 && strcmpLen63Greater == strcmp(len63Abcdx, len63Abcde));
	Assert(strcmpLen63Equal == 0 && strcmpLen63Equal == strcmp(len63Abcde, len63AbcdeEq));



	// ==============================================================================================
	//                        strcmp - Length 64
	// ==============================================================================================



	static const char len64Abcde[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdef";
	static const char len64Abcdx[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdex";
	static const char len64AbcdeEq[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdef";

	int strcmpLen64Less = base::string::__base_strcmp(len64Abcde, len64Abcdx);
	int strcmpLen64Greater = base::string::__base_strcmp(len64Abcdx, len64Abcde);
	int strcmpLen64Equal = base::string::__base_strcmp(len64Abcde, len64AbcdeEq);

	Assert(strcmpLen64Less == -1 && strcmpLen64Less == strcmp(len64Abcde, len64Abcdx));
	Assert(strcmpLen64Greater == 1 && strcmpLen64Greater == strcmp(len64Abcdx, len64Abcde));
	Assert(strcmpLen64Equal == 0 && strcmpLen64Equal == strcmp(len64Abcde, len64AbcdeEq));



	// ==============================================================================================
	//                        strcmp - Length 65
	// ==============================================================================================



	static const char len65Abcde[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefg";
	static const char len65Abcdx[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefh";
	static const char len65AbcdeEq[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefg";

	int strcmpLen65Less = base::string::__base_strcmp(len65Abcde, len65Abcdx);
	int strcmpLen65Greater = base::string::__base_strcmp(len65Abcdx, len65Abcde);
	int strcmpLen65Equal = base::string::__base_strcmp(len65Abcde, len65AbcdeEq);

	Assert(strcmpLen65Less == -1 && strcmpLen65Less == strcmp(len65Abcde, len65Abcdx));
	Assert(strcmpLen65Greater == 1 && strcmpLen65Greater == strcmp(len65Abcdx, len65Abcde));
	Assert(strcmpLen65Equal == 0 && strcmpLen65Equal == strcmp(len65Abcde, len65AbcdeEq));



	// ==============================================================================================
	//                        wcscmp - Length 15
	// ==============================================================================================



	static const wchar_t len15AbcdeW[] = L"abcdefghijklmno";
	static const wchar_t len15AbcdxW[] = L"abcdefghijklmnx";
	static const wchar_t len15AbcdeEqW[] = L"abcdefghijklmno";

	int wcscmpLen15Less = base::string::__base_wcscmp(len15AbcdeW, len15AbcdxW);
	int wcscmpLen15Greater = base::string::__base_wcscmp(len15AbcdxW, len15AbcdeW);
	int wcscmpLen15Equal = base::string::__base_wcscmp(len15AbcdeW, len15AbcdeEqW);

	Assert(wcscmpLen15Less == -1 && wcscmpLen15Less == wcscmp(len15AbcdeW, len15AbcdxW));
	Assert(wcscmpLen15Greater == 1 && wcscmpLen15Greater == wcscmp(len15AbcdxW, len15AbcdeW));
	Assert(wcscmpLen15Equal == 0 && wcscmpLen15Equal == wcscmp(len15AbcdeW, len15AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 16
	// ==============================================================================================



	static const wchar_t len16AbcdeW[] = L"abcdefghijklmnop";
	static const wchar_t len16AbcdxW[] = L"abcdefghijklmnoq";
	static const wchar_t len16AbcdeEqW[] = L"abcdefghijklmnop";

	int wcscmpLen16Less = base::string::__base_wcscmp(len16AbcdeW, len16AbcdxW);
	int wcscmpLen16Greater = base::string::__base_wcscmp(len16AbcdxW, len16AbcdeW);
	int wcscmpLen16Equal = base::string::__base_wcscmp(len16AbcdeW, len16AbcdeEqW);

	Assert(wcscmpLen16Less == -1 && wcscmpLen16Less == wcscmp(len16AbcdeW, len16AbcdxW));
	Assert(wcscmpLen16Greater == 1 && wcscmpLen16Greater == wcscmp(len16AbcdxW, len16AbcdeW));
	Assert(wcscmpLen16Equal == 0 && wcscmpLen16Equal == wcscmp(len16AbcdeW, len16AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 17
	// ==============================================================================================



	static const wchar_t len17AbcdeW[] = L"abcdefghijklmnopq";
	static const wchar_t len17AbcdxW[] = L"abcdefghijklmnopqr";
	static const wchar_t len17AbcdeEqW[] = L"abcdefghijklmnopq";

	int wcscmpLen17Less = base::string::__base_wcscmp(len17AbcdeW, len17AbcdxW);
	int wcscmpLen17Greater = base::string::__base_wcscmp(len17AbcdxW, len17AbcdeW);
	int wcscmpLen17Equal = base::string::__base_wcscmp(len17AbcdeW, len17AbcdeEqW);

	Assert(wcscmpLen17Less == -1 && wcscmpLen17Less == wcscmp(len17AbcdeW, len17AbcdxW));
	Assert(wcscmpLen17Greater == 1 && wcscmpLen17Greater == wcscmp(len17AbcdxW, len17AbcdeW));
	Assert(wcscmpLen17Equal == 0 && wcscmpLen17Equal == wcscmp(len17AbcdeW, len17AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 31
	// ==============================================================================================



	static const wchar_t len31AbcdeW[] = L"abcdefghijklmnopqrstuvwxyzabcde";
	static const wchar_t len31AbcdxW[] = L"abcdefghijklmnopqrstuvwxyzabcdx";
	static const wchar_t len31AbcdeEqW[] = L"abcdefghijklmnopqrstuvwxyzabcde";

	int wcscmpLen31Less = base::string::__base_wcscmp(len31AbcdeW, len31AbcdxW);
	int wcscmpLen31Greater = base::string::__base_wcscmp(len31AbcdxW, len31AbcdeW);
	int wcscmpLen31Equal = base::string::__base_wcscmp(len31AbcdeW, len31AbcdeEqW);

	Assert(wcscmpLen31Less == -1 && wcscmpLen31Less == wcscmp(len31AbcdeW, len31AbcdxW));
	Assert(wcscmpLen31Greater == 1 && wcscmpLen31Greater == wcscmp(len31AbcdxW, len31AbcdeW));
	Assert(wcscmpLen31Equal == 0 && wcscmpLen31Equal == wcscmp(len31AbcdeW, len31AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 32
	// ==============================================================================================



	static const wchar_t len32AbcdeW[] = L"abcdefghijklmnopqrstuvwxyzabcdef";
	static const wchar_t len32AbcdxW[] = L"abcdefghijklmnopqrstuvwxyzabcdex";
	static const wchar_t len32AbcdeEqW[] = L"abcdefghijklmnopqrstuvwxyzabcdef";

	int wcscmpLen32Less = base::string::__base_wcscmp(len32AbcdeW, len32AbcdxW);
	int wcscmpLen32Greater = base::string::__base_wcscmp(len32AbcdxW, len32AbcdeW);
	int wcscmpLen32Equal = base::string::__base_wcscmp(len32AbcdeW, len32AbcdeEqW);

	Assert(wcscmpLen32Less == -1 && wcscmpLen32Less == wcscmp(len32AbcdeW, len32AbcdxW));
	Assert(wcscmpLen32Greater == 1 && wcscmpLen32Greater == wcscmp(len32AbcdxW, len32AbcdeW));
	Assert(wcscmpLen32Equal == 0 && wcscmpLen32Equal == wcscmp(len32AbcdeW, len32AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 33
	// ==============================================================================================



	static const wchar_t len33AbcdeW[] = L"abcdefghijklmnopqrstuvwxyzabcdefg";
	static const wchar_t len33AbcdxW[] = L"abcdefghijklmnopqrstuvwxyzabcdefh";
	static const wchar_t len33AbcdeEqW[] = L"abcdefghijklmnopqrstuvwxyzabcdefg";

	int wcscmpLen33Less = base::string::__base_wcscmp(len33AbcdeW, len33AbcdxW);
	int wcscmpLen33Greater = base::string::__base_wcscmp(len33AbcdxW, len33AbcdeW);
	int wcscmpLen33Equal = base::string::__base_wcscmp(len33AbcdeW, len33AbcdeEqW);

	Assert(wcscmpLen33Less == -1 && wcscmpLen33Less == wcscmp(len33AbcdeW, len33AbcdxW));
	Assert(wcscmpLen33Greater == 1 && wcscmpLen33Greater == wcscmp(len33AbcdxW, len33AbcdeW));
	Assert(wcscmpLen33Equal == 0 && wcscmpLen33Equal == wcscmp(len33AbcdeW, len33AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 63
	// ==============================================================================================



	static const wchar_t len63AbcdeW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcde";
	static const wchar_t len63AbcdxW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdx";
	static const wchar_t len63AbcdeEqW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcde";

	int wcscmpLen63Less = base::string::__base_wcscmp(len63AbcdeW, len63AbcdxW);
	int wcscmpLen63Greater = base::string::__base_wcscmp(len63AbcdxW, len63AbcdeW);
	int wcscmpLen63Equal = base::string::__base_wcscmp(len63AbcdeW, len63AbcdeEqW);

	Assert(wcscmpLen63Less == -1 && wcscmpLen63Less == wcscmp(len63AbcdeW, len63AbcdxW));
	Assert(wcscmpLen63Greater == 1 && wcscmpLen63Greater == wcscmp(len63AbcdxW, len63AbcdeW));
	Assert(wcscmpLen63Equal == 0 && wcscmpLen63Equal == wcscmp(len63AbcdeW, len63AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 64
	// ==============================================================================================



	static const wchar_t len64AbcdeW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdef";
	static const wchar_t len64AbcdxW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdex";
	static const wchar_t len64AbcdeEqW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdef";

	int wcscmpLen64Less = base::string::__base_wcscmp(len64AbcdeW, len64AbcdxW);
	int wcscmpLen64Greater = base::string::__base_wcscmp(len64AbcdxW, len64AbcdeW);
	int wcscmpLen64Equal = base::string::__base_wcscmp(len64AbcdeW, len64AbcdeEqW);

	Assert(wcscmpLen64Less == -1 && wcscmpLen64Less == wcscmp(len64AbcdeW, len64AbcdxW));
	Assert(wcscmpLen64Greater == 1 && wcscmpLen64Greater == wcscmp(len64AbcdxW, len64AbcdeW));
	Assert(wcscmpLen64Equal == 0 && wcscmpLen64Equal == wcscmp(len64AbcdeW, len64AbcdeEqW));



	// ==============================================================================================
	//                        wcscmp - Length 65
	// ==============================================================================================



	static const wchar_t len65AbcdeW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefg";
	static const wchar_t len65AbcdxW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefh";
	static const wchar_t len65AbcdeEqW[] = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefg";

	int wcscmpLen65Less = base::string::__base_wcscmp(len65AbcdeW, len65AbcdxW);
	int wcscmpLen65Greater = base::string::__base_wcscmp(len65AbcdxW, len65AbcdeW);
	int wcscmpLen65Equal = base::string::__base_wcscmp(len65AbcdeW, len65AbcdeEqW);

	Assert(wcscmpLen65Less == -1 && wcscmpLen65Less == wcscmp(len65AbcdeW, len65AbcdxW));
	Assert(wcscmpLen65Greater == 1 && wcscmpLen65Greater == wcscmp(len65AbcdxW, len65AbcdeW));
	Assert(wcscmpLen65Equal == 0 && wcscmpLen65Equal == wcscmp(len65AbcdeW, len65AbcdeEqW));



	// ==============================================================================================
	//                        c32cmp - Length 15 (char32_t)
	// ==============================================================================================



	static const char32_t len15AbcdeC32[] = U"abcdefghijklmno";
	static const char32_t len15AbcdxC32[] = U"abcdefghijklmnx";
	static const char32_t len15AbcdeEqC32[] = U"abcdefghijklmno";

	int c32cmpLen15Less = base::string::__base_c32cmp(len15AbcdeC32, len15AbcdxC32);
	int c32cmpLen15Greater = base::string::__base_c32cmp(len15AbcdxC32, len15AbcdeC32);
	int c32cmpLen15Equal = base::string::__base_c32cmp(len15AbcdeC32, len15AbcdeEqC32);

	// Assuming there's no standard library function like strcmp/wcscmp for char32_t
	Assert(c32cmpLen15Less == -1);
	Assert(c32cmpLen15Greater == 1);
	Assert(c32cmpLen15Equal == 0);



	// ==============================================================================================
	//                        c32cmp - Length 16 (char32_t)
	// ==============================================================================================



	static const char32_t len16AbcdeC32[] = U"abcdefghijklmnop";
	static const char32_t len16AbcdxC32[] = U"abcdefghijklmnoq";
	static const char32_t len16AbcdeEqC32[] = U"abcdefghijklmnop";

	int c32cmpLen16Less = base::string::__base_c32cmp(len16AbcdeC32, len16AbcdxC32);
	int c32cmpLen16Greater = base::string::__base_c32cmp(len16AbcdxC32, len16AbcdeC32);
	int c32cmpLen16Equal = base::string::__base_c32cmp(len16AbcdeC32, len16AbcdeEqC32);

	Assert(c32cmpLen16Less == -1);
	Assert(c32cmpLen16Greater == 1);
	Assert(c32cmpLen16Equal == 0);



	// ==============================================================================================
	//                        c32cmp - Length 17 (char32_t)
	// ==============================================================================================



	static const char32_t len17AbcdeC32[] = U"abcdefghijklmnopq";
	static const char32_t len17AbcdxC32[] = U"abcdefghijklmnopqr";
	static const char32_t len17AbcdeEqC32[] = U"abcdefghijklmnopq";

	int c32cmpLen17Less = base::string::__base_c32cmp(len17AbcdeC32, len17AbcdxC32);
	int c32cmpLen17Greater = base::string::__base_c32cmp(len17AbcdxC32, len17AbcdeC32);
	int c32cmpLen17Equal = base::string::__base_c32cmp(len17AbcdeC32, len17AbcdeEqC32);

	Assert(c32cmpLen17Less == -1);
	Assert(c32cmpLen17Greater == 1);
	Assert(c32cmpLen17Equal == 0);



	// ==============================================================================================
	//                        c32cmp - Length 31 (char32_t)
	// ==============================================================================================



	static const char32_t len31AbcdeC32[] = U"abcdefghijklmnopqrstuvwxyzabcde";
	static const char32_t len31AbcdxC32[] = U"abcdefghijklmnopqrstuvwxyzabcdx";
	static const char32_t len31AbcdeEqC32[] = U"abcdefghijklmnopqrstuvwxyzabcde";

	int c32cmpLen31Less = base::string::__base_c32cmp(len31AbcdeC32, len31AbcdxC32);
	int c32cmpLen31Greater = base::string::__base_c32cmp(len31AbcdxC32, len31AbcdeC32);
	int c32cmpLen31Equal = base::string::__base_c32cmp(len31AbcdeC32, len31AbcdeEqC32);

	Assert(c32cmpLen31Less == -1);
	Assert(c32cmpLen31Greater == 1);
	Assert(c32cmpLen31Equal == 0);



	// ==============================================================================================
	//                        c32cmp - Length 32 (char32_t)
	// ==============================================================================================



	static const char32_t len32AbcdeC32[] = U"abcdefghijklmnopqrstuvwxyzabcdef";
	static const char32_t len32AbcdxC32[] = U"abcdefghijklmnopqrstuvwxyzabcdex";
	static const char32_t len32AbcdeEqC32[] = U"abcdefghijklmnopqrstuvwxyzabcdef";

	int c32cmpLen32Less = base::string::__base_c32cmp(len32AbcdeC32, len32AbcdxC32);
	int c32cmpLen32Greater = base::string::__base_c32cmp(len32AbcdxC32, len32AbcdeC32);
	int c32cmpLen32Equal = base::string::__base_c32cmp(len32AbcdeC32, len32AbcdeEqC32);

	Assert(c32cmpLen32Less == -1);
	Assert(c32cmpLen32Greater == 1);
	Assert(c32cmpLen32Equal == 0);



	// ==============================================================================================
	//                        c32cmp - Length 33 (char32_t)
	// ==============================================================================================



	static const char32_t len33AbcdeC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefg";
	static const char32_t len33AbcdxC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefh";
	static const char32_t len33AbcdeEqC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefg";

	int c32cmpLen33Less = base::string::__base_c32cmp(len33AbcdeC32, len33AbcdxC32);
	int c32cmpLen33Greater = base::string::__base_c32cmp(len33AbcdxC32, len33AbcdeC32);
	int c32cmpLen33Equal = base::string::__base_c32cmp(len33AbcdeC32, len33AbcdeEqC32);

	Assert(c32cmpLen33Less == -1);
	Assert(c32cmpLen33Greater == 1);
	Assert(c32cmpLen33Equal == 0);



	// ==============================================================================================
	//                        c32cmp - Length 63 (char32_t)
	// ==============================================================================================



	static const char32_t len63AbcdeC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcde";
	static const char32_t len63AbcdxC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdx";
	static const char32_t len63AbcdeEqC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcde";

	int c32cmpLen63Less = base::string::__base_c32cmp(len63AbcdeC32, len63AbcdxC32);
	int c32cmpLen63Greater = base::string::__base_c32cmp(len63AbcdxC32, len63AbcdeC32);
	int c32cmpLen63Equal = base::string::__base_c32cmp(len63AbcdeC32, len63AbcdeEqC32);

	Assert(c32cmpLen63Less == -1);
	Assert(c32cmpLen63Greater == 1);
	Assert(c32cmpLen63Equal == 0);



	// ==============================================================================================
	//                        c32cmp - Length 64 (char32_t)
	// ==============================================================================================



	static const char32_t len64AbcdeC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdef";
	static const char32_t len64AbcdxC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdex";
	static const char32_t len64AbcdeEqC32[] = U"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdef";

}

int main(int argc, char** argv) {



	// ==============================================================================================
	//												c32cmp
	// ==============================================================================================

	static const char32_t abcde_32[] = U"abcde";
	static const char32_t abcdx_32[] = U"abcdx";

	Assert(base::string::__base_c32cmp(&abcde_32[0], U"abcde") == 0);



	// ==============================================================================================
	//												strlen
	// ==============================================================================================

	const auto strlenTestResult = base::string::__base_strlen(&abcde[0]);

	Assert(strlenTestResult == 5);
	Assert(strlenTestResult == strlen(&abcde[0]));

	Assert(base::string::__base_any_strlen(&abcde[0]) == strlenTestResult);

	const auto strlenWCharTestResult = base::string::__base_wcslen(&abcde_W[0]);

	Assert(strlenWCharTestResult == 5);
	Assert(strlenWCharTestResult == wcslen(&abcde_W[0]));

	Assert(base::string::__base_any_strlen(&abcde_W[0]) == strlenWCharTestResult);

	Assert(base::string::__base_c32len(&abcde_32[0]) == 5);


	static const char* abcdeLong = "abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcabcdedeabcdeabcdeabcde";

	Assert(base::string::__base_strlen(&abcdeLong[0]) == 65);
	Assert(base::string::__base_strlen(&abcdeLong[0]) == strlen(&abcdeLong[0]));

	static const wchar_t* abcdeLong_W = L"abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcabcdedeabcdeabcdeabcde";

	Assert(base::string::__base_wcslen(&abcdeLong_W[0]) == 65);
	Assert(base::string::__base_wcslen(&abcdeLong_W[0]) == wcslen(&abcdeLong_W[0]));

	static const wchar_t* abcdeLong_W = L"abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcabcdedeabcdeabcdeabcde";

	Assert(base::string::__base_wcslen(&abcdeLong_W[0]) == 65);
	Assert(base::string::__base_wcslen(&abcdeLong_W[0]) == wcslen(&abcdeLong_W[0]));



	// ==============================================================================================
	//												Strstr
	// ==============================================================================================



	static const char strFind[]			= "Some test string";
	static const char strFindShort[]	= "Some string";

	Assert(base::string::__base_strstr(strFind, "Some") == strstr(strFind, "Some"));
	
	/*

	static const wchar_t strFind_W[]		= L"Some test string";
	static const wchar_t strFindShort_W[]	= L"Some string";*/

	//Assert(base::string::__base_wcsstr(strFind_W, L"Some") == wcsstr(strFind_W, L"Some"));

	//static const char32_t strFind_32[]		= U"Some test string";
	//static const char32_t strFindShort_32[] = U"Some string";

	// Assert(base::string::__base_c32str(strFind_32, U"Some") == &strFind_32[0]);

	return 0;
}
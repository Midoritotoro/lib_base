#include <base/core/string/String.h>

#include <src/core/string/crt/BaseStrlen.h>
#include <src/core/string/crt/BaseStrcmp.h>

int main(int argc, char** argv) {
	base::string::String string{ std::string("Some test string!") };
	std::wstring str(L"Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!");

	std::cout << base::string::__base_any_strlen(str.data());


	

	/*char a[16] = "aazf01010101010";
	char b[12] = "zzzzzzgdgfd";


	std::cout << base::string::__base_strcmpScalar(a, b);*/
	return 0;
}
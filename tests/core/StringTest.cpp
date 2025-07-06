#include <base/core/string/String.h>
#include <src/core/string/crt/BaseStrlen.h>


int main(int argc, char** argv) {
	//base::string::String string{ std::string("Some test string!") };
	std::wstring str(L"Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!");

	std::cout << base::string::__base_any_strlen(str.data());

	return 0;
}
#include <base/core/string/String.h>
#include <src/core/string/crt/BaseStrlen.h>


int main(int argc, char** argv) {
	//base::string::String string{ std::string("Some test string!") };
	std::string str("Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!Some test string!\0");

	std::cout << base::string::__base_strlen(str.data());

	return 0;
}
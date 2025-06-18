#include <base/core/string/String.h>

int main(int argc, char** argv) {
	base::string::String string{ std::string("Some test string!") };

	std::cout << string.data();

	return 0;
}
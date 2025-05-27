#include <base/core/string/StringConverter.h>

using namespace base;

int main(int argc, char* argv[]) {
	std::string string = "Hello world";
	
	auto result = string::StringConverter<>::convertString<char, wchar_t>(string.data(), string.size());
	//std::cout << "string: " << result.size();
}

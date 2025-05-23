#include <base/core/string/String.h>
#include <base/core/container/Vector.h>


using namespace base;

int main(int argc, char* argv[]) {
	string::String string = "Hello world";

	std::cout << "string: " << string.c_str();
}

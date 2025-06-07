#include <src/core/memory/crt/BaseMemcpy.h>
#include <src/core/memory/crt/BaseMemmove.h>

int main(int argc, char** argv) {
	std::cout << /*base::memory::*/__base_memmove();
	return 0;
}
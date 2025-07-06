#include <src/core/memory/crt/BaseMemcpy.h>
#include <src/core/memory/crt/BaseMemmove.h>

#include <vector>

int main(int argc, char** argv) {
	std::vector<int> oldVector;
	oldVector.reserve(1000);

	std::vector<int> newVector;
	newVector.reserve(1000);

	// base::memory::__base_memcpy(newVector.data(), oldVector.data(), oldVector.size() * 4);
	return 0;
}
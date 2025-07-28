#include <src/core/memory/crt/BaseMemcpy.h>
#include <src/core/memory/crt/BaseMemmove.h>

#include <vector>


int main(int argc, char** argv) {
	std::vector<int> oldVector;
	oldVector.resize(1000000000);

	std::vector<int> newVector;
	newVector.resize(1000000000);

	std::cout << newVector.size() * sizeof(int);
	base::memory::__base_memcpy(newVector.data(), oldVector.data(), newVector.size() * sizeof(int));
	//memcpy(newVector.data(), oldVector.data(), newVector.size() * sizeof(int));

	Assert(oldVector == newVector);




	return 0;
}
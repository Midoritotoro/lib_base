#include <base/Utility.h>
#include <base/Concurrent.h>


int main() {
	int variable = 10, secondVariable = 20;
	base::accumulateMax(variable, secondVariable);

	std::cout << base::take(variable);
	return 0;
}
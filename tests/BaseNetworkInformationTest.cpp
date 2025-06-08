#include <base/core/network/NetworkInformation.h>

int main(int argc, char** argv) {
	base::network::NetworksList parameters;
	parameters.reserve(100);

	base::network::NetworkInformation::enumerateNetworks(parameters);
	return 0;
}
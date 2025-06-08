#include <base/core/network/NetworkInformation.h>

int main(int argc, char** argv) {
	base::network::NetworkInformation::enumerateNetworks();
	return 0;
}
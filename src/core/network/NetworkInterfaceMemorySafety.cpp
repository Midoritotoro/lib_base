#include <src/core/network/NetworkInterfaceMemorySafety.h>

__BASE_NETWORK_NAMESPACE_BEGIN


void WlanInterfaceInformationListDeleter::operator()(wlanInterfaceInformationList_t* wlanInterfaceInformationList) noexcept {

}


void WlanAvailableNetworksListDeleter::operator()(wlanAvailableNetworksList_t* wlanAvailableNetworksList) noexcept {
	if (wlanAvailableNetworksList != nullptr)
		WlanFree
}


__BASE_NETWORK_NAMESPACE_END

#include <src/core/network/NetworkInterfaceMemorySafety.h>

__BASE_NETWORK_NAMESPACE_BEGIN

#if defined(base_os_windows)

void WlanInterfaceInformationListDeleter::operator()(wlanInterfaceInformationList_t* wlanInterfaceInformationList) noexcept {
	if (wlanInterfaceInformationList != nullptr)
		WlanFreeMemory(wlanInterfaceInformationList);
}


void WlanAvailableNetworksListDeleter::operator()(wlanAvailableNetworksList_t* wlanAvailableNetworksList) noexcept {
	if (wlanAvailableNetworksList != nullptr)
		WlanFreeMemory(wlanAvailableNetworksList);
}

#endif

__BASE_NETWORK_NAMESPACE_END

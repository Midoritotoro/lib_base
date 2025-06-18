#include <src/core/network/NetworkInterfaceMemorySafety.h>

__BASE_NETWORK_NAMESPACE_BEGIN


void WlanInterfaceInformationListDeleter::operator()(wlanInterfaceInformationList_t* wlanInterfaceInformationList) noexcept {
#if defined(OS_WIN)
	if (wlanInterfaceInformationList != nullptr)
		WlanFreeMemory(wlanInterfaceInformationList);
#else

#endif
}


void WlanAvailableNetworksListDeleter::operator()(wlanAvailableNetworksList_t* wlanAvailableNetworksList) noexcept {
#if defined(OS_WIN)
	if (wlanAvailableNetworksList != nullptr)
		WlanFreeMemory(wlanAvailableNetworksList);
#else

#endif
}


__BASE_NETWORK_NAMESPACE_END

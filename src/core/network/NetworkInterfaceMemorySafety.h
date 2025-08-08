#pragma once 

#include <src/core/network/NetworkTypes.h> 

__BASE_NETWORK_NAMESPACE_BEGIN

#if defined(base_os_windows)

struct WlanInterfaceInformationListDeleter {
	void operator()(wlanInterfaceInformationList_t* wlanInterfaceInformationList) noexcept;
};

struct WlanAvailableNetworksListDeleter {
	void operator()(wlanAvailableNetworksList_t* wlanAvailableNetworksList) noexcept;
};

#endif

__BASE_NETWORK_NAMESPACE_END

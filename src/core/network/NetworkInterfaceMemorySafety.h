#pragma once 

#include <src/core/network/NetworkTypes.h> 

__BASE_NETWORK_NAMESPACE_BEGIN

struct WlanInterfaceInformationListDeleter {
	void operator()(wlanInterfaceInformationList_t* wlanInterfaceInformationList) noexcept;
};

struct WlanAvailableNetworksListDeleter {
	void operator()(wlanAvailableNetworksList_t* wlanAvailableNetworksList) noexcept;
};

__BASE_NETWORK_NAMESPACE_END

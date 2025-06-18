#pragma once 

#include <src/core/network/NetworkParameters.h>
#include <memory>

__BASE_NETWORK_NAMESPACE_BEGIN

struct WlanInterfaceInformationListDeleter {
	void operator()(wlanInterfaceInformationList_t* wlanInterfaceInformationList) noexcept;
};

struct WlanAvailableNetworksListDeleter {
	void operator()(wlanAvailableNetworksList_t* wlanAvailableNetworksList) noexcept;
};

__BASE_NETWORK_NAMESPACE_END

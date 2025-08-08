#pragma once 

#include <base/core/arch/Platform.h>

#if defined(base_os_linux) || defined(base_os_mac) 

__BASE_NETWORK_NAMESPACE_BEGIN

class UnixNetworkInformation {
public:
	static void enumerateNetworks() noexcept;
};

__BASE_NETWORK_NAMESPACE_END

#endif
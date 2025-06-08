#pragma once 

#include <base/core/arch/Platform.h>

#if defined(OS_LINUX) || defined(OS_MAC) 

__BASE_NETWORK_NAMESPACE_BEGIN

class UnixNetworkInformation {
public:
	static void enumerateNetworks() noexcept;
};

__BASE_NETWORK_NAMESPACE_END

#endif
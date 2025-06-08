#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/network/NetworkParameters.h>

#if defined(OS_WIN) 

#include <wlanapi.h>

__BASE_NETWORK_NAMESPACE_BEGIN

class WindowsNetworkInformation {
public:
	static void enumerateNetworks(NetworksList& outputNetworkParameters) noexcept;
private:
	static BOOL WlanCloseHandleWrap(HANDLE handle) noexcept;
};

__BASE_NETWORK_NAMESPACE_END

#endif
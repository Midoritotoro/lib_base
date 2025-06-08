#pragma once 

#include <base/core/arch/Platform.h>

#if defined(OS_WIN) 

#include <wlanapi.h>

__BASE_NETWORK_NAMESPACE_BEGIN

class WindowsNetworkInformation {
public:
	static void enumerateNetworks() noexcept;
private:
	static BOOL WlanCloseHandleWrap(HANDLE handle) noexcept;
};

__BASE_NETWORK_NAMESPACE_END

#endif
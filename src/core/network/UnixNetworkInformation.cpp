#include <src/core/network/UnixNetworkInformation.h>

#if defined(OS_LINUX) || defined(OS_MAC) 

__BASE_NETWORK_NAMESPACE_BEGIN

void UnixNetworkInformation::enumerateNetworks() noexcept {

}

__BASE_NETWORK_NAMESPACE_END

#endif

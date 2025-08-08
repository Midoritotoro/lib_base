#include <src/core/network/UnixNetworkInformation.h>

#if defined(base_os_linux) || defined(base_os_mac) 

__BASE_NETWORK_NAMESPACE_BEGIN

void UnixNetworkInformation::enumerateNetworks() noexcept {

}

__BASE_NETWORK_NAMESPACE_END

#endif

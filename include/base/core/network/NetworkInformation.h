#pragma once 

#include <base/core/arch/Platform.h>

#if defined(base_os_windows) 
#  include <src/core/network/WindowsNetworkInformation.h>
#elif defined(base_os_linux) || defined(base_os_mac) 
#  include <src/core/network/UnixNetworkInformation.h>
#endif

__BASE_NETWORK_NAMESPACE_BEGIN

#if defined(base_os_windows) 
  using NetworkInformation = WindowsNetworkInformation;
#elif defined(base_os_linux) || defined(base_os_mac) 
  using NetworkInformation = UnixNetworkInformation;
#endif

__BASE_NETWORK_NAMESPACE_END

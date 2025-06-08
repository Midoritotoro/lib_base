#pragma once 

#include <base/core/arch/Platform.h>

#if defined(OS_WIN) 
#  include <src/core/network/WindowsNetworkInformation.h>
#elif defined(OS_LINUX) || defined(OS_MAC) 
#  include <src/core/network/UnixNetworkInformation.h>
#endif

__BASE_NETWORK_NAMESPACE_BEGIN

#if defined(OS_WIN) 
  using NetworkInformation = WindowsNetworkInformation;
#elif defined(OS_LINUX) || defined(OS_MAC) 
  using NetworkInformation = UnixNetworkInformation;
#endif

__BASE_NETWORK_NAMESPACE_END

#pragma once 

#include <base/core/arch/Platform.h>

#if defined(base_os_windows) 
#  include <wlanapi.h>
#elif defined(base_os_mac) || defined(base_os_linux)

#endif

__BASE_NETWORK_NAMESPACE_BEGIN

#if defined(base_os_windows)

  using wlanInterfaceInformationList_t	= WLAN_INTERFACE_INFO_LIST;
  using wlanInterfaceInformation_t		= WLAN_INTERFACE_INFO;

  using wlanAvailableNetworksList_t		= WLAN_AVAILABLE_NETWORK_LIST;
  using wlanAvailableNetwork_t			= WLAN_AVAILABLE_NETWORK;

#elif defined(base_os_mac) || defined(base_os_linux) 

#endif

__BASE_NETWORK_NAMESPACE_END

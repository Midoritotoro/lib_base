#pragma once 

#include <base/core/arch/Platform.h>

#if defined(OS_WIN) 
#  include <wlanapi.h>
#elif defined(OS_MAC) || defined(OS_LINUX)

#endif

__BASE_NETWORK_NAMESPACE_BEGIN

#if defined(OS_WIN)

  using wlanInterfaceInformationList_t	= WLAN_INTERFACE_INFO_LIST;
  using wlanInterfaceInformation_t		= WLAN_INTERFACE_INFO;

  using wlanAvailableNetworksList_t		= WLAN_AVAILABLE_NETWORK_LIST;
  using wlanAvailableNetwork_t			= WLAN_AVAILABLE_NETWORK;

#elif defined(OS_MAC) || defined(OS_LINUX) 

#endif

__BASE_NETWORK_NAMESPACE_END

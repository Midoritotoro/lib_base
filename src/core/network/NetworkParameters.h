#pragma once 

#include <base/core/arch/Platform.h> 
#include <base/core/string/String.h>

#include <src/core/network/WirelessConstants.h>

__BASE_NETWORK_NAMESPACE_BEGIN

#ifndef BASE_DOT11_SSID_MAX_LENGTH
#  define BASE_DOT11_SSID_MAX_LENGTH 32   
#endif

struct Dot11SSID {
	uint64 ssidLength;
	uchar ssid[BASE_DOT11_SSID_MAX_LENGTH];
};

struct NetworkParameters {
	// From 0 to 100
	uint32 connectionQuality = 0;

	// Indicates whether the network is connectable or not. 
	bool isNetworkConnectable = false; 

	// Indicates whether the network is currently connected or not. 
	bool isNetworkCurrentlyConnected = false;

	// Indicates whether security is enabled on the network
	bool isSecurityEnabled = false;

	// There is a profile for this network.
	bool haveProfile = false;

	// Contains the profile name associated with the network. 
	// If the network does not have a profile, this member will be empty.
	/*string::WString*/ std::wstring networkProfileName;

	// A Dot11AuthenticationAlgorithm value that indicates the default authentication
	// algorithm used to join this network for the first time.
	Dot11AuthenticationAlgorithm defaultAuthenticationAlgorithm;

	// A Dot11CipherAlgorithm value that indicates the default cipher algorithm
	// to be used when joining this network.
	Dot11CipherAlgorithm defaultCipherAlgorithm;
	Dot11SSID dot11Ssid;
};

using NetworksList = std::vector<NetworkParameters>;

__BASE_NETWORK_NAMESPACE_END

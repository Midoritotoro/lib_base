#pragma once 

#include <base/core/arch/Platform.h> 
#include <base/core/string/String.h>

#include <src/core/network/WirelessConstants.h>
#include <src/core/network/NetworkInterfaceMemorySafety.h>


#if defined(OS_WIN)

struct _WLAN_INTERFACE_INFO_LIST;
struct _WLAN_INTERFACE_INFO;

struct _WLAN_AVAILABLE_NETWORK_LIST;
struct _WLAN_AVAILABLE_NETWORK;

typedef struct _WLAN_INTERFACE_INFO_LIST WLAN_INTERFACE_INFO_LIST, *PWLAN_INTERFACE_INFO_LIST;
typedef struct _WLAN_INTERFACE_INFO WLAN_INTERFACE_INFO, *PWLAN_INTERFACE_INFO;

typedef struct _WLAN_AVAILABLE_NETWORK_LIST WLAN_AVAILABLE_NETWORK_LIST, *PWLAN_AVAILABLE_NETWORK_LIST;
typedef struct _WLAN_AVAILABLE_NETWORK WLAN_AVAILABLE_NETWORK, * PWLAN_AVAILABLE_NETWORK;

#endif

__BASE_NETWORK_NAMESPACE_BEGIN

using wlanInterfaceInformationList_t	= WLAN_INTERFACE_INFO_LIST;
using wlanInterfaceInformation_t		= WLAN_INTERFACE_INFO;

using wlanAvailableNetworksList_t		= WLAN_AVAILABLE_NETWORK_LIST;
using wlanAvailableNetwork_t			= WLAN_AVAILABLE_NETWORK;

using pWlanAvailableNetworkList_t		= std::unique_ptr<wlanAvailableNetworksList_t, WlanAvailableNetworksListDeleter>;
using pWlanInterfaceInformationList_t	= std::unique_ptr<wlanInterfaceInformationList_t, WlanInterfaceInformationListDeleter>;


#ifndef BASE_DOT11_SSID_MAX_LENGTH
#  define BASE_DOT11_SSID_MAX_LENGTH 32   
#endif

struct Dot11SSID {
	CONSTEXPR_CXX20 Dot11SSID(uchar* array, uint64 length) noexcept;

	CONSTEXPR_CXX20 ~Dot11SSID() = default;
	CONSTEXPR_CXX20 Dot11SSID()  = default;

	uint64 ssidLength = 0;
	uchar ssid[BASE_DOT11_SSID_MAX_LENGTH];
};

struct NetworkStateInformation {
	constexpr NetworkStateInformation(
		bool networkConnectable,
		bool networkCurrentlyConnected,
		bool securityEnabled,
		bool haveProfile
	) noexcept;

	constexpr ~NetworkStateInformation() = default;
	constexpr NetworkStateInformation() = default;

	// Indicates whether the network is connectable or not. 
	bool _networkConnectable : 2 = false;

	// Indicates whether the network is currently connected or not. 
	bool _networkCurrentlyConnected : 2 = false;

	// Indicates whether security is enabled on the network
	bool _securityEnabled : 2 = false;

	// There is a profile for this network.
	bool _haveProfile : 2 = false;
};

class NetworkParameters {
public:
	CONSTEXPR_CXX20 NetworkParameters() noexcept = default;
	CONSTEXPR_CXX20 ~NetworkParameters() noexcept = default;

	CONSTEXPR_CXX20 NetworkParameters(
		uint32							quality,
		std::wstring					profileName,
		NetworkStateInformation			information,
		Dot11AuthenticationAlgorithm	authAlgorithm,
		Dot11CipherAlgorithm			cipherAlgorithm,
		Dot11SSID						ssid
	) noexcept;

	CONSTEXPR_CXX20 void setConnectionQuality(uint32 quality) noexcept;
	CONSTEXPR_CXX20 NODISCARD uint32 connectionQuality() const noexcept;

	CONSTEXPR_CXX20 void setNetworkConnectable(bool networkConnectable) noexcept;
	CONSTEXPR_CXX20 NODISCARD bool isNetworkConnectable() const noexcept;

	CONSTEXPR_CXX20 void setNetworkCurrentlyConnected(bool networkCurrentlyConnected) noexcept;
	CONSTEXPR_CXX20 NODISCARD bool isNetworkCurrentlyConnected() const noexcept;

	CONSTEXPR_CXX20 void setSecurityEnabled(bool securityEnabled) noexcept;
	CONSTEXPR_CXX20 NODISCARD bool isSecurityEnabled() const noexcept;

	CONSTEXPR_CXX20 void setHaveProfile(bool value) noexcept;
	CONSTEXPR_CXX20 NODISCARD bool haveProfile() const noexcept;

	CONSTEXPR_CXX20 void setNetworkProfileName(std::wstring profileName) noexcept;
	CONSTEXPR_CXX20 NODISCARD std::wstring networkProfileName() noexcept;

	CONSTEXPR_CXX20 void setDefaultAuthenticationAlgorithm(Dot11AuthenticationAlgorithm algorithm) noexcept;
	CONSTEXPR_CXX20 NODISCARD Dot11AuthenticationAlgorithm defaultAuthenticationAlgorithm() const noexcept;

	CONSTEXPR_CXX20 void setDefaultCipherAlgorithm(Dot11CipherAlgorithm algorithm) noexcept;
	CONSTEXPR_CXX20 NODISCARD Dot11CipherAlgorithm defaultCipherAlgorithm() const noexcept;

	CONSTEXPR_CXX20 void setDot11Ssid(Dot11SSID ssid) noexcept;
	CONSTEXPR_CXX20 NODISCARD Dot11SSID dot11Ssid() const noexcept;
private:
	// From 0 to 100
	uint32 _connectionQuality = 0;

	NetworkStateInformation _networkStateInformation;

	// Contains the profile name associated with the network. 
	// If the network does not have a profile, this member will be empty.
	/*string::WString*/ std::wstring _networkProfileName;

	// A Dot11AuthenticationAlgorithm value that indicates the default authentication
	// algorithm used to join this network for the first time.
	Dot11AuthenticationAlgorithm _defaultAuthenticationAlgorithm;

	// A Dot11CipherAlgorithm value that indicates the default cipher algorithm
	// to be used when joining this network.
	Dot11CipherAlgorithm _defaultCipherAlgorithm;
	Dot11SSID _dot11Ssid;
};

using NetworksList = std::vector<NetworkParameters>;


CONSTEXPR_CXX20 Dot11SSID::Dot11SSID(uchar* array, uint64 length) noexcept {
#if BASE_HAS_CXX20
	if (is_constant_evaluated())
		for (auto i = 0; i < length; ++i)
			ssid[i] = array[i];
	else
#endif
		memcpy(&ssid[0], array, length);

	ssidLength = length;
}

constexpr NetworkStateInformation::NetworkStateInformation(
	bool networkConnectable,
	bool networkCurrentlyConnected,
	bool securityEnabled,
	bool haveProfile
) noexcept :
	_networkConnectable(networkConnectable),
	_networkCurrentlyConnected(networkCurrentlyConnected),
	_securityEnabled(securityEnabled),
	_haveProfile(haveProfile)
{}

CONSTEXPR_CXX20 NetworkParameters::NetworkParameters(
	uint32							quality,
	std::wstring					profileName,
	NetworkStateInformation			information,
	Dot11AuthenticationAlgorithm	authAlgorithm,
	Dot11CipherAlgorithm			cipherAlgorithm,
	Dot11SSID						ssid
) noexcept :
	_connectionQuality(quality),
	_networkStateInformation(information),
	_networkProfileName(profileName),
	_defaultAuthenticationAlgorithm(authAlgorithm),
	_defaultCipherAlgorithm(cipherAlgorithm),
	_dot11Ssid(ssid)
{}

CONSTEXPR_CXX20 void NetworkParameters::setConnectionQuality(uint32 quality) noexcept {
	_connectionQuality = quality;
}

CONSTEXPR_CXX20 NODISCARD uint32 NetworkParameters::connectionQuality() const noexcept {
	return _connectionQuality;
}

CONSTEXPR_CXX20 void NetworkParameters::setNetworkConnectable(bool networkConnectable) noexcept {
	_networkStateInformation._networkConnectable = networkConnectable;
}

CONSTEXPR_CXX20 NODISCARD bool NetworkParameters::isNetworkConnectable() const noexcept {
	return _networkStateInformation._networkConnectable;
}

CONSTEXPR_CXX20 void NetworkParameters::setNetworkCurrentlyConnected(bool networkCurrentlyConnected) noexcept {
	_networkStateInformation._networkCurrentlyConnected = networkCurrentlyConnected;
}

CONSTEXPR_CXX20 NODISCARD bool NetworkParameters::isNetworkCurrentlyConnected() const noexcept {
	return _networkStateInformation._networkCurrentlyConnected;
}

CONSTEXPR_CXX20 void NetworkParameters::setSecurityEnabled(bool securityEnabled) noexcept {
	_networkStateInformation._securityEnabled = securityEnabled;
}

CONSTEXPR_CXX20 NODISCARD bool NetworkParameters::isSecurityEnabled() const noexcept {
	return _networkStateInformation._securityEnabled;
}

CONSTEXPR_CXX20 void NetworkParameters::setHaveProfile(bool value) noexcept {
	_networkStateInformation._haveProfile = value;
}

CONSTEXPR_CXX20 NODISCARD bool NetworkParameters::haveProfile() const noexcept {
	return _networkStateInformation._haveProfile;
}

CONSTEXPR_CXX20 void NetworkParameters::setNetworkProfileName(std::wstring profileName) noexcept {
	_networkProfileName = profileName;
}

CONSTEXPR_CXX20 NODISCARD std::wstring NetworkParameters::networkProfileName() noexcept {
	return _networkProfileName;
}

CONSTEXPR_CXX20 void NetworkParameters::setDefaultAuthenticationAlgorithm(Dot11AuthenticationAlgorithm algorithm) noexcept {
	_defaultAuthenticationAlgorithm = algorithm;
}

CONSTEXPR_CXX20 NODISCARD Dot11AuthenticationAlgorithm NetworkParameters::defaultAuthenticationAlgorithm() const noexcept {
	return _defaultAuthenticationAlgorithm;
}

CONSTEXPR_CXX20 void NetworkParameters::setDefaultCipherAlgorithm(Dot11CipherAlgorithm algorithm) noexcept {
	_defaultCipherAlgorithm = algorithm;
}

CONSTEXPR_CXX20 NODISCARD Dot11CipherAlgorithm NetworkParameters::defaultCipherAlgorithm() const noexcept {
	return _defaultCipherAlgorithm;
}

CONSTEXPR_CXX20 void NetworkParameters::setDot11Ssid(Dot11SSID ssid) noexcept {
	_dot11Ssid = ssid;
}

CONSTEXPR_CXX20 NODISCARD Dot11SSID NetworkParameters::dot11Ssid() const noexcept {
	return _dot11Ssid;
}

__BASE_NETWORK_NAMESPACE_END

#pragma once 

#include <base/core/string/String.h>

#include <src/core/network/WirelessConstants.h>
#include <src/core/network/NetworkInterfaceMemorySafety.h>

__BASE_NETWORK_NAMESPACE_BEGIN

#if defined(base_os_windows)

using pWlanAvailableNetworkList_t		= std::unique_ptr<wlanAvailableNetworksList_t, WlanAvailableNetworksListDeleter>;
using pWlanInterfaceInformationList_t	= std::unique_ptr<wlanInterfaceInformationList_t, WlanInterfaceInformationListDeleter>;

#endif

#ifndef BASE_DOT11_SSID_MAX_LENGTH
#  define BASE_DOT11_SSID_MAX_LENGTH 32   
#endif

struct Dot11SSID {
	base_constexpr_cxx20 Dot11SSID(uchar* array, uint64 length) noexcept;

	base_constexpr_cxx20 ~Dot11SSID() = default;
	base_constexpr_cxx20 Dot11SSID()  = default;

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
	base_constexpr_cxx20 NetworkParameters() noexcept = default;
	base_constexpr_cxx20 ~NetworkParameters() noexcept = default;

	base_constexpr_cxx20 NetworkParameters(
		uint32							quality,
		std::wstring					profileName,
		NetworkStateInformation			information,
		Dot11AuthenticationAlgorithm	authAlgorithm,
		Dot11CipherAlgorithm			cipherAlgorithm,
		Dot11SSID						ssid
	) noexcept;

	base_constexpr_cxx20 void setConnectionQuality(uint32 quality) noexcept;
	base_constexpr_cxx20   uint32 connectionQuality() const noexcept;

	base_constexpr_cxx20 void setNetworkConnectable(bool networkConnectable) noexcept;
	base_constexpr_cxx20   bool isNetworkConnectable() const noexcept;

	base_constexpr_cxx20 void setNetworkCurrentlyConnected(bool networkCurrentlyConnected) noexcept;
	base_constexpr_cxx20   bool isNetworkCurrentlyConnected() const noexcept;

	base_constexpr_cxx20 void setSecurityEnabled(bool securityEnabled) noexcept;
	base_constexpr_cxx20   bool isSecurityEnabled() const noexcept;

	base_constexpr_cxx20 void setHaveProfile(bool value) noexcept;
	base_constexpr_cxx20   bool haveProfile() const noexcept;

	base_constexpr_cxx20 void setNetworkProfileName(std::wstring profileName) noexcept;
	base_constexpr_cxx20   std::wstring networkProfileName() noexcept;

	base_constexpr_cxx20 void setDefaultAuthenticationAlgorithm(Dot11AuthenticationAlgorithm algorithm) noexcept;
	base_constexpr_cxx20   Dot11AuthenticationAlgorithm defaultAuthenticationAlgorithm() const noexcept;

	base_constexpr_cxx20 void setDefaultCipherAlgorithm(Dot11CipherAlgorithm algorithm) noexcept;
	base_constexpr_cxx20   Dot11CipherAlgorithm defaultCipherAlgorithm() const noexcept;

	base_constexpr_cxx20 void setDot11Ssid(Dot11SSID ssid) noexcept;
	base_constexpr_cxx20   Dot11SSID dot11Ssid() const noexcept;
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


base_constexpr_cxx20 Dot11SSID::Dot11SSID(uchar* array, uint64 length) noexcept {
#if base_has_cxx20
	if (base::type_traits::is_constant_evaluated())
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

base_constexpr_cxx20 NetworkParameters::NetworkParameters(
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

base_constexpr_cxx20 void NetworkParameters::setConnectionQuality(uint32 quality) noexcept {
	_connectionQuality = quality;
}

base_constexpr_cxx20   uint32 NetworkParameters::connectionQuality() const noexcept {
	return _connectionQuality;
}

base_constexpr_cxx20 void NetworkParameters::setNetworkConnectable(bool networkConnectable) noexcept {
	_networkStateInformation._networkConnectable = networkConnectable;
}

base_constexpr_cxx20   bool NetworkParameters::isNetworkConnectable() const noexcept {
	return _networkStateInformation._networkConnectable;
}

base_constexpr_cxx20 void NetworkParameters::setNetworkCurrentlyConnected(bool networkCurrentlyConnected) noexcept {
	_networkStateInformation._networkCurrentlyConnected = networkCurrentlyConnected;
}

base_constexpr_cxx20   bool NetworkParameters::isNetworkCurrentlyConnected() const noexcept {
	return _networkStateInformation._networkCurrentlyConnected;
}

base_constexpr_cxx20 void NetworkParameters::setSecurityEnabled(bool securityEnabled) noexcept {
	_networkStateInformation._securityEnabled = securityEnabled;
}

base_constexpr_cxx20   bool NetworkParameters::isSecurityEnabled() const noexcept {
	return _networkStateInformation._securityEnabled;
}

base_constexpr_cxx20 void NetworkParameters::setHaveProfile(bool value) noexcept {
	_networkStateInformation._haveProfile = value;
}

base_constexpr_cxx20   bool NetworkParameters::haveProfile() const noexcept {
	return _networkStateInformation._haveProfile;
}

base_constexpr_cxx20 void NetworkParameters::setNetworkProfileName(std::wstring profileName) noexcept {
	_networkProfileName = profileName;
}

base_constexpr_cxx20   std::wstring NetworkParameters::networkProfileName() noexcept {
	return _networkProfileName;
}

base_constexpr_cxx20 void NetworkParameters::setDefaultAuthenticationAlgorithm(Dot11AuthenticationAlgorithm algorithm) noexcept {
	_defaultAuthenticationAlgorithm = algorithm;
}

base_constexpr_cxx20   Dot11AuthenticationAlgorithm NetworkParameters::defaultAuthenticationAlgorithm() const noexcept {
	return _defaultAuthenticationAlgorithm;
}

base_constexpr_cxx20 void NetworkParameters::setDefaultCipherAlgorithm(Dot11CipherAlgorithm algorithm) noexcept {
	_defaultCipherAlgorithm = algorithm;
}

base_constexpr_cxx20   Dot11CipherAlgorithm NetworkParameters::defaultCipherAlgorithm() const noexcept {
	return _defaultCipherAlgorithm;
}

base_constexpr_cxx20 void NetworkParameters::setDot11Ssid(Dot11SSID ssid) noexcept {
	_dot11Ssid = ssid;
}

base_constexpr_cxx20   Dot11SSID NetworkParameters::dot11Ssid() const noexcept {
	return _dot11Ssid;
}

__BASE_NETWORK_NAMESPACE_END

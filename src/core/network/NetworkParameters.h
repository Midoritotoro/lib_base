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

class NetworkParameters {
public:
	NetworkParameters(
		uint32							quality,
		bool							networkConnectable,
		bool							networkCurrentlyConnected,
		bool							securityEnabled,
		bool							_haveProfile_,
		std::wstring					profileName,
		Dot11AuthenticationAlgorithm	authAlgorithm,
		Dot11CipherAlgorithm			cipherAlgorithm,
		Dot11SSID						ssid
	) noexcept;

	constexpr void setConnectionQuality(uint32 quality) noexcept;
	constexpr NODISCARD uint32 connectionQuality() const noexcept;

	constexpr void setNetworkConnectable(bool networkConnectable) noexcept;
	constexpr NODISCARD bool isNetworkConnectable() const noexcept;

	constexpr void setNetworkCurrentlyConnected(bool networkCurrentlyConnected) noexcept;
	constexpr NODISCARD bool isNetworkCurrentlyConnected() const noexcept;

	constexpr void setSecurityEnabled(bool securityEnabled) noexcept;
	constexpr NODISCARD bool isSecurityEnabled() const noexcept;

	constexpr void setHaveProfile(bool value) noexcept;
	constexpr NODISCARD bool haveProfile() const noexcept;

	constexpr void setNetworkProfileName(std::wstring profileName) noexcept;
	constexpr NODISCARD std::wstring networkProfileName() const noexcept;
	constexpr NODISCARD wchar_t* networkProfileName() noexcept;

	constexpr void setDefaultAuthenticationAlgorithm(Dot11AuthenticationAlgorithm algorithm) noexcept;
	constexpr NODISCARD Dot11AuthenticationAlgorithm defaultAuthenticationAlgorithm() const noexcept;

	constexpr void setDefaultCipherAlgorithm(Dot11CipherAlgorithm algorithm) noexcept;
	constexpr NODISCARD Dot11CipherAlgorithm defaultCipherAlgorithm() const noexcept;

	constexpr void setDot11Ssid(Dot11SSID ssid) noexcept;
	constexpr NODISCARD Dot11SSID dot11Ssid() const noexcept;
private:
	// From 0 to 100
	uint32 _connectionQuality = 0;

	// Indicates whether the network is connectable or not. 
	bool _isNetworkConnectable = false; 

	// Indicates whether the network is currently connected or not. 
	bool _isNetworkCurrentlyConnected = false;

	// Indicates whether security is enabled on the network
	bool _isSecurityEnabled = false;

	// There is a profile for this network.
	bool _haveProfile = false;

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

__BASE_NETWORK_NAMESPACE_END

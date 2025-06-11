#include <src/core/network/NetworkParameters.h>

__BASE_NETWORK_NAMESPACE_BEGIN

NetworkParameters::NetworkParameters(
	uint32							quality,
	bool							networkConnectable,
	bool							networkCurrentlyConnected,
	bool							securityEnabled,
	bool							_haveProfile_,
	std::wstring					profileName,
	Dot11AuthenticationAlgorithm	authAlgorithm,
	Dot11CipherAlgorithm			cipherAlgorithm,
	Dot11SSID						ssid
) noexcept :
	_connectionQuality(quality),
	_isNetworkConnectable(networkConnectable),
	_isNetworkCurrentlyConnected(networkCurrentlyConnected),
	_isSecurityEnabled(securityEnabled),
	_haveProfile(_haveProfile_),
	_networkProfileName(profileName),
	_defaultAuthenticationAlgorithm(authAlgorithm),
	_defaultCipherAlgorithm(cipherAlgorithm),
	_dot11Ssid(ssid)
{}

constexpr void NetworkParameters::setConnectionQuality(uint32 quality) noexcept {
	_connectionQuality = quality;
}

constexpr NODISCARD uint32 NetworkParameters::connectionQuality() const noexcept {
	return _connectionQuality;
}

constexpr void NetworkParameters::setNetworkConnectable(bool networkConnectable) noexcept {
	_isNetworkConnectable = networkConnectable;
}

constexpr NODISCARD bool NetworkParameters::isNetworkConnectable() const noexcept {
	return _isNetworkConnectable;
}

constexpr void NetworkParameters::setNetworkCurrentlyConnected(bool networkCurrentlyConnected) noexcept {
	_isNetworkCurrentlyConnected = networkCurrentlyConnected;
}

constexpr NODISCARD bool NetworkParameters::isNetworkCurrentlyConnected() const noexcept {
	return _isNetworkCurrentlyConnected;
}

constexpr void NetworkParameters::setSecurityEnabled(bool securityEnabled) noexcept {
	_isSecurityEnabled = securityEnabled;
}

constexpr NODISCARD bool NetworkParameters::isSecurityEnabled() const noexcept {
	return _isSecurityEnabled;
}

constexpr void NetworkParameters::setHaveProfile(bool value) noexcept {
	_haveProfile = value;
}

constexpr NODISCARD bool NetworkParameters::haveProfile() const noexcept {
	return _haveProfile;
}

constexpr void NetworkParameters::setNetworkProfileName(std::wstring profileName) noexcept {
	_networkProfileName = profileName;
}

constexpr NODISCARD std::wstring NetworkParameters::networkProfileName() const noexcept {
	return _networkProfileName;
}

constexpr NODISCARD wchar_t* NetworkParameters::networkProfileName() noexcept {
	return _networkProfileName.data();
}

constexpr void NetworkParameters::setDefaultAuthenticationAlgorithm(Dot11AuthenticationAlgorithm algorithm) noexcept {
	_defaultAuthenticationAlgorithm = algorithm
}

constexpr NODISCARD Dot11AuthenticationAlgorithm NetworkParameters::defaultAuthenticationAlgorithm() const noexcept {
	return _defaultAuthenticationAlgorithm;
}

constexpr void NetworkParameters::setDefaultCipherAlgorithm(Dot11CipherAlgorithm algorithm) noexcept {
	_defaultCipherAlgorithm = algorithm;
}

constexpr NODISCARD Dot11CipherAlgorithm NetworkParameters::defaultCipherAlgorithm() const noexcept {
	return _defaultCipherAlgorithm;
}

constexpr void NetworkParameters::setDot11Ssid(Dot11SSID ssid) noexcept {
	_dot11Ssid = ssid;
}

constexpr NODISCARD Dot11SSID NetworkParameters::dot11Ssid() const noexcept {
	return _dot11Ssid;
}

__BASE_NETWORK_NAMESPACE_END

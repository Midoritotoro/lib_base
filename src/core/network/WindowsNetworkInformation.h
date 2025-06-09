#pragma once 

#include <src/core/network/NetworkParameters.h>

#if defined(OS_WIN) 

#include <wlanapi.h>

__BASE_NETWORK_NAMESPACE_BEGIN

class WindowsNetworkInformation {
public:
	static void enumerateNetworks(NetworksList& outputNetworkParameters) noexcept;
private:
	static BOOL WlanCloseHandleWrap(HANDLE handle) noexcept;

	static NODISCARD Dot11AuthenticationAlgorithm AuthenticationAlgorithmFromWinApi(DOT11_AUTH_ALGORITHM winApiAuthAlgorithm) noexcept;
	static NODISCARD Dot11CipherAlgorithm CipherAlgorithmFromWinApi(DOT11_CIPHER_ALGORITHM winApiCipherAlgorithm) noexcept;
};

__BASE_NETWORK_NAMESPACE_END

#endif
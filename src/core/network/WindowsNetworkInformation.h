#pragma once 

#include <src/core/network/NetworkParameters.h>

#if defined(base_os_windows) 

__BASE_IO_NAMESPACE_BEGIN
class WindowsSmartHandle;
__BASE_IO_NAMESPACE_END


__BASE_NETWORK_NAMESPACE_BEGIN

class WindowsNetworkInformation {
public:
	static void scanAvailableNetworks() noexcept;
	static void enumerateNetworks(NetworksList& outputNetworkParameters) noexcept;
private:
	static   bool_t wlanOpen(io::WindowsSmartHandle* pHandle) noexcept;
	static   bool_t wlanEnumInterfaces(handle_t handle) noexcept;

	static   dword_t WlanCloseHandleWrap(handle_t handle) noexcept;

	static   Dot11AuthenticationAlgorithm AuthenticationAlgorithmFromWinApi(
		WinApiDot11AuthAlgorithm_t winApiAuthAlgorithm) noexcept;

	static   Dot11CipherAlgorithm CipherAlgorithmFromWinApi(
		WinApiDot11CipherAlgorithm_t winApiCipherAlgorithm) noexcept;
};

__BASE_NETWORK_NAMESPACE_END

#endif
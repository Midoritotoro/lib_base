#pragma once 

#include <src/core/network/NetworkParameters.h>

#if defined(OS_WIN) 

__BASE_IO_NAMESPACE_BEGIN
class WindowsSmartHandle;
__BASE_IO_NAMESPACE_END


__BASE_NETWORK_NAMESPACE_BEGIN

class WindowsNetworkInformation {
public:
	static void scanAvailableNetworks() noexcept;
	static void enumerateNetworks(NetworksList& outputNetworkParameters) noexcept;
private:
	static NODISCARD bool_t wlanOpen(io::WindowsSmartHandle* pHandle) noexcept;
	static NODISCARD bool_t wlanEnumInterfaces(handle_t handle) noexcept;

	static NODISCARD dword WlanCloseHandleWrap(handle_t handle) noexcept;

	static NODISCARD Dot11AuthenticationAlgorithm AuthenticationAlgorithmFromWinApi(
		WinApiDot11AuthAlgorithm_t winApiAuthAlgorithm) noexcept;

	static NODISCARD Dot11CipherAlgorithm CipherAlgorithmFromWinApi(
		WinApiDot11CipherAlgorithm_t winApiCipherAlgorithm) noexcept;
};

__BASE_NETWORK_NAMESPACE_END

#endif
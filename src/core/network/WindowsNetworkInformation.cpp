#include <src/core/network/WindowsNetworkInformation.h>
#include <base/core/io/WindowsSmartHandle.h>

#if defined(OS_WIN)

#include <wlanapi.h>

#include <objbase.h>
#include <wtypes.h>

#ifndef __BASE_WINDOWS_NETWORK_GUID_SIZE_IN_BYTES
#  define __BASE_WINDOWS_NETWORK_GUID_SIZE_IN_BYTES (78)
#endif // __BASE_WINDOWS_NETWORK_GUID_SIZE_IN_BYTES

// ======================================================================================================
// WLAN API Version: The highest version the client supports.
//
// ----------------------------------------------------------------------------------------------------
// Value | Meaning
// ------|-------------------------------------------------------------
//  1  | Client version for Windows XP with SP3 and Wireless LAN API for Windows XP with SP2.
//  2  | Client version for Windows Vista and Windows Server 2008
// ======================================================================================================
#define BASE_WLAN_API_VERSION (2)

__BASE_NETWORK_NAMESPACE_BEGIN

void WindowsNetworkInformation::scanAvailableNetworks() noexcept {
	auto wlanSmartHandle = io::WindowsSmartHandle();
	wlanOpen(&wlanSmartHandle);

	wlanInterfaceInformationList_t* wlanInterfaceList = nullptr;
	const auto successfullyEnumerated = WlanEnumInterfaces(
		wlanSmartHandle.handle(), nullptr, &wlanInterfaceList);

	if (successfullyEnumerated != ERROR_SUCCESS)
		return;

	wlanInterfaceInformation_t* wlanInterfaceInformation = nullptr;

	for (dword i = 0; i < wlanInterfaceList->dwNumberOfItems; ++i) {
		wlanInterfaceInformation = static_cast<wlanInterfaceInformation_t*>(&wlanInterfaceList->InterfaceInfo[i]);

		// do something
		
	} 

}

void WindowsNetworkInformation::enumerateNetworks(NetworksList& outputNetworkParameters) noexcept {
	dword currentWlanApiVersion = 0;

	HANDLE wlanTempHandle = nullptr;
	const auto isOpened = WlanOpenHandle(
		BASE_WLAN_API_VERSION, nullptr, 
		&currentWlanApiVersion, &wlanTempHandle);

	if (isOpened != ERROR_SUCCESS)
		return;

	auto wlanSmartHandle = io::WindowsSmartHandle();

	wlanSmartHandle.setDeleteCallback(WlanCloseHandleWrap);
	wlanSmartHandle.setAutoDelete(true);

	wlanSmartHandle.setHandle(wlanTempHandle);

	wlanInterfaceInformationList_t* wlanInterfaceList = nullptr;
	const auto successfullyEnumerated = WlanEnumInterfaces(
		wlanSmartHandle.handle(), nullptr, &wlanInterfaceList);

	if (successfullyEnumerated != ERROR_SUCCESS)
		return;

	wchar_t GuidString[__BASE_WINDOWS_NETWORK_GUID_SIZE_IN_BYTES / sizeof(WCHAR)] = {0};

	wlanInterfaceInformation_t* wlanInterfaceInformation = nullptr;
	wlanAvailableNetworksList_t *wlanAvailableNetworksList = nullptr;

	wlanAvailableNetwork_t* wlanAvailableNetwork = nullptr;

	for (int i = 0; i < (int)wlanInterfaceList->dwNumberOfItems; ++i) {
		wlanInterfaceInformation = static_cast<wlanInterfaceInformation_t*>(&wlanInterfaceList->InterfaceInfo[i]);

		const auto iRet = StringFromGUID2(
			wlanInterfaceInformation->InterfaceGuid, (LPOLESTR)&GuidString,
			sizeof(GuidString) / sizeof(*GuidString));

		AssertLog(iRet != 0, "base::network::WindowsNetworkInformation: fail");

		const auto getAvailableNetworksResult = WlanGetAvailableNetworkList(
			wlanSmartHandle.handle(), &wlanInterfaceInformation->InterfaceGuid,
			0, nullptr, &wlanAvailableNetworksList);

		if (getAvailableNetworksResult != ERROR_SUCCESS)
			continue;

		if ((outputNetworkParameters.capacity() - outputNetworkParameters.size()) < wlanAvailableNetworksList->dwNumberOfItems)
			outputNetworkParameters.reserve(outputNetworkParameters.size() + wlanAvailableNetworksList->dwNumberOfItems);

		for (int j = 0; j < wlanAvailableNetworksList->dwNumberOfItems; ++j) {
			wlanAvailableNetwork = (wlanAvailableNetwork_t*) & wlanAvailableNetworksList->Network[j];

			const auto isNetworkCurrentlyConnected = wlanAvailableNetwork->dwFlags
				&& wlanAvailableNetwork->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED;

			const auto haveProfile = wlanAvailableNetwork->dwFlags
				&& wlanAvailableNetwork->dwFlags & WLAN_AVAILABLE_NETWORK_HAS_PROFILE;

			const auto networkStateInformation = NetworkStateInformation(
				isNetworkCurrentlyConnected, wlanAvailableNetwork->bSecurityEnabled,
				haveProfile, wlanAvailableNetwork->bNetworkConnectable);

			const auto authAlgorithm = AuthenticationAlgorithmFromWinApi(
				wlanAvailableNetwork->dot11DefaultAuthAlgorithm);

			const auto cipherAlgorithm = CipherAlgorithmFromWinApi(
				wlanAvailableNetwork->dot11DefaultCipherAlgorithm);
			
			Dot11SSID netSsid(&wlanAvailableNetwork->dot11Ssid.ucSSID[0],
				wlanAvailableNetwork->dot11Ssid.uSSIDLength);

			outputNetworkParameters.emplace_back(
				wlanAvailableNetwork->wlanSignalQuality, std::wstring(&wlanAvailableNetwork->strProfileName[0],
					lstrlenW(&wlanAvailableNetwork->strProfileName[0])),
				networkStateInformation, authAlgorithm, cipherAlgorithm, netSsid
			);
		}	
	}

	if (wlanAvailableNetworksList != nullptr)
		WlanFreeMemory(wlanAvailableNetworksList);

	if (wlanInterfaceList != nullptr)
		WlanFreeMemory(wlanInterfaceList);
}

bool_t WindowsNetworkInformation::wlanOpen(io::WindowsSmartHandle* pHandle) noexcept {
	dword currentWlanApiVersion = 0;

	handle_t wlanTempHandle = nullptr;
	const auto openCode = WlanOpenHandle(BASE_WLAN_API_VERSION, nullptr, &currentWlanApiVersion, &wlanTempHandle);

	if (openCode != ERROR_SUCCESS)
		return openCode;

	pHandle->setDeleteCallback(WlanCloseHandleWrap);
	pHandle->setAutoDelete(true);

	pHandle->setHandle(wlanTempHandle);
}

bool_t WindowsNetworkInformation::wlanEnumInterfaces(handle_t handle) noexcept {
	wlanInterfaceInformationList_t* wlanInterfaceList = nullptr;
	const auto successfullyEnumerated = WlanEnumInterfaces(handle, nullptr, &wlanInterfaceList);

	if (successfullyEnumerated != ERROR_SUCCESS)
		return successfullyEnumerated;

	wlanInterfaceInformation_t* wlanInterfaceInformation = nullptr;

	for (dword i = 0; i < wlanInterfaceList->dwNumberOfItems; ++i) {
		wlanInterfaceInformation = static_cast<wlanInterfaceInformation_t*>(&wlanInterfaceList->InterfaceInfo[i]);

		// do something

	}
}

dword WindowsNetworkInformation::WlanCloseHandleWrap(handle_t handle) noexcept {
	return WlanCloseHandle(handle, nullptr);
}

Dot11AuthenticationAlgorithm WindowsNetworkInformation::AuthenticationAlgorithmFromWinApi(WinApiDot11AuthAlgorithm_t winApiAuthAlgorithm) noexcept {
	switch (winApiAuthAlgorithm) {
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_80211_OPEN:			return Dot11AuthenticationAlgorithm::Ieee_802_11_AuthenticationAlgorithmOpen;
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_80211_SHARED_KEY:	return Dot11AuthenticationAlgorithm::Ieee_802_11_AuthenticationAlgorithmSharedKey;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA:					return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA;
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA_PSK:				return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA_PSK;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA_NONE:			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA_NONE;
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_RSNA:				return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmRSNA;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_RSNA_PSK:			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmRSNA_PSK;
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA3:				return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA3;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA3_SAE:			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA3_SAE;
#if defined(OS_WIN) && (NTDDI_VERSION >= NTDDI_WIN10_VB)
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_OWE:					return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmOWE;
#endif

#if defined(OS_WIN) && (NTDDI_VERSION >= NTDDI_WIN10_FE)
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA3_ENT:			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA3_ENT;
#endif
	}

	AssertUnreachable();
	return {};
}

Dot11CipherAlgorithm WindowsNetworkInformation::CipherAlgorithmFromWinApi(WinApiDot11CipherAlgorithm_t winApiCipherAlgorithm) noexcept {
	switch (winApiCipherAlgorithm) {
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_NONE:			return Dot11CipherAlgorithm::CipherAlgorithmNone;
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WEP40:			return Dot11CipherAlgorithm::CipherAlgorithmWEP40;
		
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_TKIP:			return Dot11CipherAlgorithm::CipherAlgorithmTKIP;
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_CCMP:			return Dot11CipherAlgorithm::CipherAlgorithmCCMP;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WEP104:			return Dot11CipherAlgorithm::CipherAlgorithmWEP104;
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP:				return Dot11CipherAlgorithm::CipherAlgorithmBIP;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_GCMP:			return Dot11CipherAlgorithm::CipherAlgorithmGCMP;
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_GCMP_256:		return Dot11CipherAlgorithm::CipherAlgorithmGCMP_256;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_CCMP_256:		return Dot11CipherAlgorithm::CipherAlgorithmCCMP_256;
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP_GMAC_128:	return Dot11CipherAlgorithm::CipherAlgorithmBIP_GMAC_128;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP_GMAC_256:	return Dot11CipherAlgorithm::CipherAlgorithmBIP_GMAC_256;
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP_CMAC_256:	return Dot11CipherAlgorithm::CipherAlgorithmBIP_CMAC_256;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WPA_USE_GROUP:	return Dot11CipherAlgorithm::CipherAlgorithmWPA_USE_GROUP;
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WEP:				return Dot11CipherAlgorithm::CipherAlgorithmWEP;
	}

	AssertUnreachable();
	return {};
}

__BASE_NETWORK_NAMESPACE_END

#endif

#include <src/core/network/WindowsNetworkInformation.h>
#include <base/core/io/WindowsSmartHandle.h>

#if defined(OS_WIN)

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

	WLAN_INTERFACE_INFO_LIST* wlanInterfaceList = nullptr;
	const auto successfullyEnumerated = WlanEnumInterfaces(
		wlanSmartHandle.handle(), nullptr, &wlanInterfaceList);

	if (successfullyEnumerated != ERROR_SUCCESS)
		return;

	WCHAR GuidString[39] = { 0 };

	WLAN_INTERFACE_INFO* wlanInterfaceInformation = nullptr;
	WLAN_AVAILABLE_NETWORK_LIST* wlanAvailableNetworksList = nullptr;

	WLAN_AVAILABLE_NETWORK* wlanAvailableNetwork = nullptr;

	for (int i = 0; i < (int)wlanInterfaceList->dwNumberOfItems; ++i) {
		wlanInterfaceInformation = (WLAN_INTERFACE_INFO*)&wlanInterfaceList->InterfaceInfo[i];

		const auto iRet = StringFromGUID2(
			wlanInterfaceInformation->InterfaceGuid, (LPOLESTR)&GuidString,
			sizeof(GuidString) / sizeof(*GuidString));

		/*if (iRet == 0) { fail }*/

		const auto getAvailableNetworksResult = WlanGetAvailableNetworkList(
			wlanSmartHandle.handle(), &wlanInterfaceInformation->InterfaceGuid,
			0, nullptr, &wlanAvailableNetworksList);

		if (getAvailableNetworksResult != ERROR_SUCCESS)
			continue;

		for (int j = 0; j < wlanAvailableNetworksList->dwNumberOfItems; ++j) {
			wlanAvailableNetwork = (WLAN_AVAILABLE_NETWORK*)&wlanAvailableNetworksList->Network[j];

			wprintf(L"  Profile Name[%u]:  %ws\n", j, wlanAvailableNetwork->strProfileName);
			wprintf(L"  SSID[%u]:\t\t ", j);

			if (wlanAvailableNetwork->dot11Ssid.uSSIDLength == 0)
				wprintf(L"\n");
			else {
				for (int k = 0; k < wlanAvailableNetwork->dot11Ssid.uSSIDLength; ++k)
					wprintf(L"%c", (int)wlanAvailableNetwork->dot11Ssid.ucSSID[k]);

				wprintf(L"\n");
			}

			outputNetworkParameters[j].isNetworkConnectable = wlanAvailableNetwork->bNetworkConnectable;
			outputNetworkParameters[j].isSecurityEnabled = wlanAvailableNetwork->bSecurityEnabled;

			outputNetworkParameters[j].networkProfileName = wlanAvailableNetwork->strProfileName;
			outputNetworkParameters[j].connectionQuality = (wlanAvailableNetwork->wlanSignalQuality == 0) 
				? -100 
				: (wlanAvailableNetwork->wlanSignalQuality == 100) 
					?  -50 
				:  -100 + (wlanAvailableNetwork->wlanSignalQuality / 2);

			switch (wlanAvailableNetwork->dot11DefaultAuthAlgorithm) {
				case DOT11_AUTH_ALGO_80211_OPEN:
					outputNetworkParameters[j].defaultAuthenticationAlgorithm = 
						Dot11AuthenticationAlgorithm::Ieee_802_11_AuthenticationAlgorithmOpen;
					break;
				case DOT11_AUTH_ALGO_80211_SHARED_KEY:
					outputNetworkParameters[j].defaultAuthenticationAlgorithm =
						Dot11AuthenticationAlgorithm::Ieee_802_11_AuthenticationAlgorithmSharedKey;
					break;
				case DOT11_AUTH_ALGO_WPA:
					outputNetworkParameters[j].defaultAuthenticationAlgorithm =
						Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA;
					break;
				case DOT11_AUTH_ALGO_WPA_PSK:
					outputNetworkParameters[j].defaultAuthenticationAlgorithm =
						Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA_PSK;
					break;
				case DOT11_AUTH_ALGO_WPA_NONE:
					outputNetworkParameters[j].defaultAuthenticationAlgorithm =
						Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA_NONE;
					break;
				case DOT11_AUTH_ALGO_RSNA:
					outputNetworkParameters[j].defaultAuthenticationAlgorithm =
						Dot11AuthenticationAlgorithm::AuthenticationAlgorithmRSNA;
					break;
				case DOT11_AUTH_ALGO_RSNA_PSK:
					outputNetworkParameters[j].defaultAuthenticationAlgorithm =
						Dot11AuthenticationAlgorithm::AuthenticationAlgorithmRSNA_PSK;
					break;
				default:
					break;
			}

			switch (wlanAvailableNetwork->dot11DefaultCipherAlgorithm) {
				case DOT11_CIPHER_ALGO_NONE:
					outputNetworkParameters[j].defaultCipherAlgorithm = Dot11CipherAlgorithm::CipherAlgorithmNone;
					break;
				case DOT11_CIPHER_ALGO_WEP40:
					outputNetworkParameters[j].defaultCipherAlgorithm = Dot11CipherAlgorithm::CipherAlgorithmWEP40;
					break;
				case DOT11_CIPHER_ALGO_TKIP:
					outputNetworkParameters[j].defaultCipherAlgorithm = Dot11CipherAlgorithm::CipherAlgorithmTKIP;
					break;
				case DOT11_CIPHER_ALGO_CCMP:
					outputNetworkParameters[j].defaultCipherAlgorithm = Dot11CipherAlgorithm::CipherAlgorithmCCMP;
					break;
				case DOT11_CIPHER_ALGO_WEP104:
					outputNetworkParameters[j].defaultCipherAlgorithm = Dot11CipherAlgorithm::CipherAlgorithmWEP104;
					break;
				case DOT11_CIPHER_ALGO_WEP:
					outputNetworkParameters[j].defaultCipherAlgorithm = Dot11CipherAlgorithm::CipherAlgorithmWEP;
					break;
				default:
					break;
			}

			outputNetworkParameters[j].isNetworkCurrentlyConnected =
				wlanAvailableNetwork->dwFlags && wlanAvailableNetwork->dwFlags
					& WLAN_AVAILABLE_NETWORK_CONNECTED;
		}	
	}

	if (wlanAvailableNetworksList != nullptr) {
		WlanFreeMemory(wlanAvailableNetworksList);
		wlanAvailableNetworksList = nullptr;
	}

	if (wlanInterfaceList != nullptr) {
		WlanFreeMemory(wlanInterfaceList);
		wlanInterfaceList = nullptr;
	}
}

BOOL WindowsNetworkInformation::WlanCloseHandleWrap(HANDLE handle) noexcept {
	return WlanCloseHandle(handle, nullptr);
}

__BASE_NETWORK_NAMESPACE_END

#endif

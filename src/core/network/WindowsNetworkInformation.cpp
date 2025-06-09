#include <src/core/network/WindowsNetworkInformation.h>
#include <base/core/io/WindowsSmartHandle.h>

#if defined(OS_WIN)

#include <objbase.h>
#include <wtypes.h>

#include <base/core/string/StringConverter.h>

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

	std::cout << wlanInterfaceList->dwNumberOfItems << '\n';
	
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

		if ((outputNetworkParameters.capacity() - outputNetworkParameters.size()) < wlanAvailableNetworksList->dwNumberOfItems)
			outputNetworkParameters.resize(wlanAvailableNetworksList->dwNumberOfItems);

		std::cout << wlanAvailableNetworksList->dwNumberOfItems << '\n';

		for (int j = 0; j < wlanAvailableNetworksList->dwNumberOfItems; ++j) {
			wlanAvailableNetwork = (WLAN_AVAILABLE_NETWORK*) & wlanAvailableNetworksList->Network[j];

			std::u8string str; 
			str.reserve(wlanAvailableNetwork->dot11Ssid.uSSIDLength);

			std::cout << " length: " << wlanAvailableNetwork->dot11Ssid.uSSIDLength << '\n';
			for (int ph = 0; ph < wlanAvailableNetwork->dot11Ssid.uSSIDLength; ++ph)
				str.push_back(wlanAvailableNetwork->dot11Ssid.ucSSID[ph]);

			std::wcout << string::StringConverter<>::convertString<char8_t, wchar_t>(str.data(), str.length()).data();
			
			outputNetworkParameters[j].dot11Ssid.ssidLength = wlanAvailableNetwork->dot11Ssid.uSSIDLength;
			memcpy(
				outputNetworkParameters[j].dot11Ssid.ssid, wlanAvailableNetwork->dot11Ssid.ucSSID,
				sizeof(uchar) * wlanAvailableNetwork->dot11Ssid.uSSIDLength);

			std::cout << outputNetworkParameters[j].dot11Ssid.ssid << '\n';

			outputNetworkParameters[j].isNetworkConnectable = wlanAvailableNetwork->bNetworkConnectable;
			outputNetworkParameters[j].isSecurityEnabled = wlanAvailableNetwork->bSecurityEnabled;

			outputNetworkParameters[j].networkProfileName = wlanAvailableNetwork->strProfileName;
			outputNetworkParameters[j].connectionQuality = (wlanAvailableNetwork->wlanSignalQuality == 0) 
				? -100 
				: (wlanAvailableNetwork->wlanSignalQuality == 100) 
					?  -50 
				:  -100 + (wlanAvailableNetwork->wlanSignalQuality / 2);

			outputNetworkParameters[j].defaultAuthenticationAlgorithm = AuthenticationAlgorithmFromWinApi(
				wlanAvailableNetwork->dot11DefaultAuthAlgorithm);

			outputNetworkParameters[j].defaultCipherAlgorithm = CipherAlgorithmFromWinApi(
				wlanAvailableNetwork->dot11DefaultCipherAlgorithm);

			outputNetworkParameters[j].isNetworkCurrentlyConnected =
				wlanAvailableNetwork->dwFlags && wlanAvailableNetwork->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED;
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

Dot11AuthenticationAlgorithm WindowsNetworkInformation::AuthenticationAlgorithmFromWinApi(DOT11_AUTH_ALGORITHM winApiAuthAlgorithm) noexcept {
	switch (winApiAuthAlgorithm) {
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_80211_OPEN:
			return Dot11AuthenticationAlgorithm::Ieee_802_11_AuthenticationAlgorithmOpen;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_80211_SHARED_KEY:
			return Dot11AuthenticationAlgorithm::Ieee_802_11_AuthenticationAlgorithmSharedKey;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA_PSK:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA_PSK;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA_NONE:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA_NONE;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_RSNA:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmRSNA;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_RSNA_PSK:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmRSNA_PSK;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA3:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA3;

		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA3_SAE:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA3_SAE;

#if (NTDDI_VERSION >= NTDDI_WIN10_VB)
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_OWE:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmOWE;
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_FE)
		case DOT11_AUTH_ALGORITHM::DOT11_AUTH_ALGO_WPA3_ENT:
			return Dot11AuthenticationAlgorithm::AuthenticationAlgorithmWPA3_ENT;
#endif
	}

	AssertUnreachable();
	return {};
}

Dot11CipherAlgorithm WindowsNetworkInformation::CipherAlgorithmFromWinApi(DOT11_CIPHER_ALGORITHM winApiCipherAlgorithm) noexcept {
	switch (winApiCipherAlgorithm) {
		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_NONE:
			return Dot11CipherAlgorithm::CipherAlgorithmNone;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WEP40:
			return Dot11CipherAlgorithm::CipherAlgorithmWEP40;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_TKIP:
			return Dot11CipherAlgorithm::CipherAlgorithmTKIP;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_CCMP:
			return Dot11CipherAlgorithm::CipherAlgorithmCCMP;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WEP104:
			return Dot11CipherAlgorithm::CipherAlgorithmWEP104;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP:
			return Dot11CipherAlgorithm::CipherAlgorithmBIP;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_GCMP:
			return Dot11CipherAlgorithm::CipherAlgorithmGCMP;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_GCMP_256:
			return Dot11CipherAlgorithm::CipherAlgorithmGCMP_256;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_CCMP_256:
			return Dot11CipherAlgorithm::CipherAlgorithmCCMP_256;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP_GMAC_128:
			return Dot11CipherAlgorithm::CipherAlgorithmBIP_GMAC_128;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP_GMAC_256:
			return Dot11CipherAlgorithm::CipherAlgorithmBIP_GMAC_256;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_BIP_CMAC_256:
			return Dot11CipherAlgorithm::CipherAlgorithmBIP_CMAC_256;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WPA_USE_GROUP:
			return Dot11CipherAlgorithm::CipherAlgorithmWPA_USE_GROUP;

		case DOT11_CIPHER_ALGORITHM::DOT11_CIPHER_ALGO_WEP:
			return Dot11CipherAlgorithm::CipherAlgorithmWEP;
	}

	AssertUnreachable();
	return {};
}

__BASE_NETWORK_NAMESPACE_END

#endif

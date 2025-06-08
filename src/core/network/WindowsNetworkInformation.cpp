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

void WindowsNetworkInformation::enumerateNetworks() noexcept {
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

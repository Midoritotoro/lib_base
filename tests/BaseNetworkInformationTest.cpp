#include <base/core/network/NetworkInformation.h>

int main(int argc, char** argv) {
	base::network::NetworksList parameters;
	base::network::NetworkInformation::enumerateNetworks(parameters);

	std::cout << "parameters.size(): " << parameters.size() << '\n';

	for (int i = 0; i < parameters.size(); ++i) {
		std::cout << parameters[i].dot11Ssid().ssidLength << '\n';

		std::cout << "parameters[i].connectionQuality(): " << parameters[i].connectionQuality() << '\n';
		std::cout << "parameters[i].haveProfile(): " << parameters[i].haveProfile() << '\n';
	
		if (parameters[i].haveProfile() && parameters[i].networkProfileName().size() != 0)
			std::wcout << L"parameters[i].profileName(): " << parameters[i].networkProfileName() << L'\n';

		std::cout << "parameters[i].ssidLength: " << parameters[i].dot11Ssid().ssidLength << '\n';

		std::cout << "parameters[i].isSecurityEnabled(): " << parameters[i].isSecurityEnabled() << '\n';
		std::cout << "parameters[i].isNetworkConnectable(): " << parameters[i].isNetworkConnectable() << '\n';

		std::cout << "parameters[i].isNetworkCurrentlyConnected(): " << parameters[i].isNetworkCurrentlyConnected() << '\n';



		std::wcout << "parameters[i].ssid(): ";
		for (auto k = 0; k < parameters[i].dot11Ssid().ssidLength; ++k) {
			std::cout << (int)parameters[i].dot11Ssid().ssid[k] << ' ';
			wprintf(L"%c", parameters[i].dot11Ssid().ssid[k]);
		}

		std::wstring wstr;
		wstr.reserve(parameters[i].dot11Ssid().ssidLength);
		
		base::string::StringConverter<>::convertString<uchar, wchar_t>(
			&parameters[i].dot11Ssid().ssid[0],
			parameters[i].dot11Ssid().ssidLength);
		std::cout << '\n';

	}
			
	return 0;
}
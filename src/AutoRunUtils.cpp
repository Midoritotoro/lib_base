#include "AutoRunUtils.h"

#ifdef _WIN32

bool IsWindowsGreaterThen(int version)
{
    auto osvi = OSVERSIONINFOEXW();

    const auto dwlConditionMask = VerSetConditionMask(
        VerSetConditionMask(
            VerSetConditionMask(
                0, VER_MAJORVERSION, VER_GREATER_EQUAL),
            VER_MINORVERSION, VER_GREATER_EQUAL),
        VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

    osvi = {
        .dwOSVersionInfoSize = sizeof(osvi),
        .dwMajorVersion = HIBYTE(version),
        .dwMinorVersion = LOBYTE(version),
        .wServicePackMajor = 0
    };

    return VerifyVersionInfoW(&osvi, 
        VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR,
        dwlConditionMask) != FALSE;
}

bool SetAutoRunKey(LPWSTR path, LPWSTR key)
{
    LPCWSTR lpSubKey = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    LONG result = 0;
    DWORD pResult = 0;
    HKEY hKey = NULL;

    if ((RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_READ, &hKey)) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return FALSE;
    }

    if ((RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, REG_OPTION_NON_VOLATILE, NULL, KEY_ALL_ACCESS, NULL, &hKey, &pResult)) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return FALSE;
    }
    result = RegSetValueEx(hKey, key, 0, REG_SZ, (PBYTE)(path), ((LPBYTE)(path), (lstrlen(path) * sizeof(TCHAR) + 1)));
    if (result != ERROR_SUCCESS){
        RegCloseKey(hKey);
        return FALSE;
    }
    RegCloseKey(hKey);
    return TRUE;
}

#endif // _WIN32

bool addToAutoRun(LPWSTR key)
{
#ifdef _WIN32
    TCHAR szExeName[MAX_PATH];
    TCHAR fileName[23] = L"\\TelegramQuickView.exe";

    rsize_t stringSize = MAX_PATH;
    GetModuleFileName(NULL, szExeName, stringSize);

    if (FAILED(PathCchRemoveFileSpec(szExeName, stringSize)))
        return FALSE;

    _tcscat_s(szExeName, stringSize, fileName);
    return SetAutoRunKey(szExeName, key);
#endif // _WIN32
    return FALSE;
}

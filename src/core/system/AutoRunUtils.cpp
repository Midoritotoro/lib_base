﻿#include <base/core/utility/Algorithm.h>

#if defined(OS_WIN)

#include <PathCch.h>
#include <tchar.h>

__BASE_NAMESPACE_BEGIN

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

bool SetAutoRunKey(
    const std::wstring& path, 
    const std::wstring& key)
{
    LPCWSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    LONG result = 0;
    DWORD pResult = 0;
    HKEY hKey = NULL;

    if ((RegOpenKeyExW(HKEY_CURRENT_USER, lpSubKey, 0, KEY_READ, &hKey)) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return FALSE;
    }

    if ((RegCreateKeyExW(HKEY_CURRENT_USER, lpSubKey, 0, REG_OPTION_NON_VOLATILE, NULL, KEY_ALL_ACCESS, NULL, &hKey, &pResult)) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return FALSE;
    }
    result = RegSetValueExW(hKey, &key[0], 0, REG_SZ, (PBYTE)(&path[0]), ((LPBYTE)(&path[0]), (lstrlenW(&path[0]) * sizeof(TCHAR) + 1)));
    if (result != ERROR_SUCCESS){
        RegCloseKey(hKey);
        return FALSE;
    }
    RegCloseKey(hKey);
    return TRUE;
}

__BASE_NAMESPACE_END

#endif // OS_WIN
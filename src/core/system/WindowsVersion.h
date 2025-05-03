#pragma once 

#include <base/core/arch/SystemDetection.h>



// These constants reflect _WIN32_WINNT_* macros from sdkddkver.h
// See also: http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745%28v=vs.85%29.aspx#setting_winver_or__win32_winnt

#define BASE_WINAPI_VERSION_NT4             0x0400  // Windows NT 4.0 
#define BASE_WINAPI_VERSION_WIN2K           0x0500  // Windows 2000
#define BASE_WINAPI_VERSION_WINXP           0x0501  // Windows XP
#define BASE_WINAPI_VERSION_WS03            0x0502  // Windows Server 2003
#define BASE_WINAPI_VERSION_WIN6            0x0600  // Windows Vista
#define BASE_WINAPI_VERSION_VISTA           0x0600  // Windows Vista
#define BASE_WINAPI_VERSION_WS08            0x0600  // Windows Server 2008
#define BASE_WINAPI_VERSION_LONGHORN        0x0600  // Windows Vista
#define BASE_WINAPI_VERSION_WIN7            0x0601  // Windows 7
#define BASE_WINAPI_VERSION_WIN8            0x0602  // Windows 8
#define BASE_WINAPI_VERSION_WINBLUE         0x0603  // Windows 8.1
#define BASE_WINAPI_VERSION_WINTHRESHOLD    0x0A00  // Windows 10 "Threeshold"
#define BASE_WINAPI_VERSION_WIN10           0x0A00  // Windows 10

// These constants reflect NTDDI_* macros from sdkddkver.h

#define BASE_WINAPI_NTDDI_WIN2K				0x05000000 // Windows 2000
#define BASE_WINAPI_NTDDI_WIN2KSP1			0x05000100 // Windows 2000 with service pack 1
#define BASE_WINAPI_NTDDI_WIN2KSP2			0x05000200 // Windows 2000 with service pack 2
#define BASE_WINAPI_NTDDI_WIN2KSP3			0x05000300 // Windows 2000 with service pack 3
#define BASE_WINAPI_NTDDI_WIN2KSP4			0x05000400 // Windows 2000 with service pack 4

#define BASE_WINAPI_NTDDI_WINXP				0x05010000 // Windows XP
#define BASE_WINAPI_NTDDI_WINXPSP1			0x05010100 // Windows XP with service pack 1
#define BASE_WINAPI_NTDDI_WINXPSP2			0x05010200 // Windows XP with service pack 2
#define BASE_WINAPI_NTDDI_WINXPSP3			0x05010300 // Windows XP with service pack 3
#define BASE_WINAPI_NTDDI_WINXPSP4			0x05010400 // Windows XP with service pack 4

#define BASE_WINAPI_NTDDI_WS03				0x05020000 // Windows Server 2003 
#define BASE_WINAPI_NTDDI_WS03SP1			0x05020100 // Windows Server 2003 with service pack 1
#define BASE_WINAPI_NTDDI_WS03SP2			0x05020200 // Windows Server 2003 with service pack 2
#define BASE_WINAPI_NTDDI_WS03SP3			0x05020300 // Windows Server 2003 with service pack 3
#define BASE_WINAPI_NTDDI_WS03SP4			0x05020400 // Windows Server 2003 with service pack 4

#define BASE_WINAPI_NTDDI_WIN6				0x06000000 // Windows Vista
#define BASE_WINAPI_NTDDI_WIN6SP1			0x06000100 // Windows Vista with service pack 1
#define BASE_WINAPI_NTDDI_WIN6SP2			0x06000200 // Windows Vista with service pack 2
#define BASE_WINAPI_NTDDI_WIN6SP3			0x06000300 // Windows Vista with service pack 3
#define BASE_WINAPI_NTDDI_WIN6SP4			0x06000400 // Windows Vista with service pack 4

#define BASE_WINAPI_NTDDI_VISTA				BASE_WINAPI_NTDDI_WIN6
#define BASE_WINAPI_NTDDI_VISTASP1			BASE_WINAPI_NTDDI_WIN6SP1
#define BASE_WINAPI_NTDDI_VISTASP2			BASE_WINAPI_NTDDI_WIN6SP2
#define BASE_WINAPI_NTDDI_VISTASP3			BASE_WINAPI_NTDDI_WIN6SP3
#define BASE_WINAPI_NTDDI_VISTASP4			BASE_WINAPI_NTDDI_WIN6SP4

#define BASE_WINAPI_NTDDI_LONGHORN			BASE_WINAPI_NTDDI_VISTA

#define BASE_WINAPI_NTDDI_WS08				BASE_WINAPI_NTDDI_WIN6SP1
#define BASE_WINAPI_NTDDI_WS08SP2			BASE_WINAPI_NTDDI_WIN6SP2
#define BASE_WINAPI_NTDDI_WS08SP3			BASE_WINAPI_NTDDI_WIN6SP3
#define BASE_WINAPI_NTDDI_WS08SP4			BASE_WINAPI_NTDDI_WIN6SP4

#define BASE_WINAPI_NTDDI_WIN7				0x06010000 // Windows 7
#define BASE_WINAPI_NTDDI_WIN7SP1			0x06010100 // Not defined in Windows SDK - Windows 7 with service pack 1
#define BASE_WINAPI_NTDDI_WIN8				0x06020000 // Windows 8
#define BASE_WINAPI_NTDDI_WINBLUE			0x06030000 // Windows 8.1
#define BASE_WINAPI_NTDDI_WINTHRESHOLD		0x0A000000 // Windows 10 "Threeshold"
#define BASE_WINAPI_NTDDI_WIN10				0x0A000000 // Windows 10
#define BASE_WINAPI_NTDDI_WIN10_TH2			0x0A000001 // Windows 10 "Threeshold 2"
#define BASE_WINAPI_NTDDI_WIN10_RS1			0x0A000002 // Windows 10 1607 "Redstone 1"
#define BASE_WINAPI_NTDDI_WIN10_RS2			0x0A000003 // Windows 10 1703 "Redstone 2"
#define BASE_WINAPI_NTDDI_WIN10_RS3			0x0A000004 // Windows 10 1709 "Redstone 3"
#define BASE_WINAPI_NTDDI_WIN10_RS4			0x0A000005 // Windows 10 1803 "Redstone 4"
#define BASE_WINAPI_NTDDI_WIN10_RS5			0x0A000006 // Windows 10 1809 "Redstone 5"

#define BASE_WINAPI_DETAIL_MAKE_NTDDI_VERSION2(x)   x##0000
#define BASE_WINAPI_DETAIL_MAKE_NTDDI_VERSION(x)    BASE_WINAPI_DETAIL_MAKE_NTDDI_VERSION2(x)

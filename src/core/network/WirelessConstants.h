#pragma once 

#include <base/core/arch/Platform.h>

#if defined(base_os_windows)
#  include <wlantypes.h>
#else

#endif

__BASE_NETWORK_NAMESPACE_BEGIN

enum Dot11AuthenticationAlgorithm {
    Ieee_802_11_AuthenticationAlgorithmOpen = 1,
    Ieee_802_11_AuthenticationAlgorithmSharedKey = 2,
    AuthenticationAlgorithmWPA = 3,
    AuthenticationAlgorithmWPA_PSK = 4,
    AuthenticationAlgorithmWPA_NONE = 5,
    AuthenticationAlgorithmRSNA = 6,
    AuthenticationAlgorithmRSNA_PSK = 7,
    AuthenticationAlgorithmWPA3 = 8,               // means WPA3 Enterprise 192 bits
#if defined(base_os_windows)
#if (NTDDI_VERSION >= NTDDI_WIN10_FE)
    AuthenticationAlgorithmWPA3_ENT_192 = AuthenticationAlgorithmWPA3,
#endif
#endif
    AuthenticationAlgorithmWPA3_SAE = 9,
#if defined(base_os_windows)
#if (NTDDI_VERSION >= NTDDI_WIN10_VB)
    AuthenticationAlgorithmOWE = 10,
#endif
#endif
#if defined(base_os_windows)
#if (NTDDI_VERSION >= NTDDI_WIN10_FE)
    AuthenticationAlgorithmWPA3_ENT = 11,
#endif
#endif
    AuthenticationAlgorithmIHV_START = 0x80000000,
    AuthenticationAlgorithmIHV_END = 0xffffffff
};

enum Dot11CipherAlgorithm {
    CipherAlgorithmNone = 0x00,
    CipherAlgorithmWEP40 = 0x01,
    CipherAlgorithmTKIP = 0x02,
    CipherAlgorithmCCMP = 0x04,
    CipherAlgorithmWEP104 = 0x05,
    CipherAlgorithmBIP = 0x06,                       // BIP-CMAC-128
    CipherAlgorithmGCMP = 0x08,                      // GCMP-128
    CipherAlgorithmGCMP_256 = 0x09,                  // GCMP-256
    CipherAlgorithmCCMP_256 = 0x0a,                  // CCMP-256
    CipherAlgorithmBIP_GMAC_128 = 0x0b,              // BIP-GMAC-128
    CipherAlgorithmBIP_GMAC_256 = 0x0c,              // BIP-GMAC-256
    CipherAlgorithmBIP_CMAC_256 = 0x0d,              // BIP-CMAC-256
    CipherAlgorithmWPA_USE_GROUP = 0x100,
    CipherAlgorithmRSN_USE_GROUP = 0x100,
    CipherAlgorithmWEP = 0x101,
    CipherAlgorithmIHV_START = 0x80000000,
    CipherAlgorithmIHV_END = 0xffffffff
};

#if defined(base_os_windows)
    using WinApiDot11CipherAlgorithm_t = DOT11_CIPHER_ALGORITHM;
    using WinApiDot11AuthAlgorithm_t = DOT11_AUTH_ALGORITHM;
#endif

__BASE_NETWORK_NAMESPACE_END

#include <base/core/arch/ProcessorFeatures.h>

__BASE_NAMESPACE_BEGIN

std::string ProcessorFeatures::Vendor() noexcept {
    return CPU_Rep._vendor;
}

std::string ProcessorFeatures::Brand() noexcept {
    return CPU_Rep.brand_;
}

bool ProcessorFeatures::SSE3() noexcept {
    return CPU_Rep.f_1_ECX_[0];
}

bool ProcessorFeatures::PCLMULQDQ() noexcept {
    return CPU_Rep.f_1_ECX_[1];
}

bool ProcessorFeatures::MONITOR() noexcept {
    return CPU_Rep.f_1_ECX_[3];
}

bool ProcessorFeatures::SSSE3() noexcept {
    return CPU_Rep.f_1_ECX_[9];
}

bool ProcessorFeatures::FMA() noexcept {
    return CPU_Rep.f_1_ECX_[12];
}

bool ProcessorFeatures::CMPXCHG16B() noexcept {
    return CPU_Rep.f_1_ECX_[13];
}

bool ProcessorFeatures::SSE41() noexcept {
    return CPU_Rep.f_1_ECX_[19];
}

bool ProcessorFeatures::SSE42() noexcept {
    return CPU_Rep.f_1_ECX_[20];
}

bool ProcessorFeatures::MOVBE() noexcept {
    return CPU_Rep.f_1_ECX_[22];
}

bool ProcessorFeatures::POPCNT() noexcept {
    return CPU_Rep.f_1_ECX_[23];
}

bool ProcessorFeatures::AES() noexcept {
    return CPU_Rep.f_1_ECX_[25];
}

bool ProcessorFeatures::XSAVE() noexcept {
    return CPU_Rep.f_1_ECX_[26];
}

bool ProcessorFeatures::OSXSAVE() noexcept {
    return CPU_Rep.f_1_ECX_[27];
}

bool ProcessorFeatures::AVX() noexcept {
    return CPU_Rep.f_1_ECX_[28];
}

bool ProcessorFeatures::F16C() noexcept {
    return CPU_Rep.f_1_ECX_[29];
}

bool ProcessorFeatures::RDRAND() noexcept {
    return CPU_Rep.f_1_ECX_[30];
}

bool ProcessorFeatures::MSR() noexcept {
    return CPU_Rep.f_1_EDX_[5];
}

bool ProcessorFeatures::CX8() noexcept {
    return CPU_Rep.f_1_EDX_[8];
}

bool ProcessorFeatures::SEP() noexcept {
    return CPU_Rep.f_1_EDX_[11];
}

bool ProcessorFeatures::CMOV() noexcept {
    return CPU_Rep.f_1_EDX_[15];
}

bool ProcessorFeatures::CLFSH() noexcept {
    return CPU_Rep.f_1_EDX_[19];
}

bool ProcessorFeatures::MMX() noexcept {
    return CPU_Rep.f_1_EDX_[23];
}

bool ProcessorFeatures::FXSR() noexcept {
    return CPU_Rep.f_1_EDX_[24];
}

bool ProcessorFeatures::SSE() noexcept {
    return CPU_Rep.f_1_EDX_[25];
}

bool ProcessorFeatures::SSE2() noexcept {
    return CPU_Rep.f_1_EDX_[26];
}

bool ProcessorFeatures::FSGSBASE() noexcept {
    return CPU_Rep.f_7_EBX_[0];
}

bool ProcessorFeatures::BMI1() noexcept {
    return CPU_Rep.f_7_EBX_[3];
}

bool ProcessorFeatures::HLE() noexcept {
    return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4];
}

bool ProcessorFeatures::AVX2() noexcept {
    return CPU_Rep.f_7_EBX_[5];
}

bool ProcessorFeatures::BMI2() noexcept {
    return CPU_Rep.f_7_EBX_[8];
}

bool ProcessorFeatures::ERMS() noexcept {
    return CPU_Rep.f_7_EBX_[9];
}

bool ProcessorFeatures::INVPCID() noexcept {
    return CPU_Rep.f_7_EBX_[10];
}

bool ProcessorFeatures::RTM() noexcept {
    return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11];
}

bool ProcessorFeatures::AVX512F() noexcept {
    return CPU_Rep.f_7_EBX_[16];
}

bool ProcessorFeatures::RDSEED() noexcept {
    return CPU_Rep.f_7_EBX_[18];
}

bool ProcessorFeatures::ADX() noexcept {
    return CPU_Rep.f_7_EBX_[19];
}

bool ProcessorFeatures::AVX512PF() noexcept {
    return CPU_Rep.f_7_EBX_[26];
}

bool ProcessorFeatures::AVX512ER() noexcept {
    return CPU_Rep.f_7_EBX_[27];
}

bool ProcessorFeatures::AVX512CD() noexcept {
    return CPU_Rep.f_7_EBX_[28];
}

bool ProcessorFeatures::SHA() noexcept {
    return CPU_Rep.f_7_EBX_[29];
}

bool ProcessorFeatures::PREFETCHWT1() noexcept {
    return CPU_Rep.f_7_ECX_[0];
}

bool ProcessorFeatures::LAHF() noexcept {
    return CPU_Rep.f_81_ECX_[0];
}

bool ProcessorFeatures::LZCNT() noexcept {
    return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5];
}

bool ProcessorFeatures::ABM() noexcept {
    return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5];
}

bool ProcessorFeatures::SSE4a() noexcept {
    return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6];
}

bool ProcessorFeatures::XOP() noexcept {
    return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11];
}

bool ProcessorFeatures::TBM() noexcept {
    return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21];
}

bool ProcessorFeatures::SYSCALL() noexcept {
    return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11];
}

bool ProcessorFeatures::MMXEXT() noexcept {
    return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22];
}

bool ProcessorFeatures::RDTSCP() noexcept {
    return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27];
}

bool ProcessorFeatures::_3DNOWEXT() noexcept {
    return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30];
}

bool ProcessorFeatures::_3DNOW() noexcept {
    return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31];
}

ProcessorFeatures::ProcessorFeaturesInternal::ProcessorFeaturesInternal()
{
    std::array<int, 4> cpui;

    // Calling __cpuid with 0x0 as the function_id argument
    // gets the number of the highest valid function ID.

    CpuId(cpui.data(), 0);
    nIds_ = cpui[0];

    for (int i = 0; i <= nIds_; ++i) {
        CpuIdExtended(cpui.data(), i, 0);
        data_.push_back(cpui);
    }

    // Capture vendor string
    char vendor[32];
    memset(vendor, 0, sizeof(vendor));

    *reinterpret_cast<int*>(vendor)     = data_[0][1];
    *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
    *reinterpret_cast<int*>(vendor + 8) = data_[0][2];

    _vendor = vendor;

    if (_vendor == "GenuineIntel")
        isIntel_ = true;
    else if (_vendor == "AuthenticAMD")
        isAMD_ = true;

    // load bitset with flags for function 0x00000001
    if (nIds_ >= 1) {
        f_1_ECX_ = data_[1][2];
        f_1_EDX_ = data_[1][3];
    }

    // load bitset with flags for function 0x00000007
    if (nIds_ >= 7) {
        f_7_EBX_ = data_[7][1];
        f_7_ECX_ = data_[7][2];
    }

    // Calling __cpuid with 0x80000000 as the function_id argument
    // gets the number of the highest valid extended ID.

    CpuId(cpui.data(), 0x80000000);
    nExIds_ = cpui[0];

    char brand[0x40];
    memset(brand, 0, sizeof(brand));

    for (int i = 0x80000000; i <= nExIds_; ++i) {
        CpuIdExtended(cpui.data(), i, 0);
        extdata_.push_back(cpui);
    }

    // load bitset with flags for function 0x80000001
    if (nExIds_ >= 0x80000001) {
        f_81_ECX_ = extdata_[1][2];
        f_81_EDX_ = extdata_[1][3];
    }

    // Interpret CPU brand string if reported
    if (nExIds_ >= 0x80000004) {
        memcpy(brand, extdata_[2].data(), sizeof(cpui));
        memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
        memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
        brand_ = brand;
    }
};

__BASE_NAMESPACE_END
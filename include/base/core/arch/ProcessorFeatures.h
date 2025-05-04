#pragma once

#include <bitset>
#include <vector>

#include <base/core/arch/CpuId.h>

__BASE_NAMESPACE_BEGIN

class ProcessorFeatures
{
    class ProcessorFeaturesInternal;
public:
    static NODISCARD std::string Vendor() noexcept { 
        return CPU_Rep._vendor;
    }

    static NODISCARD std::string Brand() noexcept {
        return CPU_Rep.brand_;
    }

    static NODISCARD bool SSE3() noexcept {
        return CPU_Rep.f_1_ECX_[0];
    }

    static NODISCARD bool PCLMULQDQ() noexcept {
        return CPU_Rep.f_1_ECX_[1]; 
    }

    static NODISCARD bool MONITOR() noexcept {
        return CPU_Rep.f_1_ECX_[3];
    }

    static NODISCARD bool SSSE3() noexcept {
        return CPU_Rep.f_1_ECX_[9];
    }

    static NODISCARD bool FMA() noexcept {
        return CPU_Rep.f_1_ECX_[12]; 
    }

    static NODISCARD bool CMPXCHG16B() noexcept {
        return CPU_Rep.f_1_ECX_[13]; 
    }

    static NODISCARD bool SSE41() noexcept {
        return CPU_Rep.f_1_ECX_[19]; 
    }

    static NODISCARD bool SSE42() noexcept {
        return CPU_Rep.f_1_ECX_[20];
    }

    static NODISCARD bool MOVBE() noexcept { 
        return CPU_Rep.f_1_ECX_[22]; 
    }

    static NODISCARD bool POPCNT() noexcept { 
        return CPU_Rep.f_1_ECX_[23];
    }

    static NODISCARD bool AES() noexcept { 
        return CPU_Rep.f_1_ECX_[25];
    }

    static NODISCARD bool XSAVE() noexcept { 
        return CPU_Rep.f_1_ECX_[26];
    }

    static NODISCARD bool OSXSAVE() noexcept { 
        return CPU_Rep.f_1_ECX_[27]; 
    }

    static NODISCARD bool AVX() noexcept { 
        return CPU_Rep.f_1_ECX_[28]; 
    }

    static NODISCARD bool F16C() noexcept { 
        return CPU_Rep.f_1_ECX_[29];
    }

    static NODISCARD bool RDRAND() noexcept { 
        return CPU_Rep.f_1_ECX_[30];
    }

    static NODISCARD bool MSR() noexcept { 
        return CPU_Rep.f_1_EDX_[5];
    }

    static NODISCARD bool CX8() noexcept { 
        return CPU_Rep.f_1_EDX_[8];
    }

    static NODISCARD bool SEP() noexcept { 
        return CPU_Rep.f_1_EDX_[11];
    }

    static NODISCARD bool CMOV() noexcept { 
        return CPU_Rep.f_1_EDX_[15];
    }

    static NODISCARD bool CLFSH() noexcept { 
        return CPU_Rep.f_1_EDX_[19];
    }

    static NODISCARD bool MMX() noexcept { 
        return CPU_Rep.f_1_EDX_[23]; 
    }

    static NODISCARD bool FXSR() noexcept { 
        return CPU_Rep.f_1_EDX_[24];
    }

    static NODISCARD bool SSE() noexcept { 
        return CPU_Rep.f_1_EDX_[25];
    }

    static NODISCARD bool SSE2() noexcept {
        return CPU_Rep.f_1_EDX_[26]; 
    }

    static NODISCARD bool FSGSBASE() noexcept { 
        return CPU_Rep.f_7_EBX_[0]; 
    }

    static NODISCARD bool BMI1() noexcept {
        return CPU_Rep.f_7_EBX_[3]; 
    }

    static NODISCARD bool HLE() noexcept { 
        return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4];
    }

    static NODISCARD bool AVX2() noexcept { 
        return CPU_Rep.f_7_EBX_[5];
    }

    static NODISCARD bool BMI2() noexcept { 
        return CPU_Rep.f_7_EBX_[8]; 
    }

    static NODISCARD bool ERMS() noexcept { 
        return CPU_Rep.f_7_EBX_[9];
    }

    static NODISCARD bool INVPCID() noexcept { 
        return CPU_Rep.f_7_EBX_[10]; 
    }

    static NODISCARD bool RTM() noexcept {
        return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; 
    }

    static NODISCARD bool AVX512F() noexcept {
        return CPU_Rep.f_7_EBX_[16];
    }

    static NODISCARD bool RDSEED() noexcept { 
        return CPU_Rep.f_7_EBX_[18]; 
    }

    static NODISCARD bool ADX() noexcept {
        return CPU_Rep.f_7_EBX_[19]; 
    }

    static NODISCARD bool AVX512PF() noexcept {
        return CPU_Rep.f_7_EBX_[26]; 
    }

    static NODISCARD bool AVX512ER() noexcept { 
        return CPU_Rep.f_7_EBX_[27];
    }

    static NODISCARD bool AVX512CD() noexcept { 
        return CPU_Rep.f_7_EBX_[28]; 
    }

    static NODISCARD bool SHA() noexcept { 
        return CPU_Rep.f_7_EBX_[29]; 
    }

    static NODISCARD bool PREFETCHWT1() noexcept {
        return CPU_Rep.f_7_ECX_[0]; 
    }

    static NODISCARD bool LAHF() noexcept {
        return CPU_Rep.f_81_ECX_[0]; 
    }

    static NODISCARD bool LZCNT() noexcept { 
        return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5];
    }

    static NODISCARD bool ABM() noexcept { 
        return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; 
    }

    static NODISCARD bool SSE4a() noexcept { 
        return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6];
    }

    static NODISCARD bool XOP() noexcept { 
        return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; 
    }

    static NODISCARD bool TBM() noexcept { 
        return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21];
    }

    static NODISCARD bool SYSCALL() noexcept { 
        return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; 
    }

    static NODISCARD bool MMXEXT() noexcept {
        return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22];
    }

    static NODISCARD bool RDTSCP() noexcept {
        return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; 
    }

    static NODISCARD bool _3DNOWEXT() noexcept { 
        return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; 
    }

    static NODISCARD bool _3DNOW() noexcept {
        return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; 
    }
private:
    class ProcessorFeaturesInternal
    {
    public:
        ProcessorFeaturesInternal()
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
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));

            *reinterpret_cast<int*>(vendor) = data_[0][1];
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

        int nIds_ = 0;
        int nExIds_ = 0;

        std::string _vendor;
        std::string brand_;

        bool isIntel_  = false;
        bool isAMD_ = false;

        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;

        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;

        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;

        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
    };

    inline static ProcessorFeaturesInternal CPU_Rep = {};
};

__BASE_NAMESPACE_END

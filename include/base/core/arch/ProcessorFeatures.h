#pragma once

#include <bitset>
#include <vector>

#include <array>
#include <ranges>

#include <base/core/arch/CpuId.h>

__BASE_ARCH_NAMESPACE_BEGIN


class ProcessorFeatures
{
    class ProcessorFeaturesInternal;
public:
    static base_nodiscard std::string Vendor() noexcept;
    static base_nodiscard std::string Brand() noexcept;

    static base_nodiscard bool MMX() noexcept;
    static base_nodiscard bool SSE() noexcept;
    static base_nodiscard bool SSE2() noexcept;
    static base_nodiscard bool SSE3() noexcept;
    static base_nodiscard bool SSSE3() noexcept;
    static base_nodiscard bool SSE41() noexcept;
    static base_nodiscard bool SSE42() noexcept;
    static base_nodiscard bool AVX() noexcept;
    static base_nodiscard bool AVX2() noexcept;
    static base_nodiscard bool AVX512F() noexcept;
    static base_nodiscard bool AVX512BW() noexcept;
    static base_nodiscard bool AVX512PF() noexcept;
    static base_nodiscard bool AVX512ER() noexcept;
    static base_nodiscard bool AVX512CD() noexcept;
    static base_nodiscard bool AVX512VL() noexcept;
    static base_nodiscard bool F16C() noexcept;
    static base_nodiscard bool FMA() noexcept;
    static base_nodiscard bool ADX() noexcept;
    static base_nodiscard bool SSE4a() noexcept;

    static base_nodiscard bool PCLMULQDQ() noexcept;
    static base_nodiscard bool MONITOR() noexcept;
    static base_nodiscard bool CMPXCHG16B() noexcept;
    static base_nodiscard bool MOVBE() noexcept;
    static base_nodiscard bool POPCNT() noexcept;
    static base_nodiscard bool AES() noexcept;
    static base_nodiscard bool XSAVE() noexcept;
    static base_nodiscard bool OSXSAVE() noexcept;
    static base_nodiscard bool RDRAND() noexcept;
    static base_nodiscard bool MSR() noexcept;
    static base_nodiscard bool CX8() noexcept;
    static base_nodiscard bool SEP() noexcept;
    static base_nodiscard bool CMOV() noexcept;
    static base_nodiscard bool CLFSH() noexcept;
    static base_nodiscard bool FXSR() noexcept;
    static base_nodiscard bool FSGSBASE() noexcept;
    static base_nodiscard bool BMI1() noexcept;
    static base_nodiscard bool HLE() noexcept;
    static base_nodiscard bool BMI2() noexcept;
    static base_nodiscard bool ERMS() noexcept;
    static base_nodiscard bool INVPCID() noexcept;
    static base_nodiscard bool RTM() noexcept;
    static base_nodiscard bool RDSEED() noexcept;

    static base_nodiscard bool SHA() noexcept;
    static base_nodiscard bool PREFETCHWT1() noexcept;
    static base_nodiscard bool LAHF() noexcept;
    static base_nodiscard bool LZCNT() noexcept;
    static base_nodiscard bool ABM() noexcept;
    static base_nodiscard bool XOP() noexcept;
    static base_nodiscard bool TBM() noexcept;
    static base_nodiscard bool SYSCALL() noexcept;
    static base_nodiscard bool MMXEXT() noexcept;
    static base_nodiscard bool RDTSCP() noexcept;
    static base_nodiscard bool _3DNOWEXT() noexcept;
    static base_nodiscard bool _3DNOW() noexcept;
private:
    class ProcessorFeaturesInternal
    {
    public:
        ProcessorFeaturesInternal();

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


__BASE_ARCH_NAMESPACE_END


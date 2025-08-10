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
    static   std::string Vendor() noexcept;
    static   std::string Brand() noexcept;

    static   bool MMX() noexcept;
    static   bool SSE() noexcept;
    static   bool SSE2() noexcept;
    static   bool SSE3() noexcept;
    static   bool SSSE3() noexcept;
    static   bool SSE41() noexcept;
    static   bool SSE42() noexcept;
    static   bool AVX() noexcept;
    static   bool AVX2() noexcept;
    static   bool AVX512F() noexcept;
    static   bool AVX512BW() noexcept;
    static   bool AVX512PF() noexcept;
    static   bool AVX512ER() noexcept;
    static   bool AVX512CD() noexcept;
    static   bool AVX512VL() noexcept;
    static   bool F16C() noexcept;
    static   bool FMA() noexcept;
    static   bool ADX() noexcept;
    static   bool SSE4a() noexcept;

    static   bool PCLMULQDQ() noexcept;
    static   bool MONITOR() noexcept;
    static   bool CMPXCHG16B() noexcept;
    static   bool MOVBE() noexcept;
    static   bool POPCNT() noexcept;
    static   bool AES() noexcept;
    static   bool XSAVE() noexcept;
    static   bool OSXSAVE() noexcept;
    static   bool RDRAND() noexcept;
    static   bool MSR() noexcept;
    static   bool CX8() noexcept;
    static   bool SEP() noexcept;
    static   bool CMOV() noexcept;
    static   bool CLFSH() noexcept;
    static   bool FXSR() noexcept;
    static   bool FSGSBASE() noexcept;
    static   bool BMI1() noexcept;
    static   bool HLE() noexcept;
    static   bool BMI2() noexcept;
    static   bool ERMS() noexcept;
    static   bool INVPCID() noexcept;
    static   bool RTM() noexcept;
    static   bool RDSEED() noexcept;

    static   bool SHA() noexcept;
    static   bool PREFETCHWT1() noexcept;
    static   bool LAHF() noexcept;
    static   bool LZCNT() noexcept;
    static   bool ABM() noexcept;
    static   bool XOP() noexcept;
    static   bool TBM() noexcept;
    static   bool SYSCALL() noexcept;
    static   bool MMXEXT() noexcept;
    static   bool RDTSCP() noexcept;
    static   bool _3DNOWEXT() noexcept;
    static   bool _3DNOW() noexcept;
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


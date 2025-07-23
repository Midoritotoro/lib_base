#pragma once

#include <bitset>
#include <vector>

#include <array>

#include <base/core/arch/CpuId.h>

__BASE_NAMESPACE_BEGIN

class ProcessorFeatures
{
    class ProcessorFeaturesInternal;
public:
    template <typename FunctionType, CpuFeature Feature, typename... Args>
    using FeatureAwareFunction = std::conditional_t<true, FunctionType, void>;

    template <typename FunctionType, typename... Args,
              typename = std::enable_if_t<std::is_invocable_v<FunctionType, Args...>>>
    static auto dispatch(FunctionType&& func, Args&&... args) {
        using ResultType = decltype(std::forward<FunctionType>(func)(std::forward<Args>(args)...));

        if (AVX512F()) {
            return dispatchImpl<FunctionType, CpuFeature::AVX512F>(std::forward<FunctionType>(func), std::forward<Args>(args)...);
        } else if (AVX()) {
            return dispatchImpl<FunctionType, CpuFeature::AVX>(std::forward<FunctionType>(func), std::forward<Args>(args)...);
        } else if (SSE2()) {
            return dispatchImpl<FunctionType, CpuFeature::SSE2>(std::forward<FunctionType>(func), std::forward<Args>(args)...);
        } else {
            return dispatchImpl<FunctionType, CpuFeature::None>(std::forward<FunctionType>(func), std::forward<Args>(args)...);
        }
    }

private:

    template <typename FunctionType, CpuFeature Feature, typename... Args,
              typename = std::enable_if_t<std::is_invocable_v<FunctionType, Args...>>>
    static auto dispatchImpl(FunctionType&& func, Args&&... args) -> decltype(std::forward<FunctionType>(func)(std::forward<Args>(args)...))
    {
        // This cast is crucial and relies on type erasure to avoid a static_assert
        return (func)(std::forward<Args>(args)...);
    }
public:

    static NODISCARD std::string Vendor() noexcept;
    static NODISCARD std::string Brand() noexcept;

    static NODISCARD bool MMX() noexcept;
    static NODISCARD bool SSE() noexcept;
    static NODISCARD bool SSE2() noexcept;
    static NODISCARD bool SSE3() noexcept;
    static NODISCARD bool SSSE3() noexcept;
    static NODISCARD bool SSE41() noexcept;
    static NODISCARD bool SSE42() noexcept;
    static NODISCARD bool AVX() noexcept;
    static NODISCARD bool AVX2() noexcept;
    static NODISCARD bool AVX512F() noexcept;
    static NODISCARD bool AVX512PF() noexcept;
    static NODISCARD bool AVX512ER() noexcept;
    static NODISCARD bool AVX512CD() noexcept;
    static NODISCARD bool F16C() noexcept;
    static NODISCARD bool FMA() noexcept;
    static NODISCARD bool ADX() noexcept;
    static NODISCARD bool SSE4a() noexcept;

    static NODISCARD bool PCLMULQDQ() noexcept;
    static NODISCARD bool MONITOR() noexcept;
    static NODISCARD bool CMPXCHG16B() noexcept;
    static NODISCARD bool MOVBE() noexcept;
    static NODISCARD bool POPCNT() noexcept;
    static NODISCARD bool AES() noexcept;
    static NODISCARD bool XSAVE() noexcept;
    static NODISCARD bool OSXSAVE() noexcept;
    static NODISCARD bool RDRAND() noexcept;
    static NODISCARD bool MSR() noexcept;
    static NODISCARD bool CX8() noexcept;
    static NODISCARD bool SEP() noexcept;
    static NODISCARD bool CMOV() noexcept;
    static NODISCARD bool CLFSH() noexcept;
    static NODISCARD bool FXSR() noexcept;
    static NODISCARD bool FSGSBASE() noexcept;
    static NODISCARD bool BMI1() noexcept;
    static NODISCARD bool HLE() noexcept;
    static NODISCARD bool BMI2() noexcept;
    static NODISCARD bool ERMS() noexcept;
    static NODISCARD bool INVPCID() noexcept;
    static NODISCARD bool RTM() noexcept;
    static NODISCARD bool RDSEED() noexcept;

    static NODISCARD bool SHA() noexcept;
    static NODISCARD bool PREFETCHWT1() noexcept;
    static NODISCARD bool LAHF() noexcept;
    static NODISCARD bool LZCNT() noexcept;
    static NODISCARD bool ABM() noexcept;
    static NODISCARD bool XOP() noexcept;
    static NODISCARD bool TBM() noexcept;
    static NODISCARD bool SYSCALL() noexcept;
    static NODISCARD bool MMXEXT() noexcept;
    static NODISCARD bool RDTSCP() noexcept;
    static NODISCARD bool _3DNOWEXT() noexcept;
    static NODISCARD bool _3DNOW() noexcept;
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

__BASE_NAMESPACE_END

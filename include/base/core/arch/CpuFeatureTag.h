#pragma once 

#include <base/core/arch/CpuFeature.h>

__BASE_NAMESPACE_BEGIN

template <CpuFeature Feature>
struct CpuFeatureTag {
    inline static constexpr CpuFeature value = Feature;
};

template <>
struct CpuFeatureTag<CpuFeature::None> {
    inline static constexpr CpuFeature value = CpuFeature::None;
};

template <>
struct CpuFeatureTag<CpuFeature::MMX> {
    inline static constexpr CpuFeature value = CpuFeature::MMX;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE> {
    inline static constexpr CpuFeature value = CpuFeature::SSE;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE2> {
    inline static constexpr CpuFeature value = CpuFeature::SSE2;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE3> {
    inline static constexpr CpuFeature value = CpuFeature::SSE3;
};

template <>
struct CpuFeatureTag<CpuFeature::SSSE3> {
    inline static constexpr CpuFeature value = CpuFeature::SSSE3;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE41> {
    inline static constexpr CpuFeature value = CpuFeature::SSE41;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE42> {
    inline static constexpr CpuFeature value = CpuFeature::SSE42;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX> {
    inline static constexpr CpuFeature value = CpuFeature::AVX;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX2> {
    inline static constexpr CpuFeature value = CpuFeature::AVX2;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512F> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512F;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512BW> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512BW;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512CD> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512CD;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512DQ> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512DQ;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512ER> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512ER;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512IFMA52> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512IFMA52;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512PF> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512PF;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512VL> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512VL;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512VPOPCNTDQ> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512VPOPCNTDQ;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_4FMAPS> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_4FMAPS;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_4VNNIW> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_4VNNIW;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_BF16> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_BF16;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_BITALG> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_BITALG;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VBMI> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_VBMI;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VBMI2> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_VBMI2;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VNNI> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_VNNI;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VP2INTERSECT> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_VP2INTERSECT;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_FP16> {
    inline static constexpr CpuFeature value = CpuFeature::AVX512_FP16;
};

template <>
struct CpuFeatureTag<CpuFeature::FMA> {
    inline static constexpr CpuFeature value = CpuFeature::FMA;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX_VNNI> {
    inline static constexpr CpuFeature value = CpuFeature::AVX_VNNI;
};

template <>
struct CpuFeatureTag<CpuFeature::KNC> {
    inline static constexpr CpuFeature value = CpuFeature::KNC;
};

template <>
struct CpuFeatureTag<CpuFeature::AMX> {
    inline static constexpr CpuFeature value = CpuFeature::AMX;
};

template <>
struct CpuFeatureTag<CpuFeature::AMXBF16> {
    inline static constexpr CpuFeature value = CpuFeature::AMXBF16;
};

template <>
struct CpuFeatureTag<CpuFeature::AMXINT8> {
    inline static constexpr CpuFeature value = CpuFeature::AMXINT8;
};

template <>
struct CpuFeatureTag<CpuFeature::AMXTILE> {
    inline static constexpr CpuFeature value = CpuFeature::AMXTILE;
};

template <>
struct CpuFeatureTag<CpuFeature::SVML> {
    inline static constexpr CpuFeature value = CpuFeature::SVML;
};

template <>
struct CpuFeatureTag<CpuFeature::ADX> {
    inline static constexpr CpuFeature value = CpuFeature::ADX;
};

template <>
struct CpuFeatureTag<CpuFeature::AES> {
    inline static constexpr CpuFeature value = CpuFeature::AES;
};

template <>
struct CpuFeatureTag<CpuFeature::BMI1> {
    inline static constexpr CpuFeature value = CpuFeature::BMI1;
};

template <>
struct CpuFeatureTag<CpuFeature::BMI2> {
    inline static constexpr CpuFeature value = CpuFeature::BMI2;
};

template <>
struct CpuFeatureTag<CpuFeature::CET_SS> {
    inline static constexpr CpuFeature value = CpuFeature::CET_SS;
};

template <>
struct CpuFeatureTag<CpuFeature::CLDEMOTE> {
    inline static constexpr CpuFeature value = CpuFeature::CLDEMOTE;
};

template <>
struct CpuFeatureTag<CpuFeature::CLFLUSHOPT> {
    inline static constexpr CpuFeature value = CpuFeature::CLFLUSHOPT;
};

template <>
struct CpuFeatureTag<CpuFeature::CLWB> {
    inline static constexpr CpuFeature value = CpuFeature::CLWB;
};

template <>
struct CpuFeatureTag<CpuFeature::ENQCMD> {
    inline static constexpr CpuFeature value = CpuFeature::ENQCMD;
};

template <>
struct CpuFeatureTag<CpuFeature::FP16C> {
    inline static constexpr CpuFeature value = CpuFeature::FP16C;
};

template <>
struct CpuFeatureTag<CpuFeature::FSGSBASE> {
    inline static constexpr CpuFeature value = CpuFeature::FSGSBASE;
};

template <>
struct CpuFeatureTag<CpuFeature::FXSR> {
    inline static constexpr CpuFeature value = CpuFeature::FXSR;
};

template <>
struct CpuFeatureTag<CpuFeature::GFNI> {
    inline static constexpr CpuFeature value = CpuFeature::GFNI;
};

template <>
struct CpuFeatureTag<CpuFeature::HRESET> {
    inline static constexpr CpuFeature value = CpuFeature::HRESET;
};

template <>
struct CpuFeatureTag<CpuFeature::INVPCID> {
    inline static constexpr CpuFeature value = CpuFeature::INVPCID;
};

template <>
struct CpuFeatureTag<CpuFeature::KEYLOCKER> {
    inline static constexpr CpuFeature value = CpuFeature::KEYLOCKER;
};

template <>
struct CpuFeatureTag<CpuFeature::KEYLOCKER_WIDE> {
    inline static constexpr CpuFeature value = CpuFeature::KEYLOCKER_WIDE;
};

template <>
struct CpuFeatureTag<CpuFeature::LZCNT> {
    inline static constexpr CpuFeature value = CpuFeature::LZCNT;
};

template <>
struct CpuFeatureTag<CpuFeature::MONITOR> {
    inline static constexpr CpuFeature value = CpuFeature::MONITOR;
};

template <>
struct CpuFeatureTag<CpuFeature::MOVBE> {
    inline static constexpr CpuFeature value = CpuFeature::MOVBE;
};

template <>
struct CpuFeatureTag<CpuFeature::MOVDIR64B> {
    inline static constexpr CpuFeature value = CpuFeature::MOVDIR64B;
};

template <>
struct CpuFeatureTag<CpuFeature::MOVDIRI> {
    inline static constexpr CpuFeature value = CpuFeature::MOVDIRI;
};

template <>
struct CpuFeatureTag<CpuFeature::MPX> {
    inline static constexpr CpuFeature value = CpuFeature::MPX;
};

template <>
struct CpuFeatureTag<CpuFeature::PCLMULQDQ> {
    inline static constexpr CpuFeature value = CpuFeature::PCLMULQDQ;
};

template <>
struct CpuFeatureTag<CpuFeature::PCONFIG> {
    inline static constexpr CpuFeature value = CpuFeature::PCONFIG;
};

template <>
struct CpuFeatureTag<CpuFeature::POPCNT> {
    inline static constexpr CpuFeature value = CpuFeature::POPCNT;
};

template <>
struct CpuFeatureTag<CpuFeature::PREFETCHWT1> {
    inline static constexpr CpuFeature value = CpuFeature::PREFETCHWT1;
};

template <>
struct CpuFeatureTag<CpuFeature::RDPID> {
    inline static constexpr CpuFeature value = CpuFeature::RDPID;
};

template <>
struct CpuFeatureTag<CpuFeature::RDRAND> {
    inline static constexpr CpuFeature value = CpuFeature::RDRAND;
};

template <>
struct CpuFeatureTag<CpuFeature::RDSEED> {
    inline static constexpr CpuFeature value = CpuFeature::RDSEED;
};

template <>
struct CpuFeatureTag<CpuFeature::RDTSCP> {
    inline static constexpr CpuFeature value = CpuFeature::RDTSCP;
};

template <>
struct CpuFeatureTag<CpuFeature::RTM> {
    inline static constexpr CpuFeature value = CpuFeature::RTM;
};

template <>
struct CpuFeatureTag<CpuFeature::SERIALIZE> {
    inline static constexpr CpuFeature value = CpuFeature::SERIALIZE;
};

template <>
struct CpuFeatureTag<CpuFeature::SHA> {
    inline static constexpr CpuFeature value = CpuFeature::SHA;
};

template <>
struct CpuFeatureTag<CpuFeature::TSC> {
    inline static constexpr CpuFeature value = CpuFeature::TSC;
};

template <>
struct CpuFeatureTag<CpuFeature::TSXLDTRK> {
    inline static constexpr CpuFeature value = CpuFeature::TSXLDTRK;
};

template <>
struct CpuFeatureTag<CpuFeature::UINTR> {
    inline static constexpr CpuFeature value = CpuFeature::UINTR;
};

template <>
struct CpuFeatureTag<CpuFeature::VAES> {
    inline static constexpr CpuFeature value = CpuFeature::VAES;
};

template <>
struct CpuFeatureTag<CpuFeature::VPCLMULQDQ> {
    inline inline static constexpr CpuFeature value = CpuFeature::VPCLMULQDQ;
};

template <>
struct CpuFeatureTag<CpuFeature::WAITPKG> {
    inline inline static constexpr CpuFeature value = CpuFeature::WAITPKG;
};

template <>
struct CpuFeatureTag<CpuFeature::WBNOINVD> {
    inline inline static constexpr CpuFeature value = CpuFeature::WBNOINVD;
};

template <>
struct CpuFeatureTag<CpuFeature::XSAVE> {
    inline inline static constexpr CpuFeature value = CpuFeature::XSAVE;
};

template <>
struct CpuFeatureTag<CpuFeature::XSAVEC> {
    inline inline static constexpr CpuFeature value = CpuFeature::XSAVEC;
};

template <>
struct CpuFeatureTag<CpuFeature::XSAVEOPT> {
    inline inline static constexpr CpuFeature value = CpuFeature::XSAVEOPT;
};

template <>
struct CpuFeatureTag<CpuFeature::XSS> {
    inline inline static constexpr CpuFeature value = CpuFeature::XSS;
};

__BASE_NAMESPACE_END

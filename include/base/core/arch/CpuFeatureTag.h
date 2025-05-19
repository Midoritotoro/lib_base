#pragma once 

#include <base/core/arch/CpuFeature.h>

__BASE_NAMESPACE_BEGIN

template <CpuFeature Feature>
struct CpuFeatureTag {
    static constexpr CpuFeature value = Feature;
};

template <>
struct CpuFeatureTag<CpuFeature::MMX> {
    static constexpr CpuFeature value = CpuFeature::MMX;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE> {
    static constexpr CpuFeature value = CpuFeature::SSE;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE2> {
    static constexpr CpuFeature value = CpuFeature::SSE2;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE3> {
    static constexpr CpuFeature value = CpuFeature::SSE3;
};

template <>
struct CpuFeatureTag<CpuFeature::SSSE3> {
    static constexpr CpuFeature value = CpuFeature::SSSE3;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE41> {
    static constexpr CpuFeature value = CpuFeature::SSE41;
};

template <>
struct CpuFeatureTag<CpuFeature::SSE42> {
    static constexpr CpuFeature value = CpuFeature::SSE42;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX> {
    static constexpr CpuFeature value = CpuFeature::AVX;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX2> {
    static constexpr CpuFeature value = CpuFeature::AVX2;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512> {
    static constexpr CpuFeature value = CpuFeature::AVX512;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512F> {
    static constexpr CpuFeature value = CpuFeature::AVX512F;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512BW> {
    static constexpr CpuFeature value = CpuFeature::AVX512BW;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512CD> {
    static constexpr CpuFeature value = CpuFeature::AVX512CD;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512DQ> {
    static constexpr CpuFeature value = CpuFeature::AVX512DQ;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512ER> {
    static constexpr CpuFeature value = CpuFeature::AVX512ER;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512IFMA52> {
    static constexpr CpuFeature value = CpuFeature::AVX512IFMA52;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512PF> {
    static constexpr CpuFeature value = CpuFeature::AVX512PF;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512VL> {
    static constexpr CpuFeature value = CpuFeature::AVX512VL;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512VPOPCNTDQ> {
    static constexpr CpuFeature value = CpuFeature::AVX512VPOPCNTDQ;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_4FMAPS> {
    static constexpr CpuFeature value = CpuFeature::AVX512_4FMAPS;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_4VNNIW> {
    static constexpr CpuFeature value = CpuFeature::AVX512_4VNNIW;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_BF16> {
    static constexpr CpuFeature value = CpuFeature::AVX512_BF16;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_BITALG> {
    static constexpr CpuFeature value = CpuFeature::AVX512_BITALG;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VBMI> {
    static constexpr CpuFeature value = CpuFeature::AVX512_VBMI;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VBMI2> {
    static constexpr CpuFeature value = CpuFeature::AVX512_VBMI2;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VNNI> {
    static constexpr CpuFeature value = CpuFeature::AVX512_VNNI;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_VP2INTERSECT> {
    static constexpr CpuFeature value = CpuFeature::AVX512_VP2INTERSECT;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX512_FP16> {
    static constexpr CpuFeature value = CpuFeature::AVX512_FP16;
};

template <>
struct CpuFeatureTag<CpuFeature::FMA> {
    static constexpr CpuFeature value = CpuFeature::FMA;
};

template <>
struct CpuFeatureTag<CpuFeature::AVX_VNNI> {
    static constexpr CpuFeature value = CpuFeature::AVX_VNNI;
};

template <>
struct CpuFeatureTag<CpuFeature::KNC> {
    static constexpr CpuFeature value = CpuFeature::KNC;
};

template <>
struct CpuFeatureTag<CpuFeature::AMX> {
    static constexpr CpuFeature value = CpuFeature::AMX;
};

template <>
struct CpuFeatureTag<CpuFeature::AMXBF16> {
    static constexpr CpuFeature value = CpuFeature::AMXBF16;
};

template <>
struct CpuFeatureTag<CpuFeature::AMXINT8> {
    static constexpr CpuFeature value = CpuFeature::AMXINT8;
};

template <>
struct CpuFeatureTag<CpuFeature::AMXTILE> {
    static constexpr CpuFeature value = CpuFeature::AMXTILE;
};

template <>
struct CpuFeatureTag<CpuFeature::SVML> {
    static constexpr CpuFeature value = CpuFeature::SVML;
};

template <>
struct CpuFeatureTag<CpuFeature::ADX> {
    static constexpr CpuFeature value = CpuFeature::ADX;
};

template <>
struct CpuFeatureTag<CpuFeature::AES> {
    static constexpr CpuFeature value = CpuFeature::AES;
};

template <>
struct CpuFeatureTag<CpuFeature::BMI1> {
    static constexpr CpuFeature value = CpuFeature::BMI1;
};

template <>
struct CpuFeatureTag<CpuFeature::BMI2> {
    static constexpr CpuFeature value = CpuFeature::BMI2;
};

template <>
struct CpuFeatureTag<CpuFeature::CET_SS> {
    static constexpr CpuFeature value = CpuFeature::CET_SS;
};

template <>
struct CpuFeatureTag<CpuFeature::CLDEMOTE> {
    static constexpr CpuFeature value = CpuFeature::CLDEMOTE;
};

template <>
struct CpuFeatureTag<CpuFeature::CLFLUSHOPT> {
    static constexpr CpuFeature value = CpuFeature::CLFLUSHOPT;
};

template <>
struct CpuFeatureTag<CpuFeature::CLWB> {
    static constexpr CpuFeature value = CpuFeature::CLWB;
};

template <>
struct CpuFeatureTag<CpuFeature::ENQCMD> {
    static constexpr CpuFeature value = CpuFeature::ENQCMD;
};

template <>
struct CpuFeatureTag<CpuFeature::FP16C> {
    static constexpr CpuFeature value = CpuFeature::FP16C;
};

template <>
struct CpuFeatureTag<CpuFeature::FSGSBASE> {
    static constexpr CpuFeature value = CpuFeature::FSGSBASE;
};

template <>
struct CpuFeatureTag<CpuFeature::FXSR> {
    static constexpr CpuFeature value = CpuFeature::FXSR;
};

template <>
struct CpuFeatureTag<CpuFeature::GFNI> {
    static constexpr CpuFeature value = CpuFeature::GFNI;
};

template <>
struct CpuFeatureTag<CpuFeature::HRESET> {
    static constexpr CpuFeature value = CpuFeature::HRESET;
};

template <>
struct CpuFeatureTag<CpuFeature::INVPCID> {
    static constexpr CpuFeature value = CpuFeature::INVPCID;
};

template <>
struct CpuFeatureTag<CpuFeature::KEYLOCKER> {
    static constexpr CpuFeature value = CpuFeature::KEYLOCKER;
};

template <>
struct CpuFeatureTag<CpuFeature::KEYLOCKER_WIDE> {
    static constexpr CpuFeature value = CpuFeature::KEYLOCKER_WIDE;
};

template <>
struct CpuFeatureTag<CpuFeature::LZCNT> {
    static constexpr CpuFeature value = CpuFeature::LZCNT;
};

template <>
struct CpuFeatureTag<CpuFeature::MONITOR> {
    static constexpr CpuFeature value = CpuFeature::MONITOR;
};

template <>
struct CpuFeatureTag<CpuFeature::MOVBE> {
    static constexpr CpuFeature value = CpuFeature::MOVBE;
};

template <>
struct CpuFeatureTag<CpuFeature::MOVDIR64B> {
    static constexpr CpuFeature value = CpuFeature::MOVDIR64B;
};

template <>
struct CpuFeatureTag<CpuFeature::MOVDIRI> {
    static constexpr CpuFeature value = CpuFeature::MOVDIRI;
};

template <>
struct CpuFeatureTag<CpuFeature::MPX> {
    static constexpr CpuFeature value = CpuFeature::MPX;
};

template <>
struct CpuFeatureTag<CpuFeature::PCLMULQDQ> {
    static constexpr CpuFeature value = CpuFeature::PCLMULQDQ;
};

template <>
struct CpuFeatureTag<CpuFeature::PCONFIG> {
    static constexpr CpuFeature value = CpuFeature::PCONFIG;
};

template <>
struct CpuFeatureTag<CpuFeature::POPCNT> {
    static constexpr CpuFeature value = CpuFeature::POPCNT;
};

template <>
struct CpuFeatureTag<CpuFeature::PREFETCHWT1> {
    static constexpr CpuFeature value = CpuFeature::PREFETCHWT1;
};

template <>
struct CpuFeatureTag<CpuFeature::RDPID> {
    static constexpr CpuFeature value = CpuFeature::RDPID;
};

template <>
struct CpuFeatureTag<CpuFeature::RDRAND> {
    static constexpr CpuFeature value = CpuFeature::RDRAND;
};

template <>
struct CpuFeatureTag<CpuFeature::RDSEED> {
    static constexpr CpuFeature value = CpuFeature::RDSEED;
};

template <>
struct CpuFeatureTag<CpuFeature::RDTSCP> {
    static constexpr CpuFeature value = CpuFeature::RDTSCP;
};

template <>
struct CpuFeatureTag<CpuFeature::RTM> {
    static constexpr CpuFeature value = CpuFeature::RTM;
};

template <>
struct CpuFeatureTag<CpuFeature::SERIALIZE> {
    static constexpr CpuFeature value = CpuFeature::SERIALIZE;
};

template <>
struct CpuFeatureTag<CpuFeature::SHA> {
    static constexpr CpuFeature value = CpuFeature::SHA;
};

template <>
struct CpuFeatureTag<CpuFeature::TSC> {
    static constexpr CpuFeature value = CpuFeature::TSC;
};

template <>
struct CpuFeatureTag<CpuFeature::TSXLDTRK> {
    static constexpr CpuFeature value = CpuFeature::TSXLDTRK;
};

template <>
struct CpuFeatureTag<CpuFeature::UINTR> {
    static constexpr CpuFeature value = CpuFeature::UINTR;
};

template <>
struct CpuFeatureTag<CpuFeature::VAES> {
    static constexpr CpuFeature value = CpuFeature::VAES;
};

template <>
struct CpuFeatureTag<CpuFeature::VPCLMULQDQ> {
    static constexpr CpuFeature value = CpuFeature::VPCLMULQDQ;
};

template <>
struct CpuFeatureTag<CpuFeature::WAITPKG> {
    static constexpr CpuFeature value = CpuFeature::WAITPKG;
};

template <>
struct CpuFeatureTag<CpuFeature::WBNOINVD> {
    static constexpr CpuFeature value = CpuFeature::WBNOINVD;
};

template <>
struct CpuFeatureTag<CpuFeature::XSAVE> {
    static constexpr CpuFeature value = CpuFeature::XSAVE;
};

template <>
struct CpuFeatureTag<CpuFeature::XSAVEC> {
    static constexpr CpuFeature value = CpuFeature::XSAVEC;
};

template <>
struct CpuFeatureTag<CpuFeature::XSAVEOPT> {
    static constexpr CpuFeature value = CpuFeature::XSAVEOPT;
};

template <>
struct CpuFeatureTag<CpuFeature::XSS> {
    static constexpr CpuFeature value = CpuFeature::XSS;
};

__BASE_NAMESPACE_END

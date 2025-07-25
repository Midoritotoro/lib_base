#pragma once 

#include <base/core/Types.h>
#include <base/core/utility/CommonMacros.h>

__BASE_ARCH_NAMESPACE_BEGIN

enum CpuFeature : base::uchar {
	None,
	MMX,
	SSE,
	SSE2,
	SSE3,
	SSSE3,
	SSE41,
	SSE42,
	AVX,
	AVX2,
	AVX512,
	AVX512F,
	AVX512BW,
	AVX512CD,
	AVX512DQ,
	AVX512ER,
	AVX512IFMA52,
	AVX512PF,
	AVX512VL,
	AVX512VPOPCNTDQ,
	AVX512_4FMAPS,
	AVX512_4VNNIW,
	AVX512_BF16,
	AVX512_BITALG,
	AVX512_VBMI,
	AVX512_VBMI2,
	AVX512_VNNI,
	AVX512_VP2INTERSECT,
	AVX512_FP16,
	FMA,
	AVX_VNNI,
	KNC,
	AMX,
	AMXBF16,
	AMXINT8,
	AMXTILE,
	SVML,
	ADX,
	AES,
	BMI1,
	BMI2,
	CET_SS,
	CLDEMOTE,
	CLFLUSHOPT,
	CLWB,
	ENQCMD,
	FP16C,
	FSGSBASE,
	FXSR,
	GFNI,
	HRESET,
	INVPCID,
	KEYLOCKER,
	KEYLOCKER_WIDE,
	LZCNT,
	MONITOR,
	MOVBE,
	MOVDIR64B,
	MOVDIRI,
	MPX,
	PCLMULQDQ,
	PCONFIG,
	POPCNT,
	PREFETCHWT1,
	RDPID,
	RDRAND,
	RDSEED,
	RDTSCP,
	RTM,
	SERIALIZE,
	SHA,
	TSC,
	TSXLDTRK,
	UINTR,
	VAES,
	VPCLMULQDQ,
	WAITPKG,
	WBNOINVD,
	XSAVE,
	XSAVEC,
	XSAVEOPT,
	XSS
}; 


//#ifndef BASE_STATIC_ASSERT_FEATURE_CHECK
//#define BASE_STATIC_ASSERT_FEATURE_CHECK(N, log, current, ...)	\
//	static_assert(current == __BASE_PICK_N(N, __VA_ARGS__), log)
//#endif // BASE_STATIC_ASSERT_FEATURE_CHECK
template <CpuFeature Feature, CpuFeature Candidate, typename Enable = void>
struct IsInListHelper : std::false_type {};

template <CpuFeature Feature, CpuFeature Candidate>
struct IsInListHelper<Feature, Candidate, std::enable_if_t<(Feature == Candidate)>> : std::true_type {};

template <CpuFeature Feature, CpuFeature ... List>
struct Contains {
	static constexpr bool value = (IsInListHelper<Feature, List>::value || ...);
};

#define STATIC_ASSERT_IN_LIST(current, ...)                                                   \
    static_assert(                                                                         \
        Contains<current, __VA_ARGS__>::value,                                         \
        "Error: " #current " not found in list (" #__VA_ARGS__ "). "                         \
        "Please verify the list or add " #current " to the list. "                           \
        /* Add your custom log here if needed: */                                             \
    )

int p() {
	STATIC_ASSERT_IN_LIST(CpuFeature::None, CpuFeature::XSS, CpuFeature::AVX2);
}

__BASE_ARCH_NAMESPACE_END

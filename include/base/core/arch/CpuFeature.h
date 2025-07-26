#pragma once 

#include <base/core/Types.h>
#include <base/core/utility/CommonMacros.h>

__BASE_ARCH_NAMESPACE_BEGIN

enum class CpuFeature : base::uchar {
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


template <
	CpuFeature	Feature,
	CpuFeature	Candidate,
	typename	Enable = void>
struct IsInListHelper:
	std::false_type 
{};

template <
	CpuFeature Feature, 
	CpuFeature Candidate>
struct IsInListHelper<
	Feature, Candidate, 
	std::enable_if_t<(Feature == Candidate)>>: 
		std::true_type
{};

template <
	CpuFeature		Feature,
	CpuFeature ...	List>
struct Contains {
	static constexpr bool value = (IsInListHelper<Feature, List>::value || ...);
};

#ifndef BASE_STATIC_VERIFY_CPU_FEATURE
#define BASE_STATIC_VERIFY_CPU_FEATURE(current, failureLogPrefix, ...)                       \
    static_assert(                                                                 \
        base::arch::Contains<current, __VA_ARGS__>::value,                                     \
        failureLogPrefix": "  #current " not found in supported features (" #__VA_ARGS__ ")" \
    )
#endif // BASE_STATIC_VERIFY_CPU_FEATURE

#ifndef BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION
#define BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION(functionDeclaration, featureVariableName, failureLogPrefix, ...)	\
	functionDeclaration { BASE_STATIC_VERIFY_CPU_FEATURE(featureVariableName, failureLogPrefix, __VA_ARGS__); return {}; }
#endif // BASE_DECLARE_CPU_FEATURE_GUARDED_FUNCTION

#ifndef BASE_DECLARE_CPU_FEATURE_GUARDED_CLASS
#define BASE_DECLARE_CPU_FEATURE_GUARDED_CLASS(_class, featureVariableName, failureLogPrefix, ...)	\
	_class { BASE_STATIC_VERIFY_CPU_FEATURE(featureVariableName, failureLogPrefix, __VA_ARGS__); }
#endif // BASE_DECLARE_CPU_FEATURE_GUARDED_CLASS


__BASE_ARCH_NAMESPACE_END

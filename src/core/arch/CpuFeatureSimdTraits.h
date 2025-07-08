#pragma once 

#include <base/core/arch/CpuFeature.h>
#include <src/core/utility/simd/SimdVectorType.h>

__BASE_NAMESPACE_BEGIN

template <CpuFeature feature>
struct IsXmmSimdFeature:
	std::integral_constant<bool,
		feature == CpuFeature::SSE		||
		feature == CpuFeature::SSE2		||
		feature == CpuFeature::SSSE3	||
		feature == CpuFeature::SSE41	||
		feature == CpuFeature::SSE42>
{}; 

template <CpuFeature feature>
struct IsYmmSimdFeature:
	std::integral_constant<bool,
		feature == CpuFeature::AVX			||
		feature == CpuFeature::AVX2			||
		feature == CpuFeature::FMA			||
		feature == CpuFeature::AVX_VNNI>
{}; 

template <CpuFeature feature>
struct IsZmmSimdFeature:
	std::integral_constant<bool,
		feature == CpuFeature::AVX512				||
		feature == CpuFeature::AVX512F				||
		feature == CpuFeature::AVX512BW				||
		feature == CpuFeature::AVX512CD				||
		feature == CpuFeature::AVX512DQ				||
		feature == CpuFeature::AVX512ER				||
		feature == CpuFeature::AVX512IFMA52			||
		feature == CpuFeature::AVX512PF				||
		feature == CpuFeature::AVX512VL				||
		feature == CpuFeature::AVX512VPOPCNTDQ		||
		feature == CpuFeature::AVX512_4FMAPS		||
		feature == CpuFeature::AVX512_4VNNIW		||
		feature == CpuFeature::AVX512_BF16			||
		feature == CpuFeature::AVX512_BITALG		||
		feature == CpuFeature::AVX512_VBMI			||
		feature == CpuFeature::AVX512_VBMI2			|| 
		feature == CpuFeature::AVX512_VNNI			||
		feature == CpuFeature::AVX512_VP2INTERSECT	||
		feature == CpuFeature::AVX512_FP16>
{};

template <CpuFeature feature>
struct IsSimdFeature:
	std::integral_constant<bool,
		IsXmmSimdFeature<feature>::value ||
		IsYmmSimdFeature<feature>::value ||
		IsZmmSimdFeature<feature>::value ||
		feature == CpuFeature::MMX>
{};

template <CpuFeature feature>
constexpr bool is_xmm_simd_feature_v = IsXmmSimdFeature<feature>::value;

template <CpuFeature feature>
constexpr bool is_ymm_simd_feature_v = IsYmmSimdFeature<feature>::value;

template <CpuFeature feature>
constexpr bool is_zmm_simd_feature_v = IsZmmSimdFeature<feature>::value;

template <class _Type_>
constexpr bool is_simd_feature_v = IsSimdFeature<_Type_>::value;

#if !defined(BASE_DECLARE_SIMD_TYPE_SELECTOR)
#  define BASE_DECLARE_SIMD_TYPE_SELECTOR(selectorName, simdTypeNameXmm, simdTypeNameYmm, simdTypeNameZmm, defaultType)	\
	template <CpuFeature feature>																				\
	using selectorName = std::conditional_t<is_xmm_simd_feature_v<feature>, simdTypeNameXmm,				\
		std::conditional_t<is_ymm_simd_feature_v<feature>, simdTypeNameYmm,								\
		std::conditional_t<is_zmm_simd_feature_v<feature>, simdTypeNameZmm, defaultType>>>
#endif

BASE_DECLARE_SIMD_TYPE_SELECTOR(SimdVectorIntType, __m128i, __m256i, __m512i, int32);
BASE_DECLARE_SIMD_TYPE_SELECTOR(SimdVectorFloatType, __m128, __m256, __m512, float);
BASE_DECLARE_SIMD_TYPE_SELECTOR(SimdVectorDoubleType, __m128d, __m256d, __m512d, double);

BASE_DECLARE_SIMD_TYPE_SELECTOR(BaseSimdVectorIntType, base_vec128i_t, base_vec256i_t, base_vec512i_t, int32);
BASE_DECLARE_SIMD_TYPE_SELECTOR(BaseSimdVectorFloatType, base_vec128f_t, base_vec256f_t, base_vec512f_t, float);
BASE_DECLARE_SIMD_TYPE_SELECTOR(BaseSimdVectorDoubleType, base_vec128d_t, base_vec256d_t, base_vec512d_t, double);

template <CpuFeature feature>
constexpr NODISCARD CpuFeature DowncastSimdFeatureHelper() noexcept;

template <CpuFeature feature> 
struct DowncastSimdFeature {
	static_assert(is_xmm_simd_feature_v<feature> && is_ymm_simd_feature_v<feature> && is_zmm_simd_feature_v<feature>, "Invalid feature");
	static constexpr CpuFeature value = DowncastSimdFeatureHelper(feature);
};

template <CpuFeature feature> 
constexpr CpuFeature downcast_simd_feature_v = DowncastSimdFeature<feature>::value;

template <CpuFeature feature>
constexpr NODISCARD CpuFeature DowncastSimdFeatureHelper() noexcept {
	if constexpr (is_zmm_simd_feature_v<feature>)
		return CpuFeature::AVX;
	else if constexpr (is_ymm_simd_feature_v<feature>)
		return CpuFeature::SSE;
	else if constexpr (is_xmm_simd_feature_v<feature>)
		return CpuFeature::None;
}

__BASE_NAMESPACE_END

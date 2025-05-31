#pragma once 

#include <base/core/arch/CpuFeatureTag.h>

__BASE_NAMESPACE_BEGIN

template <class _SimdType_>
struct IsXmmSimdFeature:
	std::integral_constant<bool,
		_SimdType_::value == CpuFeature::SSE		||
		_SimdType_::value == CpuFeature::SSE2		||
		_SimdType_::value == CpuFeature::SSSE3		||
		_SimdType_::value == CpuFeature::SSE41		||
		_SimdType_::value == CpuFeature::SSE42>
{}; 

template <class _SimdType_>
struct IsYmmSimdFeature:
	std::integral_constant<bool,
		_SimdType_::value == CpuFeature::AVX			||
		_SimdType_::value == CpuFeature::AVX2			||
		_SimdType_::value == CpuFeature::FMA			||
		_SimdType_::value == CpuFeature::AVX_VNNI>
{}; 

template <class _SimdType_>
struct IsZmmSimdFeature:
	std::integral_constant<bool,
		_SimdType_::value == CpuFeature::AVX512					||
		_SimdType_::value == CpuFeature::AVX512F				||
		_SimdType_::value == CpuFeature::AVX512BW				||
		_SimdType_::value == CpuFeature::AVX512CD				||
		_SimdType_::value == CpuFeature::AVX512DQ				||
		_SimdType_::value == CpuFeature::AVX512ER				||
		_SimdType_::value == CpuFeature::AVX512IFMA52			||
		_SimdType_::value == CpuFeature::AVX512PF				||
		_SimdType_::value == CpuFeature::AVX512VL				||
		_SimdType_::value == CpuFeature::AVX512VPOPCNTDQ		||
		_SimdType_::value == CpuFeature::AVX512_4FMAPS			||
		_SimdType_::value == CpuFeature::AVX512_4VNNIW			||
		_SimdType_::value == CpuFeature::AVX512_BF16			||
		_SimdType_::value == CpuFeature::AVX512_BITALG			||
		_SimdType_::value == CpuFeature::AVX512_VBMI			||
		_SimdType_::value == CpuFeature::AVX512_VBMI2			|| 
		_SimdType_::value == CpuFeature::AVX512_VNNI			||
		_SimdType_::value == CpuFeature::AVX512_VP2INTERSECT	||
		_SimdType_::value == CpuFeature::AVX512_FP16>
{};

template <class _SimdType_>
struct IsSimdFeature:
	std::integral_constant<bool,
		IsXmmSimdFeature<_SimdType_>::value ||
		IsYmmSimdFeature<_SimdType_>::value || 
		IsZmmSimdFeature<_SimdType_>::value ||
		_SimdType_::value == CpuFeature::MMX>
{};

template <class _SimdType_>
constexpr bool is_xmm_simd_feature_v = IsXmmSimdFeature<_SimdType_>::value;

template <class _SimdType_>
constexpr bool is_ymm_simd_feature_v = IsYmmSimdFeature<_SimdType_>::value;

template <class _SimdType_>
constexpr bool is_zmm_simd_feature_v = IsZmmSimdFeature<_SimdType_>::value;

template <class _Type_>
constexpr bool is_simd_feature_v = IsSimdFeature<_Type_>::value;

#if !defined(BASE_DECLARE_SIMD_TYPE_SELECTOR)
#  define BASE_DECLARE_SIMD_TYPE_SELECTOR(selectorName, simdTypeNameXmm, simdTypeNameYmm, simdTypeNameZmm)	\
	template <class _SimdType_>																				\
	using selectorName = std::conditional_t<is_xmm_simd_feature_v<_SimdType_>, simdTypeNameXmm,				\
		std::conditional_t<is_ymm_simd_feature_v<_SimdType_>, simdTypeNameYmm,								\
		std::conditional_t<is_zmm_simd_feature_v<_SimdType_>, simdTypeNameZmm, void>>>
#endif

BASE_DECLARE_SIMD_TYPE_SELECTOR(SimdVectorIntType, __m128i, __m256i, __m512i);
BASE_DECLARE_SIMD_TYPE_SELECTOR(SimdVectorFloatType, __m128, __m256, __m512);
BASE_DECLARE_SIMD_TYPE_SELECTOR(SimdVectorDoubleType, __m128d, __m256d, __m512d);

BASE_DECLARE_SIMD_TYPE_SELECTOR(BaseSimdVectorIntType, base_vec128i_t, base_vec256i_t, base_vec512i_t);
BASE_DECLARE_SIMD_TYPE_SELECTOR(BaseSimdVectorFloatType, base_vec128f_t, base_vec256f_t, base_vec512f_t);
BASE_DECLARE_SIMD_TYPE_SELECTOR(BaseSimdVectorDoubleType, base_vec128d_t, base_vec256d_t, base_vec512d_t);

__BASE_NAMESPACE_END

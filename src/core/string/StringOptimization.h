#pragma once 

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

__BASE_STRING_NAMESPACE_BEGIN

namespace stringSimd {
	struct Optimization
	{};
	// If SSE4.2 is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct OptimizationEnableOnlyIfSSE42
		: Optimization
	{};

	// If AVX is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct OptimizationEnableOnlyIfAVX 
		: Optimization
	{};

	// If AVX512 is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct OptimizationEnableOnlyIfAVX512 
		: Optimization
	{};

	// Auto-selecting the most efficient option
	struct OptimizationAuto 
		: Optimization
	{};

	// A fully scalar string
	struct OptimizationDisable
		: Optimization
	{};
};

namespace stringStorage {
	struct OptimizationDisable {
		inline static constexpr bool SmallStorageOptimization = false;
		inline static constexpr bool LargeStorageOptimization = false;
	};

	struct OptimizationSmallAndLarge {
		inline static constexpr bool SmallStorageOptimization = true;
		inline static constexpr bool LargeStorageOptimization = true;
	};

	struct OptimizationSmall {
		inline static constexpr bool SmallStorageOptimization = true;
		inline static constexpr bool LargeStorageOptimization = false;
	};

	struct OptimizationLarge {
		inline static constexpr bool SmallStorageOptimization = false;
		inline static constexpr bool LargeStorageOptimization = true;
	};
}

__BASE_STRING_NAMESPACE_END

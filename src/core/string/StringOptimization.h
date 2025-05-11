#pragma once 

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

__BASE_STRING_NAMESPACE_BEGIN

namespace stringSimd {
	// Auto-selecting the most efficient option: AVX512 > AVX2 > SSE42 > Scalar
	struct OptimizationEnable
	{};

	// A fully scalar string
	struct OptimizationDisable
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

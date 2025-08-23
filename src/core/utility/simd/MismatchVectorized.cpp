#include <src/core/utility/simd/SimdAlgorithm.h>

#include <base/core/type_traits/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <base/core/memory/MemoryUtility.h>
#include <base/core/math/Math.h>

__BASE_NAMESPACE_BEGIN

template <size_t typeSizeInbytes>
base_declare_const_function base_constexpr_cxx20 size_t MismatchVectorized(
    const void* const   firstRangeBegin,
    const void* const   secondRangeBegin,
    const size_t        length) noexcept
{

}

__BASE_NAMESPACE_END

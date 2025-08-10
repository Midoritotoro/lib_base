#include <src/core/utility/simd/SimdAlgorithm.h>

#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <base/core/memory/MemoryUtility.h>
#include <base/core/math/Math.h>

__BASE_NAMESPACE_BEGIN

base_declare_const_function   std::size_t CountSubRange8Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return 0;
}

base_declare_const_function   std::size_t CountSubRange16Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return 0;
}

base_declare_const_function   std::size_t CountSubRange32Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return 0;
}

base_declare_const_function   std::size_t CountSubRange64Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return 0;
}

__BASE_NAMESPACE_END

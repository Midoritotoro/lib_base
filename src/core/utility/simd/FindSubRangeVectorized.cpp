#include <src/core/utility/simd/SimdAlgorithm.h>

#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <base/core/memory/MemoryUtility.h>
#include <base/core/math/Math.h>

__BASE_NAMESPACE_BEGIN


base_declare_const_function base_nodiscard const void* FindSubRange8Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

base_declare_const_function base_nodiscard const void* FindSubRange16Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

base_declare_const_function base_nodiscard const void* FindSubRange32Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

base_declare_const_function base_nodiscard const void* FindSubRange64Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

__BASE_NAMESPACE_END

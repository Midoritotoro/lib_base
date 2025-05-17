#include <src/core/utility/simd/SimdAlgorithm.h>

#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/Math.h>

__BASE_NAMESPACE_BEGIN


DECLARE_NOALIAS NODISCARD const void* FindLastSubRange8Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

DECLARE_NOALIAS NODISCARD const void* FindLastSubRange16Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

DECLARE_NOALIAS NODISCARD const void* FindLastSubRange32Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

DECLARE_NOALIAS NODISCARD const void* FindLastSubRange64Bit(
    const void* firstMainRangePointer,
    const void* lastMainRangePointer,
    const void* firstSubRangePointer,
    const void* lastSubRangePointer) noexcept
{
    return nullptr;
}

__BASE_NAMESPACE_END

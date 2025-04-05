#pragma once 

#include <base/core/memory/MemoryUtility.h>

#include <base/core/thread/CommonAtomicOperations.h>
#include <base/core/utility/OverflowCheck.h>


__BASE_MEMORY_NAMESPACE_BEGIN

static inline constexpr std::atomic<uint32> MaximumAllocationSize = UINT_MAX;

NODISCARD_RETURN_RAW_PTR inline
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* Allocate(sizetype bytes) {
    void* pointer = nullptr;

    if (bytes > thread::AtomicOperations<int32>::loadRelaxed(MaximumAllocationSize))
        return nullptr;

    pointer = malloc(bytes);

    if (!pointer && !bytes) {
        bytes = 1;
        pointer = Allocate(1);
    }

    return pointer;

}

NODISCARD_RETURN_RAW_PTR inline 
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* AllocateAligned(
    sizetype bytes,
    sizetype alignment)
{
    void* pointer = nullptr;

    if (bytes > thread::AtomicOperations<int32>::loadRelaxed(MaximumAllocationSize))
        return pointer;

#if defined(OS_MAC) || defined(OS_LINUX)
    if (bytes && posix_memalign(&pointer, alignment, bytes))
        pointer = nullptr;
#else
    pointer = aligned_malloc(bytes, alignment);
#endif

    if (!pointer && !bytes) {
        bytes = 1;
        pointer = Allocate(1);
    }

    return pointer;
}

NODISCARD_RETURN_RAW_PTR inline
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* AllocateZeros(sizetype bytes)
{
    void* pointer = Allocate(bytes);

    if (pointer)
        memset(pointer, 0, bytes);

    return pointer;
}

NODISCARD_RETURN_RAW_PTR inline
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* AllocateArray(
    sizetype numberOfElements,
    sizetype singleElementSize)
{
    sizetype result = 0;

    if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
        return nullptr;

    return Allocate(result);
}

NODISCARD_RETURN_RAW_PTR inline
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* AllocateArrayAligned(
    sizetype numberOfElements,
    sizetype singleElementSize,
    sizetype alignment)
{
    sizetype result = 0;

    if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
        return nullptr;

    return AllocateAligned(result, alignment);
}

NODISCARD_RETURN_RAW_PTR inline 
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* ReAllocate(
    void* pointer,
    sizetype bytes)
{
    void* resultPointer = nullptr;

    if (bytes > thread::AtomicOperations<int32>::loadRelaxed(MaximumAllocationSize))
        return nullptr;

    return realloc(pointer, bytes + !bytes);
}


NODISCARD_RETURN_RAW_PTR inline 
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* ReAllocateAligned(
    void* pointer,
    sizetype bytes,
    sizetype alignment)
{
    void* resultPointer = nullptr;

    if (bytes > thread::AtomicOperations<int32>::loadRelaxed(MaximumAllocationSize))
        return nullptr;

    return aligned_realloc(
        pointer, bytes + !bytes,
        alignment);
}

NODISCARD_RETURN_RAW_PTR inline
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* ReallocateArray(
    void* pointer,
    sizetype numberOfElements,
    sizetype singleElementSize)
{
    sizetype result = 0;

    if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
        return nullptr;

    return ReAllocate(pointer, result);
}

NODISCARD_RETURN_RAW_PTR inline
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* ReallocateArrayAligned(
    void* pointer,
    sizetype numberOfElements,
    sizetype singleElementSize,
    sizetype alignment)
{
    sizetype result = 0;

    if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
        return nullptr;

    return ReAllocateAligned(
        pointer, result, alignment);
}

NODISCARD_RETURN_RAW_PTR inline 
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* ReallocateFast(
    void* pointer,
    uint32* size,
    sizetype minimumSize)
{
    sizetype maximumSize = 0;

    if (minimumSize <= *size)
        return pointer;

    maximumSize = thread::AtomicOperations<int32>::loadRelaxed(MaximumAllocationSize);
    maximumSize = std::min(maximumSize, static_cast<sizetype>(UINT_MAX));

    if (minimumSize > maximumSize) {
        *size = 0;
        return nullptr;
    }

    minimumSize = std::min(maximumSize,
        std::max(minimumSize + minimumSize / 16 + 32,
            minimumSize));

    pointer = ReAllocate(pointer, minimumSize);

    if (!pointer)
        minimumSize = 0;

    *size = minimumSize;

    return pointer;
}

NODISCARD_RETURN_RAW_PTR inline CLANG_CONSTEXPR_CXX20 void FreeNull(void* pointer) {
    void* value = nullptr;

    memcpy(&value, pointer, sizeof(value));
    memcpy(pointer, nullptr, sizeof(value));

    free(value);
}

NODISCARD_RETURN_RAW_PTR inline CLANG_CONSTEXPR_CXX20 void FreeNullAligned(void* pointer) {
    void* value = nullptr;

    memcpy(&value, pointer, sizeof(value));
    memcpy(pointer, nullptr, sizeof(value));

    aligned_free(value);
}

DECLARE_MEMORY_ALLOCATOR inline CLANG_CONSTEXPR_CXX20 void AllocateFast(
    void* pointer,
    uint32* size,
    sizetype minimumSize,
    bool zeroReallocate)
{
    sizetype maximumSize = 0;
    void* value = nullptr;

    memcpy(&value, pointer, sizeof(value));

    if (minimumSize <= *size)
        // Assert(value || !minimumSize);
        return;

    maximumSize = thread::AtomicOperations<int32>::loadRelaxed(MaximumAllocationSize);
    maximumSize = std::min(maximumSize, static_cast<sizetype>(UINT_MAX));

    if (minimumSize > maximumSize) {
        FreeNull(pointer);
        *size = 0;

        return;
    }

    minimumSize = std::min(maximumSize,
        std::max(minimumSize + minimumSize / 16 + 32,
            minimumSize));

    FreeNull(pointer);

    value = zeroReallocate
        ? AllocateZeros(minimumSize)
        : Allocate(minimumSize);

    memcpy(pointer, &value, sizeof(value));

    if (!value)
        minimumSize = 0;

    *size = minimumSize;
}

CONSTEXPR_CXX20 inline DECLARE_MEMORY_ALLOCATOR void AllocateZerosFast(
    void* pointer,
    uint32* size,
    sizetype minimumSize)
{
    AllocateFast(pointer, size,
        minimumSize, true);
}


NODISCARD_RETURN_RAW_PTR inline 
DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void* MemoryDuplicate(
    const void* pointer,
    sizetype size)
{
    if (!pointer)
        return nullptr;

    void* outPointer = Allocate(size);

    if (outPointer)
        memcpy(outPointer, pointer, size);

    return outPointer;
}

__BASE_MEMORY_NAMESPACE_END

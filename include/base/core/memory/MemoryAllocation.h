#pragma once 

#include <base/core/thread/CommonAtomicOperations.h>

__BASE_MEMORY_NAMESPACE_BEGIN

CLANG_CONSTEXPR_CXX20 inline uint32 MaximumMemoryAllocationSize() noexcept;

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR
CLANG_CONSTEXPR_CXX20 void* Allocate(sizetype bytes);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR
CLANG_CONSTEXPR_CXX20 void* AllocateAligned(
    sizetype bytes,
    sizetype alignment);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR 
CLANG_CONSTEXPR_CXX20 void* AllocateZeros(sizetype bytes);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR 
CLANG_CONSTEXPR_CXX20 void* AllocateArray(
    sizetype numberOfElements,
    sizetype singleElementSize);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR 
CLANG_CONSTEXPR_CXX20 void* AllocateArrayAligned(
    sizetype numberOfElements,
    sizetype singleElementSize,
    sizetype alignment);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR
CLANG_CONSTEXPR_CXX20 void* ReAllocate(
    void*       pointer,
    sizetype    bytes);


NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR 
CLANG_CONSTEXPR_CXX20 void* ReAllocateAligned(
    void*       pointer,
    sizetype    bytes,
    sizetype    alignment);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR
CLANG_CONSTEXPR_CXX20 void* ReallocateArray(
    void*       pointer,
    sizetype    numberOfElements,
    sizetype    singleElementSize);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR
CLANG_CONSTEXPR_CXX20 void* ReallocateArrayAligned(
    void*       pointer,
    sizetype    numberOfElements,
    sizetype    singleElementSize,
    sizetype    alignment);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR 
CLANG_CONSTEXPR_CXX20 void* ReallocateFast(
    void*       pointer,
    uint32*     size,
    sizetype    minimumSize);

NODISCARD_RETURN_RAW_PTR CLANG_CONSTEXPR_CXX20 void FreeNull(void* pointer);
NODISCARD_RETURN_RAW_PTR CLANG_CONSTEXPR_CXX20 void FreeNullAligned(void* pointer);

DECLARE_MEMORY_ALLOCATOR CLANG_CONSTEXPR_CXX20 void AllocateFast(
    void*       pointer,
    uint32*     size,
    sizetype    minimumSize,
    bool        zeroReallocate);

CLANG_CONSTEXPR_CXX20 CLANG_CONSTEXPR_CXX20
DECLARE_MEMORY_ALLOCATOR void AllocateZerosFast(
    void*       pointer,
    uint32*     size,
    sizetype    minimumSize);

NODISCARD_RETURN_RAW_PTR DECLARE_MEMORY_ALLOCATOR 
CLANG_CONSTEXPR_CXX20 void* MemoryDuplicate(
    const void* pointer,
    sizetype    size);

__BASE_MEMORY_NAMESPACE_END

#pragma once 

#include <base/core/thread/CommonAtomicOperations.h>

__BASE_MEMORY_NAMESPACE_BEGIN

base_clang_constexpr_cxx20 uint32 MaximumMemoryAllocationSize() noexcept;

base_clang_constexpr_cxx20 base_declare_memory_allocator void* Allocate(sizetype bytes);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* AllocateAligned(
    sizetype bytes,
    sizetype alignment);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* AllocateZeros(sizetype bytes);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* AllocateArray(
    sizetype numberOfElements,
    sizetype singleElementSize);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* AllocateArrayAligned(
    sizetype numberOfElements,
    sizetype singleElementSize,
    sizetype alignment);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* ReAllocate(
    void*       pointer,
    sizetype    bytes);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* ReAllocateAligned(
    void*       pointer,
    sizetype    bytes,
    sizetype    alignment);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* ReallocateArray(
    void*       pointer,
    sizetype    numberOfElements,
    sizetype    singleElementSize);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* ReallocateArrayAligned(
    void*       pointer,
    sizetype    numberOfElements,
    sizetype    singleElementSize,
    sizetype    alignment);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* ReallocateFast(
    void*       pointer,
    uint32*     size,
    sizetype    minimumSize);

base_clang_constexpr_cxx20 void FreeNull(void* pointer);
base_clang_constexpr_cxx20 void FreeNullAligned(void* pointer);

base_clang_constexpr_cxx20 void AllocateFast(
    void*       pointer,
    uint32*     size,
    sizetype    minimumSize,
    bool        zeroReallocate);

base_clang_constexpr_cxx20 void AllocateZerosFast(
    void*       pointer,
    uint32*     size,
    sizetype    minimumSize);

base_clang_constexpr_cxx20 base_declare_memory_allocator void* MemoryDuplicate(
    const void* pointer,
    sizetype    size);

__BASE_MEMORY_NAMESPACE_END

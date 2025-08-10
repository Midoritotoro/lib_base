#pragma once 

#include <base/core/thread/CommonAtomicOperations.h>

__BASE_MEMORY_NAMESPACE_BEGIN

base_clang_constexpr_cxx20 inline uint32 MaximumMemoryAllocationSize() noexcept;

 _return_raw_ptr base_declare_memory_allocator
base_clang_constexpr_cxx20 void* Allocate(sizetype bytes);

 _return_raw_ptr base_declare_memory_allocator
base_clang_constexpr_cxx20 void* AllocateAligned(
    sizetype bytes,
    sizetype alignment);

 _return_raw_ptr base_declare_memory_allocator 
base_clang_constexpr_cxx20 void* AllocateZeros(sizetype bytes);

 _return_raw_ptr base_declare_memory_allocator 
base_clang_constexpr_cxx20 void* AllocateArray(
    sizetype numberOfElements,
    sizetype singleElementSize);

 _return_raw_ptr base_declare_memory_allocator 
base_clang_constexpr_cxx20 void* AllocateArrayAligned(
    sizetype numberOfElements,
    sizetype singleElementSize,
    sizetype alignment);

 _return_raw_ptr base_declare_memory_allocator
base_clang_constexpr_cxx20 void* ReAllocate(
    void*       pointer,
    sizetype    bytes);


 _return_raw_ptr base_declare_memory_allocator 
base_clang_constexpr_cxx20 void* ReAllocateAligned(
    void*       pointer,
    sizetype    bytes,
    sizetype    alignment);

 _return_raw_ptr base_declare_memory_allocator
base_clang_constexpr_cxx20 void* ReallocateArray(
    void*       pointer,
    sizetype    numberOfElements,
    sizetype    singleElementSize);

 _return_raw_ptr base_declare_memory_allocator
base_clang_constexpr_cxx20 void* ReallocateArrayAligned(
    void*       pointer,
    sizetype    numberOfElements,
    sizetype    singleElementSize,
    sizetype    alignment);

 _return_raw_ptr base_declare_memory_allocator 
base_clang_constexpr_cxx20 void* ReallocateFast(
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

 _return_raw_ptr base_declare_memory_allocator 
base_clang_constexpr_cxx20 void* MemoryDuplicate(
    const void* pointer,
    sizetype    size);

__BASE_MEMORY_NAMESPACE_END

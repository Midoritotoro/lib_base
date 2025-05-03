#pragma once 

#include <src/core/memory/Alignment.h>
#include <src/core/memory/AlignmentOf.h>

#include <base/core/memory/MemoryAllocation.h>

__BASE_MEMORY_NAMESPACE_BEGIN


template<
    class _Type_,
    std::size_t _Alignment_>
class AlignedAllocator 
{
    static_assert(IsAlignmentConstant<Alignment>::value);
public:
    using value_type            = _Type_;
    using pointer               = _Type_*;

    using const_pointer         = const _Type_*;
    using void_pointer          = void*;

    using const_void_pointer    = const void*;
    using size_type             = std::size_t;

    using difference_type       =  std::ptrdiff_t;
    using reference             = _Type_&;

    using const_reference       = const _Type_&;

    template <class U>
    struct rebind {
        using other = AlignedAllocator<U, _Alignment_>;
    };

    AlignedAllocator() = default;

    template <class U>
    AlignedAllocator(const AlignedAllocator<U, _Alignment_>&) noexcept
    {}

    pointer allocate(
        size_type size,
        const_void_pointer = static_cast<const_void_pointer>(0))
    {
        enum : sizetype {
            m = MaximumSize<_Alignment_,
                AlignmentOf<value_type>::value>::value
        };

        if (size == 0)
            return 0;
    
        void* p = AllocateAligned(m, static_cast<sizetype>(sizeof(_Type_) * size));

        if (!p)
            throw std::bad_alloc();
        
        return static_cast<_Type_*>(p);
    }

    void deallocate(pointer ptr, size_type) {
        aligned_free(ptr);
    }

    constexpr size_type max_size() const noexcept {
        return MaximumObjects<value_type>::value;
    }

    template <
        class U, 
        class... Args>
    void construct(
        U* ptr,
        Args&&... args) 
    {
        ::new((void*)ptr)
            U(std::forward<Args>(args)...);
    }

    template <class U>
    void construct(U* ptr) {
        ::new((void*)ptr) 
            U();
    }

    template <class U>
    void destroy(U* ptr) {
        (void)ptr;
        ptr->~U();
    }
};

template <std::size_t _Alignment_>
class AlignedAllocator<void, _Alignment_> {
    static_assert(IsAlignmentConstant<_Alignment_>::value);
public:
    using value_type    = void;

    using pointer       = void*;
    using const_pointer = const void*;

    template <class U>
    struct rebind {
        using other = AlignedAllocator<U, _Alignment_>;
    };
};

template <
    class T,
    class U, 
    std::size_t _Alignment_>
inline bool operator==(
    const AlignedAllocator<T, _Alignment_>&,
    const AlignedAllocator<U, _Alignment_>&) noexcept
{
    return true;
}

template <
    class T,
    class U, 
    std::size_t _Alignment_>
inline bool operator!=(
    const AlignedAllocator<T, _Alignment_>&,
    const AlignedAllocator<U, _Alignment_>&) noexcept
{
    return false;
}

#endif

__BASE_MEMORY_NAMESPACE_END
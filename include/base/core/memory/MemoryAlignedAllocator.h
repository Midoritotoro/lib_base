#pragma once 

#include <base/core/memory/Memory.h>
#include <base/core/memory/MemoryTypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template<
    class _Type_,
    std::size_t Alignment>
class AlignedAllocator {
    static_assert(IsAlignmentConstant<Alignment>::value);
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef void* void_pointer;
    typedef const void* const_void_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;

    template<class U>
    struct rebind {
        typedef std::aligned_allocator<U, Alignment> other;
    };

    aligned_allocator() = default;

    template<class U>
    aligned_allocator(const aligned_allocator<U, Alignment>&) noexcept
    {}

    pointer allocate(
        size_type size,
        const_void_pointer = static_cast<const_void_pointer>(0))
    {
        enum {
            m = MaximumSize<Alignment,
                AlignmentOf<value_type>::value>::value
        };

        if (size == 0)
            return 0;
    
        void* p = aligned_malloc(m, sizeof(T) * size);

        if (!p)
            throw std::bad_alloc();
        
        return static_cast<T*>(p);
    }

    void deallocate(pointer ptr, size_type) {
        aligned_free(ptr);
    }

    constexpr size_type max_size() const noexcept {
        return MaximumObjects<T>::value;
    }

    template<class U, class... Args>
    void construct(U* ptr, Args&&... args) {
        ::new((void*)ptr) U(std::forward<Args>(args)...);
    }

    template<class U>
    void construct(U* ptr) {
        ::new((void*)ptr) U();
    }

    template<class U>
    void destroy(U* ptr) {
        (void)ptr;
        ptr->~U();
    }
};

template <std::size_t Alignment>
class AlignedAllocator<void, Alignment> {
    static_assert(IsAlignmentConstant<Alignment>::value);
public:
    typedef void value_type;
    typedef void* pointer;
    typedef const void* const_pointer;

    template<class U>
    struct rebind {
        typedef AlignedAllocator<U, Alignment> other;
    };
};

template<class T, class U, std::size_t Alignment>
inline bool
operator==(const AlignedAllocator<T, Alignment>&,
    const AlignedAllocator<U, Alignment>&) noexcept
{
    return true;
}

template<class T, class U, std::size_t Alignment>
inline bool
operator!=(const AlignedAllocator<T, Alignment>&,
    const AlignedAllocator<U, Alignment>&) noexcept
{
    return false;
}

#endif

__BASE_MEMORY_NAMESPACE_END
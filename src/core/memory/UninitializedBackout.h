#pragma once 

#include <base/core/memory/MemoryTypeTraits.h>

template <class _Allocator_>
// Class to undo partially constructed ranges in UninitializedXXX algorithms
class UninitializedBackout {
private:
    using pointer = AllocatorPointerType<_Allocator_>;

public:
    CONSTEXPR_CXX20 inline UninitializedBackout(
        pointer         destinationPointer, 
        _Allocator_&    allocator
    ) :
        _firstPointer(destinationPointer),
        _lastPointer(destinationPointer),
        _allocator(allocator)
    {}

    UninitializedBackout(const UninitializedBackout&) = delete;
    UninitializedBackout& operator=(const UninitializedBackout&) = delete;

    CONSTEXPR_CXX20 ~UninitializedBackout() {
        DestroyRange(_firstPointer, _lastPointer, _allocator);
    }

    template <class... _Types_>
    // construct a new element at *_lastPointer and increment
    CONSTEXPR_CXX20 inline void emplaceBack(_Types_&&... values) noexcept(
        std::allocator_traits<_Allocator_>::construct(
            _allocator, UnFancy(_lastPointer),
            std::forward<_Types_>(values)...))
    {
        std::allocator_traits<_Allocator_>::construct(
            _allocator, UnFancy(_lastPointer),
            std::forward<_Types_>(values)...);

        ++_lastPointer;
    }

    constexpr inline pointer release() noexcept { 
        // suppress any exception handling backout and return _lastPointer
        _firstPointer = _lastPointer;
        return _lastPointer;
    }
private:
    pointer         _firstPointer = nullptr;
    pointer         _lastPointer  = nullptr;

    _Allocator_&    _allocator;
};
#pragma once

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryAllocator.h>

#include <base/core/container/VectorIterator.h> 


__BASE_CONTAINER_NAMESPACE_BEGIN

template <
	typename _Element_,
	class _Allocator_ = memory::RawMemoryAllocator<_Element_>>
class Vector {
public:
    using value_type        = _Element_;
    using allocator_type    = _Allocator_;

    using reference         = _Element_&;
    using const_reference   = const _Element_&;

    using iterator          = VectorIterator<_Element_>;
    using const_iterator    = VectorConstIterator<_Element_>;

    using pointer           = typename _Allocator_::pointer;
    using const_pointer     = typename _Allocator_::const_pointer;

    using size_type         = typename _Allocator_::size_type;
    using difference_type   = typename _Allocator_::difference_type;

    using Iterator          = iterator;
    using ConstIterator     = const_iterator;

    using ValueType         = value_type;


    constexpr Vector() noexcept {};
    constexpr ~Vector() noexcept = default;

    inline Vector(std::initializer_list<_Element_> elements) noexcept {

    }

    inline Vector(const Vector<_Element_>& other) {

    }

    inline Vector(const std::vector<_Element_>& vector) {

    }

    inline Vector(const size_type capacity) {

    }

    inline Vector(
        const size_type capacity,
        const _Element_& fill) 
    {

    }
    
    inline Vector(Vector&& rOther) noexcept {

    }
    inline void resize(const size_type capacity) {

    }

    inline void push_back(const _Element_& element) {

    }

    inline void push_front(const _Element_& element) {

    }

    inline void append(const _Element_& element) {

    }


private:
    value_type* _start = nullptr;
    value_type* _end = nullptr;
};

__BASE_CONTAINER_NAMESPACE_END
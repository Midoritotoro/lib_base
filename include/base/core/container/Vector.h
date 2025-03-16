#pragma once

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryAllocator.h>

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

    using pointer           = typename _Allocator_::pointer;
    using const_pointer     = typename _Allocator_::const_pointer;

    using size_type         = typename _Allocator_::size_type;
    using difference_type   = typename _Allocator_::difference_type;
};

__BASE_CONTAINER_NAMESPACE_END
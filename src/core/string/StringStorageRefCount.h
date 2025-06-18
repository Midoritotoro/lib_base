#pragma once 

#include <base/core/utility/TypeTraits.h>
#include <base/core/thread/AtomicInteger.h>

#include <base/core/utility/OverflowCheck.h>
#include <src/core/string/BasicStringUtility.h>

__BASE_STRING_NAMESPACE_BEGIN

template <typename _Char_>
struct RefCounted {
    std::atomic<size_t> _refCount;
    _Char_ _data[1];

    constexpr static size_t getDataOffset() {
        return offsetof(RefCounted, _data);
    }

    static RefCounted* fromData(_Char_* p) {
        return static_cast<RefCounted*>(static_cast<void*>(
            static_cast<unsigned char*>(static_cast<void*>(p)) -
            getDataOffset()));
    }

    static size_t refs(_Char_* p) {
        return fromData(p)->_refCount.load(std::memory_order_acquire);
    }

    static void incrementRefs(_Char_* p) {
        fromData(p)->_refCount.fetch_add(1, std::memory_order_acq_rel);
    }

    static void decrementRefs(_Char_* p) {
        auto const dis = fromData(p);
        size_t oldcnt = dis->_refCount.fetch_sub(1, std::memory_order_acq_rel);

        assert(oldcnt > 0);
        if (oldcnt == 1)
            free(dis);
    }

    static RefCounted* create(size_t* size) {
        size_t capacityBytes = 0;

        if (AdditionOverflow(*size, size_t(1), &capacityBytes))
            return nullptr;

        if (MultiplyOverflow(
            capacityBytes, size_t(sizeof(_Char_)), &capacityBytes)
        )
            return nullptr;

        if (AdditionOverflow(
            capacityBytes, getDataOffset(), &capacityBytes)
        )
            return nullptr;

        const size_t allocSize = capacityBytes;
        auto result = static_cast<RefCounted*>(malloc(allocSize));

        result->_refCount.store(1, std::memory_order_release);
        *size = (allocSize - getDataOffset()) / sizeof(_Char_) - 1;

        return result;
    }

    static RefCounted* create(const _Char_* data, size_t* size) {
        const size_t effectiveSize = *size;
        auto result = create(size);

        if (effectiveSize > 0)
            podCopy(data, data + effectiveSize, result->_data);

        return result;
    }

    static RefCounted* reallocate(
        _Char_* const data,
        const size_t currentSize,
        const size_t currentCapacity,
        size_t* newCapacity)
    {
        assert(*newCapacity > 0 && *newCapacity > currentSize);
        size_t capacityBytes;

        if (AdditionOverflow(*newCapacity, size_t(1), &capacityBytes))
            return nullptr;

        if (MultiplyOverflow(
            &capacityBytes, capacityBytes, sizeof(_Char_))
        )
            return nullptr;

        if (AdditionOverflow(
            capacityBytes, getDataOffset(), &capacityBytes)
        )
            return nullptr;

        const size_t allocNewCapacity = capacityBytes;
        auto const dis = fromData(data);

        assert(dis->_refCount.load(std::memory_order_acquire) == 1);

        auto result = static_cast<RefCounted*>(smartRealloc(
            dis,
            getDataOffset() + (currentSize + 1) * sizeof(_Char_),
            getDataOffset() + (currentCapacity + 1) * sizeof(_Char_),
            allocNewCapacity));

        assert(result->_refCount.load(std::memory_order_acquire) == 1);
        *newCapacity = (allocNewCapacity - getDataOffset()) / sizeof(_Char_) - 1;

        return result;
    }
};


__BASE_STRING_NAMESPACE_END

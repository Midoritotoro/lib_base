#pragma once 

#include <base/Flags.h>
#include <base/system/Platform.h>

struct CalculateGrowingBlockSizeResult
{
    qsizetype size;
    qsizetype elementCount;
};


namespace base::io {
    struct ArrayData
    {
        enum AllocationOption {
            Grow,
            KeepSize
        };

        enum GrowthPosition {
            GrowsAtEnd,
            GrowsAtBeginning
        };

        enum ArrayOption {
            ArrayOptionDefault = 0,
            CapacityReserved = 0x1  //!< the capacity was reserved by the user, try to keep it
        };

        DECLARE_FLAGS(ArrayOptions, ArrayOption)

        std::atomic<int> ref_;
        ArrayOptions flags;
        sizetype alloc;

        sizetype allocatedCapacity() noexcept
        {
            return alloc;
        }

        sizetype constAllocatedCapacity() const noexcept
        {
            return alloc;
        }

        bool ref() noexcept
        {
            ref_.fetch_add(1, std::memory_order_acq_rel);
            return true;
        }

        bool deref() noexcept
        {
            return ref_.fetch_sub(1, std::memory_order_acq_rel);
        }

        bool isShared() const noexcept
        {
            return ref_.load(std::memory_order_relaxed) != 1;
        }

        bool needsDetach() noexcept
        {
            return ref_.load(std::memory_order_relaxed) > 1;
        }

        sizetype detachCapacity(sizetype newSize) const noexcept
        {
            if (flags & CapacityReserved && newSize < constAllocatedCapacity())
                return constAllocatedCapacity();
            return newSize;
        }

        DECLARE_MALLOCLIKE
        static void* allocate(
            ArrayData** pdata, sizetype objectSize,
            sizetype alignment, sizetype capacity,
            AllocationOption option = ArrayData::KeepSize) noexcept;

        DECLARE_MALLOCLIKE
        static void* allocate1(
            ArrayData** pdata, sizetype capacity,
            AllocationOption option = ArrayData::KeepSize) noexcept;

        DECLARE_MALLOCLIKE
        static void* allocate2(
            ArrayData** pdata, sizetype capacity,
            AllocationOption option = ArrayData::KeepSize) noexcept;

        [[nodiscard]] static std::pair<ArrayData*, void*>
            reallocateUnaligned(
                ArrayData* data, void* dataPointer,
                sizetype objectSize, sizetype newCapacity,
                AllocationOption option) noexcept;

        static void deallocate(
            ArrayData* data, sizetype objectSize,
            sizetype alignment) noexcept;
    };

#if defined(PROCESSOR_X86_32) && defined(CC_GNU)
    inline constexpr size_t MaxPrimitiveAlignment = 2 * sizeof(void*);
#else
    inline constexpr size_t MaxPrimitiveAlignment = alignof(std::max_align_t);
#endif

    struct alignas(MaxPrimitiveAlignment) AlignedArrayData : ArrayData
    {};

    template <class T>
    struct TypedArrayData
        : ArrayData
    {
        struct AlignmentDummy {
            AlignedArrayData header;
            T data;
        };

        [[nodiscard]] static std::pair<TypedArrayData*, T*> allocate(qsizetype capacity, AllocationOption option = ArrayData::KeepSize)
        {
            static_assert(sizeof(TypedArrayData) == sizeof(ArrayData));

            ArrayData* d;
            void* result;

            if constexpr (sizeof(T) == 1)
                result = allocate1(&d, capacity, option);
            else if constexpr (sizeof(T) == 2)
                result = allocate2(&d, capacity, option);
            else
                result = ArrayData::allocate(&d, sizeof(T), alignof(AlignmentDummy), capacity, option);

#if __has_builtin(__builtin_assume_aligned)
            result = __builtin_assume_aligned(result, alignof(AlignmentDummy));
#endif
            return {
                static_cast<TypedArrayData*>(d),
                static_cast<T*>(result)
            };
        }

        static std::pair<TypedArrayData*, T*>
            reallocateUnaligned(TypedArrayData* data, T* dataPointer, qsizetype capacity, AllocationOption option)
        {
            static_assert(sizeof(TypedArrayData) == sizeof(ArrayData));
            std::pair<ArrayData*, void*> pair =
                ArrayData::reallocateUnaligned(data, dataPointer, sizeof(T), capacity, option);

            return {
                static_cast<TypedArrayData*>(pair.first),
                static_cast<T*>(pair.second)
            };
        }

        static void deallocate(ArrayData* data) noexcept
        {
            static_assert(sizeof(TypedArrayData) == sizeof(ArrayData));
            ArrayData::deallocate(data, sizeof(T), alignof(AlignmentDummy));
        }

        static T* dataStart(ArrayData* data, qsizetype alignment) noexcept
        {
            Assert(alignment >= qsizetype(alignof(ArrayData)) && !(alignment & (alignment - 1)));
            void* start = reinterpret_cast<void*>(
                (quintptr(data) + sizeof(ArrayData) + alignment - 1) & ~(alignment - 1));
            return static_cast<T*>(start);
        }
    };
} // base::io
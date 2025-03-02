#include <base/io/ArrayData.h>
#include <base/io/ByteArray.h>

#include <base/utility/BitOps.h>
#include <base/utility/Math.h>

#include <cstdlib>


namespace base::io {
    sizetype CalculateBlockSize(sizetype elementCount, sizetype elementSize, sizetype headerSize) noexcept
    {
        Assert(elementSize);

        size_t bytes;

        if (UNLIKELY(qMulOverflow(size_t(elementSize), size_t(elementCount), &bytes)) ||
            UNLIKELY(qAddOverflow(bytes, size_t(headerSize), &bytes)))
            return -1;

        if (UNLIKELY(sizetype(bytes) < 0))
            return -1;

        return sizetype(bytes);
    }


    CalculateGrowingBlockSizeResult
        CalculateGrowingBlockSize(sizetype elementCount, sizetype elementSize, sizetype headerSize) noexcept
    {
        CalculateGrowingBlockSizeResult result = {
            sizetype(-1), sizetype(-1)
        };

        sizetype bytes = CalculateBlockSize(elementCount, elementSize, headerSize);
        if (bytes < 0)
            return result;

        size_t morebytes = static_cast<size_t>(NextPowerOfTwo(uint64(bytes)));
        if (UNLIKELY(sizetype(morebytes) < 0)) {
            // grow by half the difference between bytes and morebytes
            // this slows the growth and avoids trying to allocate exactly
            // 2G of memory (on 32bit), something that many OSes can't deliver
            bytes += (morebytes - bytes) / 2;
        }
        else {
            bytes = sizetype(morebytes);
        }

        result.elementCount = (bytes - headerSize) / elementSize;
        result.size = result.elementCount * elementSize + headerSize;
        return result;
    }

    /*
        Calculate the byte size for a block of \a capacity objects of size \a
        objectSize, with a header of size \a headerSize. If the \a option is
        QArrayData::Grow, the capacity itself adjusted up, preallocating room for
        more elements to be added later; otherwise, it is an exact calculation.

        Returns a structure containing the size in bytes and elements available.
    */
    static inline CalculateGrowingBlockSizeResult
        calculateBlockSize(sizetype capacity, sizetype objectSize, sizetype headerSize, ArrayData::AllocationOption option)
    {
        // Adjust the header size up to account for the trailing null for QString
        // and QByteArray. This is not checked for overflow because headers sizes
        // should not be anywhere near the overflow limit.
        constexpr sizetype FooterSize = std::max(sizeof(char16_t), sizeof(ByteArray::value_type));
        if (objectSize <= FooterSize)
            headerSize += FooterSize;

        // allocSize = objectSize * capacity + headerSize, but checked for overflow
        // plus padded to grow in size
        if (option == ArrayData::Grow) {
            return CalculateGrowingBlockSize(capacity, objectSize, headerSize);
        }
        else {
            return { CalculateBlockSize(capacity, objectSize, headerSize), capacity };
        }
    }

    static ArrayData* allocateData(qsizetype allocSize)
    {
        ArrayData* header = static_cast<ArrayData*>(::malloc(size_t(allocSize)));
        if (header) {
            header->ref_.storeRelaxed(1);
            header->flags = {};
            header->alloc = 0;
        }
        return header;
    }

    namespace {
        struct AllocationResult {
            void* data;
            ArrayData* header;
        };
    }

    static inline AllocationResult
        allocateHelper(sizetype objectSize, sizetype alignment, sizetype capacity,
            ArrayData::AllocationOption option) noexcept
    {
        if (capacity == 0)
            return {};

        sizetype headerSize = sizeof(AlignedArrayData);
        const sizetype headerAlignment = alignof(AlignedArrayData);

        if (alignment > headerAlignment) {
            // Allocate extra (alignment - Q_ALIGNOF(AlignedQArrayData)) padding
            // bytes so we can properly align the data array. This assumes malloc is
            // able to provide appropriate alignment for the header -- as it should!
            // Effectively, we allocate one QTypedArrayData<T>::AlignmentDummy.
            headerSize += alignment - headerAlignment;
        }
        Assert(headerSize > 0);

        auto blockSize = calculateBlockSize(capacity, objectSize, headerSize, option);
        capacity = blockSize.elementCount;
        sizetype allocSize = blockSize.size;
        if (Q_UNLIKELY(allocSize < 0))      // handle overflow. cannot allocate reliably
            return {};

        ArrayData* header = allocateData(allocSize);
        void* data = nullptr;
        if (header) {
            // find where offset should point to so that data() is aligned to alignment bytes
            data = TypedArrayData<void>::dataStart(header, alignment);
            header->alloc = sizetype(capacity);
        }

        return { data, header };
    }

    // Generic size and alignment allocation function
    void* ArrayData::allocate(ArrayData** dptr, sizetype objectSize, sizetype alignment,
        sizetype capacity, AllocationOption option) noexcept
    {
        Assert(dptr);
        // Alignment is a power of two
        Assert(alignment >= qsizetype(alignof(QArrayData))
            && !(alignment & (alignment - 1)));

        auto r = allocateHelper(objectSize, alignment, capacity, option);
        *dptr = r.header;
        return r.data;
    }

    // Fixed size and alignment allocation functions
    void* ArrayData::allocate1(ArrayData** dptr, sizetype capacity, AllocationOption option) noexcept
    {
        Assert(dptr);

        auto r = allocateHelper(1, alignof(AlignedArrayData), capacity, option);
        *dptr = r.header;
        return r.data;
    }

    void* ArrayData::allocate2(ArrayData** dptr, sizetype capacity, AllocationOption option) noexcept
    {
        Assert(dptr);

        auto r = allocateHelper(2, alignof(AlignedArrayData), capacity, option);
        *dptr = r.header;
        return r.data;
    }

    std::pair<ArrayData*, void*>
        ArrayData::reallocateUnaligned(ArrayData* data, void* dataPointer,
            sizetype objectSize, sizetype capacity, AllocationOption option) noexcept
    {
        Assert(!data || !data->isShared());

        const sizetype headerSize = sizeof(AlignedArrayData);
        auto r = calculateBlockSize(capacity, objectSize, headerSize, option);
        sizetype allocSize = r.size;
        capacity = r.elementCount;
        if (UNLIKELY(allocSize < 0))
            return {};

        const ptrdiff offset = dataPointer
            ? reinterpret_cast<char*>(dataPointer) - reinterpret_cast<char*>(data)
            : headerSize;
        Assert(offset > 0);
        Assert(offset <= allocSize); // equals when all free space is at the beginning

        ArrayData* header = static_cast<ArrayData*>(::realloc(data, size_t(allocSize)));
        if (header) {
            header->alloc = capacity;
            dataPointer = reinterpret_cast<char*>(header) + offset;
        }
        else {
            dataPointer = nullptr;
        }
        return { header, dataPointer };
    }

    void ArrayData::deallocate(ArrayData* data, sizetype objectSize,
        sizetype alignment) noexcept
    {
        // Alignment is a power of two
        Assert(alignment >= sizetype(alignof(ArrayData))
            && !(alignment & (alignment - 1)));
        UNUSED(objectSize);
        UNUSED(alignment);

        ::free(data);
    }
} // namespace base
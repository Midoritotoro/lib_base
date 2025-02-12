#include <base/io/ArrayData.h>
#include <base/io/ByteArray.h>

#include <cstdlib>

namespace base::io {
    sizetype qCalculateBlockSize(sizetype elementCount, sizetype elementSize, sizetype headerSize) noexcept
    {
        Assert(elementSize);

        size_t bytes;

        if (unlikely(qMulOverflow(size_t(elementSize), size_t(elementCount), &bytes)) ||
            unlikely(qAddOverflow(bytes, size_t(headerSize), &bytes)))
            return -1;

        if (unlikely(sizetype(bytes) < 0))
            return -1;

        return sizetype(bytes);
    }

    /*!
        \internal
        \since 5.7

        Returns the memory block size and the number of elements that will fit in
        that block for a container containing \a elementCount elements, each of \a
        elementSize bytes, plus a header of \a headerSize bytes. This function
        assumes the container will grow and pre-allocates a growth factor.

        Both \a elementCount and \a headerSize can be zero, but \a elementSize
        cannot.

        This function returns -1 on overflow or if the memory block size
        would not fit a qsizetype.

        \note The memory block may contain up to \a elementSize - 1 bytes more than
        needed.
    */
    CalculateGrowingBlockSizeResult
        qCalculateGrowingBlockSize(sizetype elementCount, sizetype elementSize, sizetype headerSize) noexcept
    {
        CalculateGrowingBlockSizeResult result = {
            sizetype(-1), sizetype(-1)
        };

        qsizetype bytes = qCalculateBlockSize(elementCount, elementSize, headerSize);
        if (bytes < 0)
            return result;

        size_t morebytes = static_cast<size_t>(qNextPowerOfTwo(quint64(bytes)));
        if (Q_UNLIKELY(qsizetype(morebytes) < 0)) {
            // grow by half the difference between bytes and morebytes
            // this slows the growth and avoids trying to allocate exactly
            // 2G of memory (on 32bit), something that many OSes can't deliver
            bytes += (morebytes - bytes) / 2;
        }
        else {
            bytes = qsizetype(morebytes);
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
        calculateBlockSize(qsizetype capacity, qsizetype objectSize, qsizetype headerSize, QArrayData::AllocationOption option)
    {
        // Adjust the header size up to account for the trailing null for QString
        // and QByteArray. This is not checked for overflow because headers sizes
        // should not be anywhere near the overflow limit.
        constexpr qsizetype FooterSize = qMax(sizeof(QString::value_type), sizeof(QByteArray::value_type));
        if (objectSize <= FooterSize)
            headerSize += FooterSize;

        // allocSize = objectSize * capacity + headerSize, but checked for overflow
        // plus padded to grow in size
        if (option == QArrayData::Grow) {
            return qCalculateGrowingBlockSize(capacity, objectSize, headerSize);
        }
        else {
            return { qCalculateBlockSize(capacity, objectSize, headerSize), capacity };
        }
    }

    static QArrayData* allocateData(qsizetype allocSize)
    {
        QArrayData* header = static_cast<QArrayData*>(::malloc(size_t(allocSize)));
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
            QArrayData* header;
        };
    }
    using QtPrivate::AlignedQArrayData;

    static inline AllocationResult
        allocateHelper(qsizetype objectSize, qsizetype alignment, qsizetype capacity,
            QArrayData::AllocationOption option) noexcept
    {
        if (capacity == 0)
            return {};

        qsizetype headerSize = sizeof(AlignedQArrayData);
        const qsizetype headerAlignment = alignof(AlignedQArrayData);

        if (alignment > headerAlignment) {
            // Allocate extra (alignment - Q_ALIGNOF(AlignedQArrayData)) padding
            // bytes so we can properly align the data array. This assumes malloc is
            // able to provide appropriate alignment for the header -- as it should!
            // Effectively, we allocate one QTypedArrayData<T>::AlignmentDummy.
            headerSize += alignment - headerAlignment;
        }
        Q_ASSERT(headerSize > 0);

        auto blockSize = calculateBlockSize(capacity, objectSize, headerSize, option);
        capacity = blockSize.elementCount;
        qsizetype allocSize = blockSize.size;
        if (Q_UNLIKELY(allocSize < 0))      // handle overflow. cannot allocate reliably
            return {};

        QArrayData* header = allocateData(allocSize);
        void* data = nullptr;
        if (header) {
            // find where offset should point to so that data() is aligned to alignment bytes
            data = QTypedArrayData<void>::dataStart(header, alignment);
            header->alloc = qsizetype(capacity);
        }

        return { data, header };
    }

    // Generic size and alignment allocation function
    void* QArrayData::allocate(QArrayData** dptr, qsizetype objectSize, qsizetype alignment,
        qsizetype capacity, AllocationOption option) noexcept
    {
        Q_ASSERT(dptr);
        // Alignment is a power of two
        Q_ASSERT(alignment >= qsizetype(alignof(QArrayData))
            && !(alignment & (alignment - 1)));

        auto r = allocateHelper(objectSize, alignment, capacity, option);
        *dptr = r.header;
        return r.data;
    }

    // Fixed size and alignment allocation functions
    void* QArrayData::allocate1(QArrayData** dptr, qsizetype capacity, AllocationOption option) noexcept
    {
        Q_ASSERT(dptr);

        auto r = allocateHelper(1, alignof(AlignedQArrayData), capacity, option);
        *dptr = r.header;
        return r.data;
    }

    void* QArrayData::allocate2(QArrayData** dptr, qsizetype capacity, AllocationOption option) noexcept
    {
        Q_ASSERT(dptr);

        auto r = allocateHelper(2, alignof(AlignedQArrayData), capacity, option);
        *dptr = r.header;
        return r.data;
    }

    std::pair<QArrayData*, void*>
        QArrayData::reallocateUnaligned(ArrayData* data, void* dataPointer,
            sizetype objectSize, sizetype capacity, AllocationOption option) noexcept
    {
        Q_ASSERT(!data || !data->isShared());

        const sizetype headerSize = sizeof(AlignedArrayData);
        auto r = calculateBlockSize(capacity, objectSize, headerSize, option);
        sizetype allocSize = r.size;
        capacity = r.elementCount;
        if (Q_UNLIKELY(allocSize < 0))
            return {};

        const qptrdiff offset = dataPointer
            ? reinterpret_cast<char*>(dataPointer) - reinterpret_cast<char*>(data)
            : headerSize;
        Q_ASSERT(offset > 0);
        Q_ASSERT(offset <= allocSize); // equals when all free space is at the beginning

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
        Q_ASSERT(alignment >= sizetype(alignof(ArrayData))
            && !(alignment & (alignment - 1)));
        Q_UNUSED(objectSize);
        Q_UNUSED(alignment);

        ::free(data);
    }
} // namespace base
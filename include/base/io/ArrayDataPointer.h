#pragma once

#include <base/utility/Flags.h>
#include <base/system/Platform.h>

#include <base/utility/Assert.h>
#include <atomic>

#include <base/utility/ClassHelperMacros.h>
#include <base/utility/Swap.h>

#include <base/io/ArrayDataOps.h>


namespace base::io {
    template <class T>
    struct ArrayDataPointer
    {
    private:
        typedef TypedArrayData<T> Data;
        typedef ArrayDataOps<T> DataOps;

    public:
        enum {
            pass_parameter_by_value =
            std::is_arithmetic<T>::value ||
            std::is_pointer<T>::value ||
            std::is_enum<T>::value
        };

        typedef typename std::conditional<pass_parameter_by_value, T, const T&>::type parameter_type;

        constexpr ArrayDataPointer() noexcept
            : d(nullptr), ptr(nullptr), size(0)
        {
        }

        ArrayDataPointer(const ArrayDataPointer& other) noexcept
            : d(other.d), ptr(other.ptr), size(other.size)
        {
            ref();
        }

        constexpr ArrayDataPointer(Data* header, T* adata, sizetype n = 0) noexcept
            : d(header), ptr(adata), size(n)
        {
        }

        explicit ArrayDataPointer(std::pair<TypedArrayData<T>*, T*> adata, sizetype n = 0) noexcept
            : d(adata.first), ptr(adata.second), size(n)
        {
        }

        explicit
            ArrayDataPointer(sizetype alloc, sizetype n = 0,
                ArrayData::AllocationOption option = ArrayData::KeepSize)
            : ArrayDataPointer(Data::allocate(alloc, option), n)
        {
        }

        static ArrayDataPointer fromRawData(const T* rawData, sizetype length) noexcept
        {
            Assert(rawData || !length);
            return {
                nullptr,
                const_cast<T*>(rawData),
                length
            };
        }

        ArrayDataPointer& operator=(const ArrayDataPointer& other) noexcept
        {
            ArrayDataPointer tmp(other);
            this->swap(tmp);
            return *this;
        }

        ArrayDataPointer(ArrayDataPointer&& other) noexcept
            : d(std::exchange(other.d, nullptr)),
            ptr(std::exchange(other.ptr, nullptr)),
            size(std::exchange(other.size, 0))
        {
        }

        MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(ArrayDataPointer)

            DataOps& operator*() noexcept
        {
            return *static_cast<DataOps*>(this);
        }

        DataOps* operator->() noexcept
        {
            return static_cast<DataOps*>(this);
        }

        const DataOps& operator*() const noexcept
        {
            return *static_cast<const DataOps*>(this);
        }

        const DataOps* operator->() const noexcept
        {
            return static_cast<const DataOps*>(this);
        }

        ~ArrayDataPointer()
        {
            if (!deref()) {
                (*this)->destroyAll();
                free(d);
            }
        }

        bool isNull() const noexcept
        {
            return !ptr;
        }

        T* data() noexcept { return ptr; }
        const T* data() const noexcept { return ptr; }

        T* begin() noexcept { return data(); }
        T* end() noexcept { return data() + size; }
        const T* begin() const noexcept { return data(); }
        const T* end() const noexcept { return data() + size; }
        const T* constBegin() const noexcept { return data(); }
        const T* constEnd() const noexcept { return data() + size; }

        void swap(ArrayDataPointer& other) noexcept
        {
            PtrSwap(d, other.d);
            PtrSwap(ptr, other.ptr);
            std::swap(size, other.size);
        }

        void clear() noexcept(std::is_nothrow_destructible<T>::value)
        {
            ArrayDataPointer tmp;
            swap(tmp);
        }

        void detach(ArrayDataPointer* old = nullptr)
        {
            if (needsDetach())
                reallocateAndGrow(ArrayData::GrowsAtEnd, 0, old);
        }

        template <typename X> ArrayDataPointer<X> reinterpreted()&&
        {
            if (sizeof(T) != sizeof(X)) {
                Assert(!d->isShared());
                d->alloc = d->alloc * sizeof(T) / sizeof(X);
            }
            auto od = reinterpret_cast<TypedArrayData<X> *>(std::exchange(d, nullptr));
            auto optr = reinterpret_cast<X*>(std::exchange(ptr, nullptr));
            return { od, optr, std::exchange(size, 0) };
        }

        void detachAndGrow(ArrayData::GrowthPosition where, sizetype n, const T** data,
            ArrayDataPointer* old)
        {
            const bool detach = needsDetach();
            bool readjusted = false;
            if (!detach) {
                if (!n || (where == ArrayData::GrowsAtBeginning && freeSpaceAtBegin() >= n)
                    || (where == ArrayData::GrowsAtEnd && freeSpaceAtEnd() >= n))
                    return;
                readjusted = tryReadjustFreeSpace(where, n, data);
                _ASSERT(!readjusted
                    || (where == ArrayData::GrowsAtBeginning && freeSpaceAtBegin() >= n)
                    || (where == ArrayData::GrowsAtEnd && freeSpaceAtEnd() >= n));
            }

            if (!readjusted)
                reallocateAndGrow(where, n, old);
        }

        never_inline void reallocateAndGrow(ArrayData::GrowthPosition where, sizetype n,
            ArrayDataPointer* old = nullptr)
        {
            if constexpr (QTypeInfo<T>::isRelocatable && alignof(T) <= alignof(std::max_align_t)) {
                if (where == ArrayData::GrowsAtEnd && !old && !needsDetach() && n > 0) {
                    (*this)->reallocate(constAllocatedCapacity() - freeSpaceAtEnd() + n, ArrayData::Grow); // fast path
                    return;
                }
            }

            ArrayDataPointer dp(allocateGrow(*this, n, where));
            if (n > 0)
                Assert(dp.data() != nullptr); // / / / / / / / /
            if (where == ArrayData::GrowsAtBeginning) {
                Assert(dp.freeSpaceAtBegin() >= n);
            }
            else {
                Assert(dp.freeSpaceAtEnd() >= n);
            }
            if (size) {
                sizetype toCopy = size;
                if (n < 0)
                    toCopy += n;
                if (needsDetach() || old)
                    dp->copyAppend(begin(), begin() + toCopy);
                else
                    dp->moveAppend(begin(), begin() + toCopy);
                Assert(dp.size == toCopy);
            }

            swap(dp);
            if (old)
                old->swap(dp);
        }

        bool tryReadjustFreeSpace(ArrayData::GrowthPosition pos, sizetype n, const T** data = nullptr)
        {
            Assert(!this->needsDetach());
            Assert(n > 0);
            Assert((pos == ArrayData::GrowsAtEnd && this->freeSpaceAtEnd() < n)
                || (pos == ArrayData::GrowsAtBeginning && this->freeSpaceAtBegin() < n));

            const sizetype capacity = this->constAllocatedCapacity();
            const sizetype freeAtBegin = this->freeSpaceAtBegin();
            const sizetype freeAtEnd = this->freeSpaceAtEnd();

            sizetype dataStartOffset = 0;
            // algorithm:
            //   a. GrowsAtEnd: relocate if space at begin AND size < (capacity * 2) / 3
            //      [all goes to free space at end]:
            //      new free space at begin = 0
            //
            //   b. GrowsAtBeginning: relocate if space at end AND size < capacity / 3
            //      [balance the free space]:
            //      new free space at begin = n + (total free space - n) / 2
            if (pos == ArrayData::GrowsAtEnd && freeAtBegin >= n
                && ((3 * this->size) < (2 * capacity))) {
                // dataStartOffset = 0; - done in declaration
            }
            else if (pos == ArrayData::GrowsAtBeginning && freeAtEnd >= n
                && ((3 * this->size) < capacity)) {
                // total free space == capacity - size
                dataStartOffset = n + qMax(0, (capacity - this->size - n) / 2);
            }
            else {
                // nothing to do otherwise
                return false;
            }

            relocate(dataStartOffset - freeAtBegin, data);

            Assert((pos == ArrayData::GrowsAtEnd && this->freeSpaceAtEnd() >= n)
                || (pos == ArrayData::GrowsAtBeginning && this->freeSpaceAtBegin() >= n));
            return true;
        }

        /*! \internal

            Relocates [begin(), end()) by \a offset and updates \a data if it is not
            \c nullptr and points into [begin(), end()).
        */
        void relocate(sizetype offset, const T** data = nullptr)
        {
            T* res = this->ptr + offset;
            QtPrivate::q_relocate_overlap_n(this->ptr, this->size, res);
            // first update data pointer, then this->ptr
            if (data && QtPrivate::q_points_into_range(*data, *this))
                *data += offset;
            this->ptr = res;
        }

        template <typename InputIterator, typename Projection = q20::identity>
        void assign(InputIterator first, InputIterator last, Projection proj = {})
        {
            // This function only provides the basic exception guarantee.
            constexpr bool IsFwdIt = std::is_convertible_v<
                typename std::iterator_traits<InputIterator>::iterator_category,
                std::forward_iterator_tag>;
            constexpr bool IsIdentity = std::is_same_v<Projection, identity>;

            if constexpr (IsFwdIt) {
                const sizetype n = std::distance(first, last);
                if (needsDetach() || n > constAllocatedCapacity()) {
                    ArrayDataPointer allocated(detachCapacity(n));
                    swap(allocated);
                }
            }
            else if (needsDetach()) {
                ArrayDataPointer allocated(allocatedCapacity());
                swap(allocated);
                // We don't want to copy data that we know we'll overwrite
            }

            auto offset = freeSpaceAtBegin();
            const auto capacityBegin = begin() - offset;
            const auto prependBufferEnd = begin();

            if constexpr (!std::is_nothrow_constructible_v<T, decltype(std::invoke(proj, *first))>) {
                // If construction can throw, and we have freeSpaceAtBegin(),
                // it's easiest to just clear the container and start fresh.
                // The alternative would be to keep track of two active, disjoint ranges.
                if (offset) {
                    (*this)->truncate(0);
                    setBegin(capacityBegin);
                    offset = 0;
                }
            }

            auto dst = capacityBegin;
            const auto dend = end();
            if (offset) { // avoids dead stores
                setBegin(capacityBegin); // undo prepend optimization

                // By construction, the following loop is nothrow!
                // (otherwise, we can't reach here)
                // Assumes InputIterator operations don't throw.
                // (but we can't statically assert that, as these operations
                //  have preconditons, so typically aren't noexcept)
                while (true) {
                    if (dst == prependBufferEnd) {  // ran out of prepend buffer space
                        size += offset;
                        // we now have a contiguous buffer, continue with the main loop:
                        break;
                    }
                    if (first == last) {            // ran out of elements to assign
                        std::destroy(prependBufferEnd, dend);
                        size = dst - begin();
                        return;
                    }
                    // construct element in prepend buffer
                    ::std::construct_at(dst, std::invoke(proj, *first));
                    ++dst;
                    ++first;
                }
            }

            while (true) {
                if (first == last) {    // ran out of elements to assign
                    std::destroy(dst, dend);
                    break;
                }
                if (dst == dend) {      // ran out of existing elements to overwrite
                    if constexpr (IsFwdIt && IsIdentity) {
                        dst = std::uninitialized_copy(first, last, dst);
                        break;
                    }
                    else if constexpr (IsFwdIt && !IsIdentity
                        && std::is_nothrow_constructible_v<T, decltype(std::invoke(proj, *first))>) {
                        for (; first != last; ++dst, ++first)   // uninitialized_copy with projection
                            ::std::construct_at(dst, std::invoke(proj, *first));
                        break;
                    }
                    else {
                        do {
                            (*this)->emplace(size, std::invoke(proj, *first));
                        } while (++first != last);
                        return;         // size() is already correct (and dst invalidated)!
                    }
                }
                *dst = std::invoke(proj, *first);    // overwrite existing element
                ++dst;
                ++first;
            }
            size = dst - begin();
        }

        ArrayDataPointer sliced(sizetype pos, sizetype n) const&
        {
            ArrayDataPointer result(n);
            std::uninitialized_copy_n(begin() + pos, n, result.begin());
            result.size = n;
            return result;
        }

        ArrayDataPointer sliced(sizetype pos, sizetype n)&&
        {
            if (needsDetach())
                return sliced(pos, n);
            T* newBeginning = begin() + pos;
            std::destroy(begin(), newBeginning);
            std::destroy(newBeginning + n, end());
            setBegin(newBeginning);
            size = n;
            return std::move(*this);
        }

        // forwards from ArrayData
        sizetype allocatedCapacity() noexcept { return d ? d->allocatedCapacity() : 0; }
        sizetype constAllocatedCapacity() const noexcept { return d ? d->constAllocatedCapacity() : 0; }
        void ref() noexcept { if (d) d->ref(); }
        bool deref() noexcept { return !d || d->deref(); }
        bool isMutable() const noexcept { return d; }
        bool isShared() const noexcept { return !d || d->isShared(); }
        bool isSharedWith(const ArrayDataPointer& other) const noexcept { return d && d == other.d; }
        bool needsDetach() const noexcept { return !d || d->needsDetach(); }
        sizetype detachCapacity(sizetype newSize) const noexcept { return d ? d->detachCapacity(newSize) : newSize; }
        const typename Data::ArrayOptions flags() const noexcept { return d ? d->flags : Data::ArrayOptionDefault; }
        void setFlag(typename Data::ArrayOptions f) noexcept { Assert(d); d->flags |= f; }
        void clearFlag(typename Data::ArrayOptions f) noexcept { if (d) d->flags &= ~f; }

        Data* d_ptr() noexcept { return d; }
        void setBegin(T* begin) noexcept { ptr = begin; }

        sizetype freeSpaceAtBegin() const noexcept
        {
            if (d == nullptr)
                return 0;
            return this->ptr - Data::dataStart(d, alignof(typename Data::AlignmentDummy));
        }

        sizetype freeSpaceAtEnd() const noexcept
        {
            if (d == nullptr)
                return 0;
            return d->constAllocatedCapacity() - freeSpaceAtBegin() - this->size;
        }


        static ArrayDataPointer allocateGrow(const ArrayDataPointer& from, sizetype n, ArrayData::GrowthPosition position)
        {
            sizetype minimalCapacity = std::max(from.size, from.constAllocatedCapacity()) + n;

            minimalCapacity -= (position == ArrayData::GrowsAtEnd) ? from.freeSpaceAtEnd() : from.freeSpaceAtBegin();
            sizetype capacity = from.detachCapacity(minimalCapacity);
            const bool grows = capacity > from.constAllocatedCapacity();
            auto [header, dataPtr] = Data::allocate(capacity, grows ? ArrayData::Grow : ArrayData::KeepSize);
            const bool valid = header != nullptr && dataPtr != nullptr;
            if (!valid)
                return ArrayDataPointer(header, dataPtr);

            // Idea: * when growing backwards, adjust pointer to prepare free space at the beginning
            //       * when growing forward, adjust by the previous data pointer offset
            dataPtr += (position == ArrayData::GrowsAtBeginning)
                ? n + qMax(0, (header->alloc - from.size - n) / 2)
                : from.freeSpaceAtBegin();
            header->flags = from.flags();
            return ArrayDataPointer(header, dataPtr);
        }

        friend bool operator==(const ArrayDataPointer& lhs, const ArrayDataPointer& rhs) noexcept
        {
            return lhs.data() == rhs.data() && lhs.size == rhs.size;
        }

        friend bool operator!=(const ArrayDataPointer& lhs, const ArrayDataPointer& rhs) noexcept
        {
            return lhs.data() != rhs.data() || lhs.size != rhs.size;
        }

        Data* d;
        T* ptr;
        sizetype size;
    };

    template <class T>
    inline void swap(ArrayDataPointer<T>& p1, ArrayDataPointer<T>& p2) noexcept
    {
        p1.swap(p2);
    }


#define ARRAY_LITERAL(Type, ...) \
    ([]() -> ArrayDataPointer<Type> { \
        static Type const data[] = { __VA_ARGS__ }; \
        return ArrayDataPointer<Type>::fromRawData(const_cast<Type *>(data), std::size(data)); \
    }())
    /**/

} // namespace base::io
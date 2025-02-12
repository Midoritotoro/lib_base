#pragma once 

#include <base/utility/TypeInfo.h>
#include <base/Assert.h>

#include <base/io/ArrayDataPointer.h>


namespace base {
    template <typename T>
    struct ArrayDataPointer;

    template <class T>
    struct GenericArrayOps
        : public ArrayDataPointer<T>
    {
        static_assert (std::is_nothrow_destructible_v<T>);

    protected:
        typedef TypedArrayData<T> Data;
        using DataPointer = ArrayDataPointer<T>;

    public:
        typedef typename ArrayDataPointer<T>::parameter_type parameter_type;

        void appendInitialize(sizetype newSize)
        {
            Assert(this->isMutable());
            Assert(!this->isShared());
            Assert(newSize > this->size);
            Assert(newSize - this->size <= this->freeSpaceAtEnd());

            T* const b = this->begin();
            do {
                new (b + this->size) T;
            } while (++this->size != newSize);
        }

        void copyAppend(const T* b, const T* e)
        {
            Assert(this->isMutable() || b == e);
            Assert(!this->isShared() || b == e);
            Assert(b <= e);
            Assert((e - b) <= this->freeSpaceAtEnd());

            if (b == e)
                return;

            T* data = this->begin();
            while (b < e) {
                new (data + this->size) T(*b);
                ++b;
                ++this->size;
            }
        }

        void copyAppend(sizetype n, parameter_type t)
        {
            Assert(!this->isShared() || n == 0);
            Assert(this->freeSpaceAtEnd() >= n);
            if (!n)
                return;

            T* data = this->begin();
            while (n--) {
                new (data + this->size) T(t);
                ++this->size;
            }
        }

        void moveAppend(T* b, T* e)
        {
            Assert(this->isMutable() || b == e);
            Assert(!this->isShared() || b == e);
            Assert(b <= e);
            Assert((e - b) <= this->freeSpaceAtEnd());

            if (b == e)
                return;

            T* data = this->begin();
            while (b < e) {
                new (data + this->size) T(std::move(*b));
                ++b;
                ++this->size;
            }
        }

        void truncate(size_t newSize)
        {
            Assert(this->isMutable());
            Assert(!this->isShared());
            Assert(newSize < size_t(this->size));

            std::destroy(this->begin() + newSize, this->end());
            this->size = newSize;
        }

        void destroyAll()
        {
            Assert(this->d);
            Assert(this->d->ref_.loadRelaxed() == 0);

            std::destroy(this->begin(), this->end());
        }

        struct Inserter
        {
            ArrayDataPointer<T>* data;
            T* begin;
            sizetype size;

            sizetype sourceCopyConstruct = 0, nSource = 0, move = 0, sourceCopyAssign = 0;
            T* end = nullptr, * last = nullptr, * where = nullptr;

            Inserter(ArrayDataPointer<T>* d) : data(d)
            {
                begin = d->ptr;
                size = d->size;
            }
            ~Inserter() {
                data->ptr = begin;
                data->size = size;
            }
            DISABLE_COPY(Inserter)

                void setup(sizetype pos, sizetype n)
            {
                end = begin + size;
                last = end - 1;
                where = begin + pos;
                sizetype dist = size - pos;
                sourceCopyConstruct = 0;
                nSource = n;
                move = n - dist;
                sourceCopyAssign = n;
                if (n > dist) {
                    sourceCopyConstruct = n - dist;
                    move = 0;
                    sourceCopyAssign -= sourceCopyConstruct;
                }
            }

            void insert(sizetype pos, const T* source, sizetype n)
            {
                sizetype oldSize = size;
                unused(oldSize);

                setup(pos, n);

                for (sizetype i = 0; i != sourceCopyConstruct; ++i) {
                    new (end + i) T(source[nSource - sourceCopyConstruct + i]);
                    ++size;
                }
                Assert(size <= oldSize + n);

                for (sizetype i = sourceCopyConstruct; i != nSource; ++i) {
                    new (end + i) T(std::move(*(end + i - nSource)));
                    ++size;
                }

                Assert(size == oldSize + n);

                for (sizetype i = 0; i != move; --i)
                    last[i] = std::move(last[i - nSource]);

                for (sizetype i = 0; i != sourceCopyAssign; ++i)
                    where[i] = source[i];
            }

            void insert(sizetype pos, const T& t, sizetype n)
            {
                const sizetype oldSize = size;
                unused(oldSize);

                setup(pos, n);

                for (sizetype i = 0; i != sourceCopyConstruct; ++i) {
                    new (end + i) T(t);
                    ++size;
                }
                Assert(size <= oldSize + n);

                for (sizetype i = sourceCopyConstruct; i != nSource; ++i) {
                    new (end + i) T(std::move(*(end + i - nSource)));
                    ++size;
                }

                Assert(size == oldSize + n);

                for (sizetype i = 0; i != move; --i)
                    last[i] = std::move(last[i - nSource]);


                for (sizetype i = 0; i != sourceCopyAssign; ++i)
                    where[i] = t;
            }

            void insertOne(
                sizetype pos, T&& t)
            {
                setup(pos, 1);

                if (sourceCopyConstruct) {
                    Assert(sourceCopyConstruct == 1);
                    new (end) T(std::move(t));
                    ++size;
                }
                else {

                    new (end) T(std::move(*(end - 1)));
                    ++size;

                    for (sizetype i = 0; i != move; --i)
                        last[i] = std::move(last[i - 1]);

                    *where = std::move(t);
                }
            }
        };

        void insert(sizetype i, const T* data, sizetype n)
        {
            const bool growsAtBegin = this->size != 0 && i == 0;
            const auto pos = growsAtBegin ? Data::GrowsAtBeginning : Data::GrowsAtEnd;

            DataPointer oldData;
            this->detachAndGrow(pos, n, &data, &oldData);
            Assert((pos == Data::GrowsAtBeginning && this->freeSpaceAtBegin() >= n) ||
                (pos == Data::GrowsAtEnd && this->freeSpaceAtEnd() >= n));

            if (growsAtBegin) {

                Assert(this->freeSpaceAtBegin() >= n);
                while (n) {
                    --n;
                    new (this->begin() - 1) T(data[n]);
                    --this->ptr;
                    ++this->size;
                }
            }
            else {
                Inserter(this).insert(i, data, n);
            }
        }

        void insert(sizetype i, sizetype n, parameter_type t)
        {
            T copy(t);

            const bool growsAtBegin = this->size != 0 && i == 0;
            const auto pos = growsAtBegin ? Data::GrowsAtBeginning : Data::GrowsAtEnd;

            this->detachAndGrow(pos, n, nullptr, nullptr);
            Assert((pos == Data::GrowsAtBeginning && this->freeSpaceAtBegin() >= n) ||
                (pos == Data::GrowsAtEnd && this->freeSpaceAtEnd() >= n));

            if (growsAtBegin) {

                Assert(this->freeSpaceAtBegin() >= n);
                while (n--) {
                    new (this->begin() - 1) T(copy);
                    --this->ptr;
                    ++this->size;
                }
            }
            else {
                Inserter(this).insert(i, copy, n);
            }
        }

        template<typename... Args>
        void emplace(sizetype i, Args &&... args)
        {
            bool detach = this->needsDetach();
            if (!detach) {
                if (i == this->size && this->freeSpaceAtEnd()) {
                    new (this->end()) T(std::forward<Args>(args)...);
                    ++this->size;
                    return;
                }
                if (i == 0 && this->freeSpaceAtBegin()) {
                    new (this->begin() - 1) T(std::forward<Args>(args)...);
                    --this->ptr;
                    ++this->size;
                    return;
                }
            }
            T tmp(std::forward<Args>(args)...);
            const bool growsAtBegin = this->size != 0 && i == 0;
            const auto pos = growsAtBegin ? Data::GrowsAtBeginning : Data::GrowsAtEnd;

            this->detachAndGrow(pos, 1, nullptr, nullptr);

            if (growsAtBegin) {
                Assert(this->freeSpaceAtBegin());
                new (this->begin() - 1) T(std::move(tmp));
                --this->ptr;
                ++this->size;
            }
            else {
                Inserter(this).insertOne(i, std::move(tmp));
            }
        }

        void erase(T* b, sizetype n)
        {
            T* e = b + n;
            Assert(this->isMutable());
            Assert(b < e);
            Assert(b >= this->begin() && b < this->end());
            Assert(e > this->begin() && e <= this->end());

            if (b == this->begin() && e != this->end()) {
                this->ptr = e;
            }
            else {
                const T* const end = this->end();

                while (e != end) {
                    *b = std::move(*e);
                    ++b;
                    ++e;
                }
            }
            this->size -= n;
            std::destroy(b, e);
        }

        void eraseFirst() noexcept
        {
            Assert(this->isMutable());
            Assert(this->size);
            this->begin()->~T();
            ++this->ptr;
            --this->size;
        }

        void eraseLast() noexcept
        {
            Assert(this->isMutable());
            Assert(this->size);
            (this->end() - 1)->~T();
            --this->size;
        }


        void assign(T* b, T* e, parameter_type t)
        {
            Assert(b <= e);
            Assert(b >= this->begin() && e <= this->end());

            while (b != e)
                *b++ = t;
        }

        bool compare(const T* begin1, const T* begin2, size_t n) const
        {
            const T* end1 = begin1 + n;
            while (begin1 != end1) {
                if (*begin1 == *begin2) {
                    ++begin1;
                    ++begin2;
                }
                else {
                    return false;
                }
            }
            return true;
        }
    };

    template <class T>
    struct MovableArrayOps
        : GenericArrayOps<T>
    {
        static_assert (std::is_nothrow_destructible_v<T>, "");

    protected:
        typedef TypedArrayData<T> Data;
        using DataPointer = ArrayDataPointer<T>;

    public:
        typedef typename GenericArrayOps<T>::parameter_type parameter_type;

        struct Inserter
        {
            ArrayDataPointer<T>* data;
            T* displaceFrom;
            T* displaceTo;
            sizetype nInserts = 0;
            sizetype bytes;

            Inserter(ArrayDataPointer<T>* d) : data(d) { }
            ~Inserter() {
                if constexpr (!std::is_nothrow_copy_constructible_v<T>) {
                    if (displaceFrom != displaceTo) {
                        ::memmove(static_cast<void*>(displaceFrom), static_cast<void*>(displaceTo), bytes);
                        nInserts -= qAbs(displaceFrom - displaceTo);
                    }
                }
                data->size += nInserts;
            }
            DISABLE_COPY(Inserter)

                T* displace(sizetype pos, sizetype n)
            {
                nInserts = n;
                T* insertionPoint = data->ptr + pos;
                displaceFrom = data->ptr + pos;
                displaceTo = displaceFrom + n;
                bytes = data->size - pos;
                bytes *= sizeof(T);
                ::memmove(static_cast<void*>(displaceTo), static_cast<void*>(displaceFrom), bytes);
                return insertionPoint;
            }

            void insert(qsizetype pos, const T* source, qsizetype n)
            {
                T* where = displace(pos, n);

                while (n--) {
                    new (where) T(*source);
                    ++where;
                    ++source;
                    ++displaceFrom;
                }
            }

            void insert(qsizetype pos, const T& t, qsizetype n)
            {
                T* where = displace(pos, n);

                while (n--) {
                    new (where) T(t);
                    ++where;
                    ++displaceFrom;
                }
            }

            void insertOne(qsizetype pos, T&& t)
            {
                T* where = displace(pos, 1);
                new (where) T(std::move(t));
                ++displaceFrom;
                Q_ASSERT(displaceFrom == displaceTo);
            }

        };


        void insert(qsizetype i, const T* data, qsizetype n)
        {
            const bool growsAtBegin = this->size != 0 && i == 0;
            const auto pos = growsAtBegin ? Data::GrowsAtBeginning : Data::GrowsAtEnd;

            DataPointer oldData;
            this->detachAndGrow(pos, n, &data, &oldData);
            Q_ASSERT((pos == Data::GrowsAtBeginning && this->freeSpaceAtBegin() >= n) ||
                (pos == Data::GrowsAtEnd && this->freeSpaceAtEnd() >= n));

            if (growsAtBegin) {
                // copy construct items in reverse order at the begin
                Q_ASSERT(this->freeSpaceAtBegin() >= n);
                while (n) {
                    --n;
                    new (this->begin() - 1) T(data[n]);
                    --this->ptr;
                    ++this->size;
                }
            }
            else {
                Inserter(this).insert(i, data, n);
            }
        }

        void insert(qsizetype i, qsizetype n, parameter_type t)
        {
            T copy(t);

            const bool growsAtBegin = this->size != 0 && i == 0;
            const auto pos = growsAtBegin ? Data::GrowsAtBeginning : Data::GrowsAtEnd;

            this->detachAndGrow(pos, n, nullptr, nullptr);
            Q_ASSERT((pos == Data::GrowsAtBeginning && this->freeSpaceAtBegin() >= n) ||
                (pos == Data::GrowsAtEnd && this->freeSpaceAtEnd() >= n));

            if (growsAtBegin) {
                // copy construct items in reverse order at the begin
                Q_ASSERT(this->freeSpaceAtBegin() >= n);
                while (n--) {
                    new (this->begin() - 1) T(copy);
                    --this->ptr;
                    ++this->size;
                }
            }
            else {
                Inserter(this).insert(i, copy, n);
            }
        }

        template<typename... Args>
        void emplace(qsizetype i, Args &&... args)
        {
            bool detach = this->needsDetach();
            if (!detach) {
                if (i == this->size && this->freeSpaceAtEnd()) {
                    new (this->end()) T(std::forward<Args>(args)...);
                    ++this->size;
                    return;
                }
                if (i == 0 && this->freeSpaceAtBegin()) {
                    new (this->begin() - 1) T(std::forward<Args>(args)...);
                    --this->ptr;
                    ++this->size;
                    return;
                }
            }
            T tmp(std::forward<Args>(args)...);
            const bool growsAtBegin = this->size != 0 && i == 0;
            const auto pos = growsAtBegin ? Data::GrowsAtBeginning : Data::GrowsAtEnd;

            this->detachAndGrow(pos, 1, nullptr, nullptr);
            if (growsAtBegin) {
                Q_ASSERT(this->freeSpaceAtBegin());
                new (this->begin() - 1) T(std::move(tmp));
                --this->ptr;
                ++this->size;
            }
            else {
                Inserter(this).insertOne(i, std::move(tmp));
            }
        }

        void erase(T* b, sizetype n)
        {
            T* e = b + n;

            Assert(this->isMutable());
            Assert(b < e);
            Assert(b >= this->begin() && b < this->end());
            Assert(e > this->begin() && e <= this->end());

            // Comply with std::vector::erase(): erased elements and all after them
            // are invalidated. However, erasing from the beginning effectively
            // means that all iterators are invalidated. We can use this freedom to
            // erase by moving towards the end.

            std::destroy(b, e);
            if (b == this->begin() && e != this->end()) {
                this->ptr = e;
            }
            else if (e != this->end()) {
                memmove(static_cast<void*>(b), static_cast<const void*>(e), (static_cast<const T*>(this->end()) - e) * sizeof(T));
            }
            this->size -= n;
        }

        void reallocate(sizetype alloc, ArrayData::AllocationOption option)
        {
            auto pair = Data::reallocateUnaligned(this->d, this->ptr, alloc, option);
            Q_CHECK_PTR(pair.second);
            Q_ASSERT(pair.first != nullptr);
            this->d = pair.first;
            this->ptr = pair.second;
        }
    };

    template <class T>
    struct PodArrayOps
        : public ArrayDataPointer<T>
    {
        static_assert (std::is_nothrow_destructible_v<T>, "Types with throwing destructors are not supported in Qt containers.");

    protected:
        typedef TypedArrayData<T> Data;
        using DataPointer = ArrayDataPointer<T>;

    public:
        typedef typename ArrayDataPointer<T>::parameter_type parameter_type;

        using ArrayDataPointer<T>::ArrayDataPointer;

        void appendInitialize(sizetype newSize) noexcept
        {
            Assert(this->isMutable());
            Assert(!this->isShared());
            Assert(newSize > this->size);
            Assert(newSize - this->size <= this->freeSpaceAtEnd());

            T* where = this->end();
            this->size = newSize;
            const T* e = this->end();
            while (where != e)
                *where++ = T();
        }

        void copyAppend(const T* b, const T* e) noexcept
        {
            Assert(this->isMutable() || b == e);
            Assert(!this->isShared() || b == e);
            Assert(b <= e);
            Assert((e - b) <= this->freeSpaceAtEnd());

            if (b == e)
                return;

            ::memcpy(static_cast<void*>(this->end()), static_cast<const void*>(b), (e - b) * sizeof(T));
            this->size += (e - b);
        }

        void copyAppend(qsizetype n, parameter_type t) noexcept
        {
            Assert(!this->isShared() || n == 0);
            Assert(this->freeSpaceAtEnd() >= n);

            if (!n)
                return;

            T* where = this->end();
            this->size += qsizetype(n);
            while (n--)
                *where++ = t;
        }

        void moveAppend(T* b, T* e) noexcept
        {
            copyAppend(b, e);
        }

        void truncate(size_t newSize) noexcept
        {
            Assert(this->isMutable());
            Assert(!this->isShared());
            Assert(newSize < size_t(this->size));

            this->size = qsizetype(newSize);
        }

        void destroyAll() noexcept
        {
            Assert(this->d);
            Assert(this->d->ref_.loadRelaxed() == 0);
        }

        T* createHole(ArrayData::GrowthPosition pos, sizetype where, sizetype n)
        {
            Assert((pos == ArrayData::GrowsAtBeginning && n <= this->freeSpaceAtBegin()) ||
                (pos == ArrayData::GrowsAtEnd && n <= this->freeSpaceAtEnd()));

            T* insertionPoint = this->ptr + where;
            if (pos == ArrayData::GrowsAtEnd) {
                if (where < this->size)
                    ::memmove(static_cast<void*>(insertionPoint + n), static_cast<void*>(insertionPoint), (this->size - where) * sizeof(T));
            }
            else {
                Assert(where == 0);
                this->ptr -= n;
                insertionPoint -= n;
            }
            this->size += n;
            return insertionPoint;
        }

        void insert(sizetype i, const T* data, sizetype n)
        {
            typename Data::GrowthPosition pos = Data::GrowsAtEnd;
            if (this->size != 0 && i == 0)
                pos = Data::GrowsAtBeginning;

            DataPointer oldData;
            this->detachAndGrow(pos, n, &data, &oldData);
            Assert((pos == Data::GrowsAtBeginning && this->freeSpaceAtBegin() >= n) ||
                (pos == Data::GrowsAtEnd && this->freeSpaceAtEnd() >= n));

            T* where = createHole(pos, i, n);
            ::memcpy(static_cast<void*>(where), static_cast<const void*>(data), n * sizeof(T));
        }

        void insert(sizetype i, sizetype n, parameter_type t)
        {
            T copy(t);

            typename Data::GrowthPosition pos = Data::GrowsAtEnd;
            if (this->size != 0 && i == 0)
                pos = Data::GrowsAtBeginning;

            this->detachAndGrow(pos, n, nullptr, nullptr);
            Assert((pos == Data::GrowsAtBeginning && this->freeSpaceAtBegin() >= n) ||
                (pos == Data::GrowsAtEnd && this->freeSpaceAtEnd() >= n));

            T* where = createHole(pos, i, n);
            while (n--)
                *where++ = copy;
        }

        template<typename... Args>
        void emplace(sizetype i, Args &&... args)
        {
            bool detach = this->needsDetach();
            if (!detach) {
                if (i == this->size && this->freeSpaceAtEnd()) {
                    new (this->end()) T(std::forward<Args>(args)...);
                    ++this->size;
                    return;
                }
                if (i == 0 && this->freeSpaceAtBegin()) {
                    new (this->begin() - 1) T(std::forward<Args>(args)...);
                    --this->ptr;
                    ++this->size;
                    return;
                }
            }
            T tmp(std::forward<Args>(args)...);
            typename ArrayData::GrowthPosition pos = ArrayData::GrowsAtEnd;
            if (this->size != 0 && i == 0)
                pos = ArrayData::GrowsAtBeginning;

            this->detachAndGrow(pos, 1, nullptr, nullptr);

            T* where = createHole(pos, i, 1);
            new (where) T(std::move(tmp));
        }

        void erase(T* b, sizetype n)
        {
            T* e = b + n;

            Assert(this->isMutable());
            Assert(b < e);
            Assert(b >= this->begin() && b < this->end());
            Assert(e > this->begin() && e <= this->end());

            // Comply with std::vector::erase(): erased elements and all after them
            // are invalidated. However, erasing from the beginning effectively
            // means that all iterators are invalidated. We can use this freedom to
            // erase by moving towards the end.
            if (b == this->begin() && e != this->end()) {
                this->ptr = e;
            }
            else if (e != this->end()) {
                ::memmove(static_cast<void*>(b), static_cast<void*>(e),
                    (static_cast<T*>(this->end()) - e) * sizeof(T));
            }
            this->size -= n;
        }

        void eraseFirst() noexcept
        {
            Assert(this->isMutable());
            Assert(this->size);

            ++this->ptr;
            --this->size;
        }

        void eraseLast() noexcept
        {
            Assert(this->isMutable());
            Assert(this->size);

            --this->size;
        }

        template <typename Predicate>
        sizetype eraseIf(Predicate pred)
        {
            sizetype result = 0;
            if (this->size == 0)
                return result;

            if (!this->needsDetach()) {
                auto end = this->end();
                auto it = std::remove_if(this->begin(), end, pred);
                if (it != end) {
                    result = std::distance(it, end);
                    erase(it, result);
                }
            }
            else {
                const auto begin = this->begin();
                const auto end = this->end();
                auto it = std::find_if(begin, end, pred);
                if (it == end)
                    return result;

                QPodArrayOps<T> other(this->size);
                Assert(other.data() != nullptr);

                auto dest = other.begin();
                // std::uninitialized_copy will fallback to ::memcpy/memmove()
                dest = std::uninitialized_copy(begin, it, dest);
                dest = q_uninitialized_remove_copy_if(std::next(it), end, dest, pred);
                other.size = std::distance(other.data(), dest);
                result = this->size - other.size;
                this->swap(other);
            }
            return result;
        }

        struct Span { T* begin; T* end; };

        void copyRanges(std::initializer_list<Span> ranges)
        {
            auto it = this->begin();
            std::for_each(ranges.begin(), ranges.end(), [&it](const auto& span) {
                it = std::copy(span.begin, span.end, it);
                });
            this->size = std::distance(this->begin(), it);
        }

        void assign(T* b, T* e, parameter_type t) noexcept
        {
            Assert(b <= e);
            Assert(b >= this->begin() && e <= this->end());

            while (b != e)
                ::memcpy(static_cast<void*>(b++), static_cast<const void*>(&t), sizeof(T));
        }

        bool compare(const T* begin1, const T* begin2, size_t n) const
        {

            if constexpr (ArrayDataPointer<T>::pass_parameter_by_value) {
                return ::memcmp(begin1, begin2, n * sizeof(T)) == 0;
            }
            else {
                const T* end1 = begin1 + n;
                while (begin1 != end1) {
                    if (*begin1 == *begin2) {
                        ++begin1;
                        ++begin2;
                    }
                    else {
                        return false;
                    }
                }
                return true;
            }
        }

        void reallocate(sizetype alloc, ArrayData::AllocationOption option)
        {
            auto pair = Data::reallocateUnaligned(this->d, this->ptr, alloc, option);
            Assert(pair.second != nullptr);
            Assert(pair.first != nullptr);
            this->d = pair.first;
            this->ptr = pair.second;
        }
    };

    template <class T, class = void>
    struct ArrayOpsSelector
    {
        typedef GenericArrayOps<T> Type;
    };

    template <class T>
    struct ArrayOpsSelector<T,
        typename std::enable_if<
        !TypeInfo<T>::isComplex&& TypeInfo<T>::isRelocatable
    >::type>
    {
        typedef PodArrayOps<T> Type;
    };

    template <class T>
    struct ArrayOpsSelector<T,
        typename std::enable_if<
        TypeInfo<T>::isComplex&& TypeInfo<T>::isRelocatable
    >::type>
    {
        typedef QMovableArrayOps<T> Type;
    };

    template <class T>
    struct CommonArrayOps : ArrayOpsSelector<T>::Type
    {
        using Base = typename QArrayOpsSelector<T>::Type;
        using Data = QTypedArrayData<T>;
        using DataPointer = QArrayDataPointer<T>;
        using parameter_type = typename Base::parameter_type;

    protected:
        using Self = QCommonArrayOps<T>;

    public:
        template<typename It>
        void appendIteratorRange(It b, It e, QtPrivate::IfIsForwardIterator<It> = true)
        {
            Assert(this->isMutable() || b == e);
            Assert(!this->isShared() || b == e);
            const qsizetype distance = std::distance(b, e);
            Assert(distance >= 0 && distance <= this->allocatedCapacity() - this->size);
            unused(distance);

#if __cplusplus >= 202002L && defined(__cpp_concepts) && defined(__cpp_lib_concepts)
            constexpr bool canUseCopyAppend =
                std::contiguous_iterator<It> &&
                std::is_same_v<
                std::remove_cv_t<typename std::iterator_traits<It>::value_type>,
                T
                >;
            if constexpr (canUseCopyAppend) {
                this->copyAppend(std::to_address(b), std::to_address(e));
            }
            else
#endif
            {
                T* iter = this->end();
                for (; b != e; ++iter, ++b) {
                    new (iter) T(*b);
                    ++this->size;
                }
            }
        }

        // slightly higher level API than copyAppend() that also preallocates space
        void growAppend(const T* b, const T* e)
        {
            if (b == e)
                return;
            Assert(b < e);
            const qsizetype n = e - b;
            DataPointer old;

            // points into range:
            if (QtPrivate::q_points_into_range(b, *this))
                this->detachAndGrow(QArrayData::GrowsAtEnd, n, &b, &old);
            else
                this->detachAndGrow(QArrayData::GrowsAtEnd, n, nullptr, nullptr);
            Assert(this->freeSpaceAtEnd() >= n);
            // b might be updated so use [b, n)
            this->copyAppend(b, b + n);
        }
    };

    template <class T>
    struct QArrayDataOps
        : QtPrivate::QCommonArrayOps<T>
    {
    };

} // namespace base
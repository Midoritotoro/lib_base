#pragma once 

#include <base/core/system/SystemInfo.h>

#include <base/core/math/OverflowCheck.h>
#include <src/core/string/StringOptimization.h>

#include <src/core/string/StringStorageRefCount.h>

__BASE_STRING_NAMESPACE_BEGIN


enum class AcquireMallocatedString 
{};

template <
	class _Char_,
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _StorageOptimization_ = stringStorage::OptimizationSmallAndLarge>
class BasicStringStorage {
public:
    using CategoryType = uint8;

    enum class Category : CategoryType {
        isSmall     = 0,
        isMedium    = system::SystemInfo::IsLittleEndian() ? 0x80 : 0x2,
        isLarge     = system::SystemInfo::IsBigEndian() ? 0x40 : 0x1
    };

    struct MediumLarge {
        _Char_* _data;
        size_t _size;
        size_t _capacity;


        size_t capacity() const {
            return system::SystemInfo::IsLittleEndian()
                ? _capacity & capacityExtractMask
                : _capacity >> 2;
        }

        void setCapacity(
            size_t capacity,
            Category category)
        {
            _capacity = system::SystemInfo::IsLittleEndian()
                ? capacity | (static_cast<size_t>(category) << kCategoryShift)
                : (capacity << 2) | static_cast<size_t>(category);
        }
    };

    union {
        uint8		_bytes[sizeof(MediumLarge)]; // For accessing the last byte.
        _Char_		_small[sizeof(MediumLarge) / sizeof(_Char_)];
        MediumLarge _mediumLarge;
    };

    constexpr static size_t lastChar = sizeof(MediumLarge) - 1;
    constexpr static size_t maxSmallSize = lastChar / sizeof(_Char_);

    constexpr static size_t maxMediumSize = 254 / sizeof(_Char_);
    constexpr static uint8_t categoryExtractMask = system::SystemInfo::IsLittleEndian()
        ? 0xC0
        : 0x3;

    constexpr static size_t kCategoryShift = (sizeof(size_t) - 1) * 8;
    constexpr static size_t capacityExtractMask = system::SystemInfo::IsLittleEndian()
        ? ~(size_t(categoryExtractMask) << kCategoryShift)
        : 0x0 /* unused */;

    static_assert(
        !(sizeof(MediumLarge) % sizeof(_Char_)),
        "Corrupt memory layout.");

    size_t smallSize() const {
        Assert(category() == Category::isSmall);
        constexpr auto shift = system::SystemInfo::IsLittleEndian() ? 0 : 2;

        auto smallShifted = static_cast<size_t>(_small[maxSmallSize]) >> shift;
        Assert(static_cast<size_t>(maxSmallSize) >= smallShifted);

        return static_cast<size_t>(maxSmallSize) - smallShifted;
    }

    void setSmallSize(size_t s) {
        // Warning: this should work with uninitialized strings too,
        // so don't assume anything about the previous value of
        // _small[maxSmallSize].
        Assert(s <= maxSmallSize);
        constexpr auto shift = system::SystemInfo::IsLittleEndian() ? 0 : 2;

        _small[maxSmallSize] = char((maxSmallSize - s) << shift);
        _small[s] = '\0';

        Assert(category() == Category::isSmall && size() == s);
    }


    BasicStringStorage() noexcept {
        reset();
    }

    BasicStringStorage(const BasicStringStorage& rhs) {
        Assert(&rhs != this);

        switch (rhs.category()) {
        case Category::isSmall:
            copySmall(rhs);
            break;
        case Category::isMedium:
            copyMedium(rhs);
            break;
        case Category::isLarge:
            copyLarge(rhs);
            break;
        default:
            AssertUnreachable();
        }

        Assert(size() == rhs.size());
        Assert(memcmp(data(), rhs.data(), size() * sizeof(_Char_)) == 0);
    }

    BasicStringStorage& operator=(const BasicStringStorage& rhs) = delete;
    BasicStringStorage(BasicStringStorage&& goner) noexcept {
        // Take goner's guts
        _mediumLarge = goner._mediumLarge;
        // Clean goner's carcass
        goner.reset();
    }

    BasicStringStorage(
        const _Char_* const	data,
        const size_t		size)
    {
        initAny(data, size);
    }

    ~BasicStringStorage() noexcept {
        if (category() == Category::isSmall)
            return;

        destroyMediumLarge();
    }


    Category category() const {
        // works for both big-endian and little-endian
        return static_cast<Category>(_bytes[lastChar] & categoryExtractMask);
    }


    // ===============================================

    BasicStringStorage(
        _Char_* const data,
        const size_t size,
        const size_t allocatedSize,
        AcquireMallocatedString)
    {
        if (size > 0) {
            Assert(allocatedSize >= size + 1);
            Assert(data[size] == '\0');
            // Use the medium string storage
            _mediumLarge._data = data;
            _mediumLarge._size = size;
            // Don't forget about null terminator
            _mediumLarge.setCapacity(allocatedSize - 1, Category::isMedium);
        }
        else {
            // No need for the memory
            free(data);
            reset();
        }
    }

    // swap below doesn't test whether &rhs == this (and instead
    // potentially does extra work) on the premise that the rarity of
    // that situation actually makes the check more expensive than is
    // worth.
    void swap(BasicStringStorage& rhs) {
        auto const t = _mediumLarge;
        _mediumLarge = rhs._mediumLarge;

        rhs._mediumLarge = t;
    }

    // In C++11 data() and c_str() are 100% equivalent.
    const _Char_* data() const noexcept {
        return c_str();
    }

    _Char_* data() noexcept {
        return c_str();
    }

    _Char_* mutableData() {
        switch (category()) {
        case Category::isSmall:
            return _small;
        case Category::isMedium:
            return _mediumLarge._data;
        case Category::isLarge:
            return mutableDataLarge();
        }

        AssertUnreachable();
    }

    const _Char_* c_str() const noexcept {
        const _Char_* ptr = _mediumLarge._data;
        // With this syntax, GCC and Clang generate a CMOV instead of a branch.
        ptr = (category() == Category::isSmall)
            ? _small
            : ptr;

        return ptr;
    }
    
    _Char_* c_str() noexcept {
        _Char_* ptr = _mediumLarge._data;
        // With this syntax, GCC and Clang generate a CMOV instead of a branch.
        ptr = (category() == Category::isSmall)
            ? _small
            : ptr;

        return ptr;
    }

    void shrink(const size_t delta) {
        if (category() == Category::isSmall)
            shrinkSmall(delta);
        else if (category() == Category::isMedium
            || RefCounted<_Char_>::refs(_mediumLarge._data) == 1
            )
            shrinkMedium(delta);
        else
            shrinkLarge(delta);
    }

    void reserve(size_t minCapacity) {
        switch (category()) {
        case Category::isSmall:
            reserveSmall(minCapacity);
            break;
        case Category::isMedium:
            reserveMedium(minCapacity);
            break;
        case Category::isLarge:
            reserveLarge(minCapacity);
            break;
        default:
            AssertUnreachable();
        }

        Assert(capacity() >= minCapacity);
    }

    Char* expandNoinit(
        const size_t delta,
        bool expGrowth = false);

    void push_back(_Char_ c) {
        *expandNoinit(1, /* expGrowth = */ true) = c;
    }

    size_t size() const {
        size_t ret = _mediumLarge._size;

        if constexpr (system::SystemInfo::IsLittleEndian()) {
            // We can save a couple instructions, because the category is
            // small iff the last char, as unsigned, is <= maxSmallSize.
            using UChar = typename std::make_unsigned<_Char_>::type;

            auto maybeSmallSize = size_t(maxSmallSize) -
                size_t(static_cast<UChar>(_small[maxSmallSize]));

            // With this syntax, GCC and Clang generate a CMOV instead of a branch.
            ret = (static_cast<ptrdiff_t>(maybeSmallSize) >= 0)
                ? maybeSmallSize
                : ret;
        }
        else
            ret = (category() == Category::isSmall)
            ? smallSize()
            : ret;

        return ret;
    }

    size_t capacity() const {
        switch (category()) {
            case Category::isSmall:
                return maxSmallSize;
            case Category::isLarge:
                // For large-sized strings, a multi-referenced chunk has no
                // available capacity. This is because any attempt to append
                // data would trigger a new allocation.
                if (RefCounted<_Char_>::refs(_mediumLarge._data) > 1)
                    return _mediumLarge._size;

                break;
            case Category::isMedium:
            default:
                break;
        }

        return _mediumLarge.capacity();
    }

    bool isShared() const {
        return category() == Category::isLarge
            && RefCounted<_Char_>::refs(_mediumLarge._data) > 1;
    }

    void initAny(
        const _Char_* const   data,
        const size_t        size);
    void initSmall(
        const _Char_* const data,
        const size_t size);

    void initMedium(
        const _Char_* const data,
        const size_t size);
    void initLarge(
        const _Char_* const data,
        const size_t size);
private:
    void reset() {
        setSmallSize(0);
    }

    void destroyMediumLarge() noexcept {
        auto const c = category();
        Assert(c != Category::isSmall);

        if (c == Category::isMedium)
            free(_mediumLarge._data);
        else
            RefCounted<_Char_>::decrementRefs(_mediumLarge._data);
    }

    void copySmall(const BasicStringStorage& rhs);
    void copyMedium(const BasicStringStorage& rhs);
    void copyLarge(const BasicStringStorage& rhs);

    void reserveSmall(size_t minCapacity);

    void reserveMedium(size_t minCapacity);
    void reserveLarge(size_t minCapacity);

    void shrinkSmall(size_t delta);
    void shrinkMedium(size_t delta);
    void shrinkLarge(size_t delta);

    void unshare(size_t minCapacity = 0);
    _Char_* mutableDataLarge();
};

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::initAny(
    const _Char_* const   data,
    const size_t        size)
{
    if (size <= maxSmallSize)
        initSmall(data, size);
    else if (size <= maxMediumSize)
        initMedium(data, size);
    else
        initLarge(data, size);

    Assert(this->size() == size);
    Assert(size == 0 || memcmp(this->data(), data, size * sizeof(_Char_)) == 0);
}

// Small strings are bitblitted
template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::initSmall(
    const _Char_* const   data,
    const size_t        size)
{
    // Layout is: Char* data_, size_t size_, size_t capacity_
    static_assert(
        sizeof(*this) == sizeof(_Char_*) + 2 * sizeof(size_t),
        "BasicString has unexpected size");
    static_assert(
        sizeof(_Char_*) == sizeof(size_t), "fbstring size assumption violation");
    // sizeof(size_t) must be a power of 2
    static_assert(
        (sizeof(size_t) & (sizeof(size_t) - 1)) == 0,
        "BasicString size assumption violation");

    constexpr size_t kPageSize = 4096;
    const auto addr = reinterpret_cast<uintptr_t>(data);

    if (size && (addr ^ (addr + sizeof(_small) - 1)) < kPageSize)
        // the input data is all within one page so over-reads will not segfault
        std::memcpy(_small, data, sizeof(_small)); // lowers to a 4-insn sequence
    else if (size != 0)
        podCopy(data, data + size, _small);

    setSmallSize(size);
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::initMedium(
    const _Char_* const   data,
    const size_t        size)
{
    // Medium strings are allocated normally. Don't forget to
    // allocate one extra Char for the terminating null.
    auto const allocSize = (1 + size) * sizeof(_Char_);

    _mediumLarge._data = static_cast<_Char_*>(malloc(allocSize));

    if (size > 0)
        podCopy(data, data + size, _mediumLarge._data);

    _mediumLarge._size = size;
    _mediumLarge.setCapacity(allocSize / sizeof(_Char_) - 1, Category::isMedium);

    _mediumLarge._data[size] = '\0';
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::initLarge(
    const _Char_* const   data,
    const size_t        size)
{
    // Large strings are allocated differently
    size_t effectiveCapacity = size;

    auto const newRC = RefCounted<_Char_>::create(data, &effectiveCapacity);
    _mediumLarge._data = newRC->_data;

    _mediumLarge._size = size;
    _mediumLarge.setCapacity(effectiveCapacity, Category::isLarge);

    _mediumLarge._data[size] = '\0';
}


template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::copySmall(const BasicStringStorage& rhs) {
    static_assert(offsetof(MediumLarge, _data) == 0, "layout failure");

    static_assert(
        offsetof(MediumLarge, _size) == sizeof(_mediumLarge._data),
        "layout failure");

    static_assert(
        offsetof(MediumLarge, _capacity) == 2 * sizeof(_mediumLarge._data),
        "layout failure");

    // Just write the whole thing, don't look at details. In
    // particular we need to copy capacity anyway because we want
    // to set the size (don't forget that the last character,
    // which stores a short string's length, is shared with the
    // _mediumLarge.capacity field).

    _mediumLarge = rhs._mediumLarge;
    Assert(category() == Category::isSmall && this->size() == rhs.size());
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::copyMedium(const BasicStringStorage& rhs) {
    // Medium strings are copied eagerly. Don't forget to allocate
    // one extra Char for the null terminator.
    auto const allocSize = (1 + rhs._mediumLarge._size) * sizeof(_Char_);
    _mediumLarge._data = static_cast<_Char_*>(malloc(allocSize));
    // Also copies terminator.

    podCopy(
        rhs._mediumLarge._data, rhs._mediumLarge._data 
        + rhs._mediumLarge._size + 1, _mediumLarge._data);

    _mediumLarge._size = rhs._mediumLarge._size;
    _mediumLarge.setCapacity(allocSize / sizeof(_Char_) - 1, Category::isMedium);

    Assert(category() == Category::isMedium);
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::copyLarge(const BasicStringStorage& rhs) {
    // Large strings are just refcounted
    _mediumLarge = rhs._mediumLarge;
    RefCounted<_Char_>::incrementRefs(_mediumLarge._data);

    Assert(category() == Category::isLarge && size() == rhs.size());
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::unshare(size_t minCapacity) {
    Assert(category() == Category::isLarge);
    size_t effectiveCapacity = std::max(minCapacity, _mediumLarge.capacity());

    auto const newRC = RefCounted<_Char_>::create(&effectiveCapacity);
    // If this fails, someone placed the wrong capacity in an
    // fbstring.

    Assert(effectiveCapacity >= _mediumLarge.capacity());
    // Also copies terminator.

    podCopy(_mediumLarge._data, _mediumLarge._data + _mediumLarge._size + 1, newRC->_data);
    RefCounted<_Char_>::decrementRefs(_mediumLarge._data);

    _mediumLarge._data = newRC->_data;
    _mediumLarge.setCapacity(effectiveCapacity, Category::isLarge);
    // size_ remains unchanged.
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
_Char_* BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::mutableDataLarge() {
    Assert(category() == Category::isLarge);

    if (RefCounted<_Char_>::refs(_mediumLarge._data) > 1) // Ensure unique.
        unshare();
  
    return _mediumLarge._data;
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::reserveLarge(size_t minCapacity) {
    Assert(category() == Category::isLarge);

    if (RefCounted<_Char_>::refs(_mediumLarge._data) > 1) {
        // Ensure unique
        // We must make it unique regardless; in-place reallocation is
        // useless if the string is shared. In order to not surprise
        // people, reserve the new block at current capacity or
        // more. That way, a string's capacity never shrinks after a
        // call to reserve.
        unshare(minCapacity);
    } else {
    // String is not shared, so let's try to realloc (if needed)
        if (minCapacity > _mediumLarge.capacity()) {
            // Asking for more memory
            auto const newRC = RefCounted<_Char_>::reallocate(
                _mediumLarge._data, _mediumLarge._size, _mediumLarge.capacity(), &minCapacity);

            _mediumLarge._data = newRC->_data;
            _mediumLarge.setCapacity(minCapacity, Category::isLarge);
        }
        Assert(capacity() >= minCapacity);
    }
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::reserveMedium(
    const size_t minCapacity) 
{
    Assert(category() == Category::isMedium);
    // String is not shared

    if (minCapacity <= _mediumLarge.capacity())
        return; // nothing to do, there's enough room
    
    if (minCapacity <= maxMediumSize) {
        // Keep the string at medium size. Don't forget to allocate
        // one extra Char for the terminating null.
        size_t capacityBytes = (1 + minCapacity) * sizeof(_Char_);
        // Also copies terminator.

        _mediumLarge._data = static_cast<_Char_*>(smartRealloc(
            _mediumLarge._data,
            (_mediumLarge._size + 1) * sizeof(_Char_),
            (_mediumLarge.capacity() + 1) * sizeof(_Char_),
            capacityBytes));

        _mediumLarge.setCapacity(capacityBytes / sizeof(_Char_) - 1, Category::isMedium);
    } 
    else {
        // Conversion from medium to large string
        BasicStringStorage nascent;
        // Will recurse to another branch of this function

        nascent.reserve(minCapacity);
        nascent._mediumLarge._size = _mediumLarge._size;

        // Also copies terminator.
        podCopy(
            _mediumLarge._data, _mediumLarge._data + _mediumLarge._size + 1, nascent._mediumLarge._data);
        nascent.swap(*this);

        Assert(capacity() >= minCapacity);
    }
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::reserveSmall(size_t minCapacity) {
    Assert(category() == Category::isSmall);

    if (minCapacity <= maxSmallSize) {
        // small
        // Nothing to do, everything stays put
    } else if (minCapacity <= maxMediumSize) {
        // medium
        // Don't forget to allocate one extra Char for the terminating null
        auto const allocSizeBytes =
            (1 + minCapacity) * sizeof(_Char_);

        auto const pData = static_cast<_Char_*>(malloc(allocSizeBytes));
        auto const size = smallSize();

        // Also copies terminator.
        podCopy(_small, _small + size + 1, pData);

        _mediumLarge._data = pData;
        _mediumLarge._size = size;

        _mediumLarge.setCapacity(allocSizeBytes / sizeof(_Char_) - 1,
            Category::isMedium);
    } else {
        // large
        auto const newRC = RefCounted<_Char_>::create(&minCapacity);
        auto const size = smallSize();

        // Also copies terminator.
        podCopy(_small, _small + size + 1, newRC->_data);
        _mediumLarge._data = newRC->_data;

        _mediumLarge._size = size;
        _mediumLarge.setCapacity(minCapacity, Category::isLarge);

        Assert(capacity() >= minCapacity);
    }
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
Char* BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::expandNoinit(
    const size_t delta,
    bool expGrowth)
{
    // Strategy is simple: make room, then change size
    Assert(capacity() >= size());
    size_t sz, newSz;

    if (category() == Category::isSmall) {
        sz = smallSize();
        newSz = sz + delta;

        if (newSz <= maxSmallSize) {
            setSmallSize(newSz);
            return _small + sz;
        }

        reserveSmall(
            expGrowth ? std::max(newSz, 2 * maxSmallSize) : newSz);
    } 
    else {
        sz = _mediumLarge._size;
        newSz = sz + delta;

        if (newSz > capacity()) {
            // ensures not shared
            reserve(expGrowth ? std::max(newSz, 1 + capacity() * 3 / 2) : newSz);
        }
    }

    Assert(capacity() >= newSz);
    // Category can't be small - we took care of that above
    Assert(category() == Category::isMedium || category() == Category::isLarge);

    _mediumLarge._size = newSz;
    _mediumLarge._data[newSz] = '\0';

    Assert(size() == newSz);
    return _mediumLarge._data + sz;
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::shrinkSmall(const size_t delta) {
    // Check for underflow
    Assert(delta <= smallSize());
    setSmallSize(smallSize() - delta);
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::shrinkMedium(const size_t delta) {
    // Medium strings and unique large strings need no special
    // handling.
    Assert(_mediumLarge._size >= delta);

    _mediumLarge._size -= delta;
    _mediumLarge._data[_mediumLarge._size] = '\0';
}

template <
    class _Char_,
    class _SimdOptimization_,
    class _StorageOptimization_>
void BasicStringStorage<_Char_, _SimdOptimization_, _StorageOptimization_>::shrinkLarge(const size_t delta) {
    Assert(_mediumLarge._size >= delta);
    // Shared large string, must make unique. This is because of the
    // durn terminator must be written, which may trample the shared
    // data.
    if (delta)
        BasicStringStorage(_mediumLarge._data, _mediumLarge._size - delta).swap(*this);
    // No need to write the terminator.
}

__BASE_STRING_NAMESPACE_END

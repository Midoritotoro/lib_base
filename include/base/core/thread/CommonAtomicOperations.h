#pragma once

#include <base/core/thread/AtomicConfig.h>

__BASE_THREAD_NAMESPACE_BEGIN

template <typename AtomicType>
class AtomicOperations {
public:
    using Type = std::atomic<AtomicType>;

    template <typename T> 
    base_nodiscard static inline T load(const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    base_nodiscard static inline T load(const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    base_nodiscard static inline T loadRelaxed(const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    base_nodiscard static inline T loadRelaxed(const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    base_nodiscard static inline T loadAcquire(const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_acquire);
    }

    template <typename T>
    base_nodiscard static inline T loadAcquire(const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_acquire);
    }

    template <typename T>
    base_nodiscard static inline T loadSequentiallyConsistent(const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_seq_cst);
    }

    template <typename T>
    base_nodiscard static inline T loadSequentiallyConsistent(const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_seq_cst);
    }

    template <typename T>
    static inline void store(
        std::atomic<T>& _atomic_value, 
        T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_relaxed);
    }

    template <typename T>
    static inline void storeRelaxed(
        std::atomic<T>& _atomic_value, 
        T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_relaxed);
    }

    template <typename T>
    static inline void storeRelease(
            std::atomic<T>& _atomic_value,
            T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_release);
    }

    template <typename T>
    static inline void storeSequentiallyConsistent(
            std::atomic<T>& _atomic_value,
            T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_seq_cst);
    }

    base_nodiscard static inline bool isReferenceCountingNative() noexcept
    { 
        return isTestAndSetNative(); 
    }

    base_nodiscard static inline constexpr bool isReferenceCountingWaitFree() noexcept
    {
        return false;
    }

    template <typename T>
    base_nodiscard static inline bool ref(std::atomic<T>& _atomic_value)
    {
        return 
            (_atomic_value.fetch_add(
            1, std::memory_order_acq_rel) != T(-1));
    }

    template <typename T>
    base_nodiscard static inline bool deref(std::atomic<T>& _atomic_value) noexcept
    {
        // compare with ref
        return 
            (_atomic_value.fetch_sub(
            1, std::memory_order_acq_rel) != T(1));
    }

    base_nodiscard static inline bool isTestAndSetNative() noexcept
    {
        return AtomicTraits<sizeof(AtomicType)>::isLockFree();
    }

    base_nodiscard static inline constexpr bool isTestAndSetWaitFree() noexcept
    { 
        return false;
    }

    template <typename T>
    base_nodiscard static bool testAndSetRelaxed(
        std::atomic<T>& _atomic_value,
        T expectedValue, 
        T newValue, 
        T* currentValue = nullptr) noexcept
    {
        const auto temp = _atomic_value.compare_exchange_strong(
            expectedValue, newValue,
            std::memory_order_relaxed, std::memory_order_relaxed);

        if (currentValue)
            *currentValue = expectedValue;

        return temp;
    }

    template <typename T>
    base_nodiscard static bool testAndSetAcquire(
        std::atomic<T>& _atomic_value,
        T expectedValue, 
        T newValue, 
        T* currentValue = nullptr) noexcept
    {
        const auto temp = _atomic_value.compare_exchange_strong(
            expectedValue, newValue, 
            std::memory_order_acquire, std::memory_order_acquire);

        if (currentValue)
            *currentValue = expectedValue;

        return temp;
    }

    template <typename T>
    base_nodiscard static bool testAndSetRelease(
        std::atomic<T>& _atomic_value,
        T expectedValue, 
        T newValue, 
        T* currentValue = nullptr) noexcept
    {
        bool tmp = _atomic_value.compare_exchange_strong(
            expectedValue, newValue,
            std::memory_order_release, std::memory_order_relaxed);

        if (currentValue)
            *currentValue = expectedValue;

        return tmp;
    }

    template <typename T>
    base_nodiscard static bool testAndSetOrdered(
        std::atomic<T>& _atomic_value, 
        T expectedValue,
        T newValue,
        T* currentValue = nullptr) noexcept
    {
        bool tmp = _atomic_value.compare_exchange_strong(
            expectedValue, newValue,
            std::memory_order_acq_rel, std::memory_order_acquire);

        if (currentValue)
            *currentValue = expectedValue;

        return tmp;
    }

    base_nodiscard static inline bool isFetchAndStoreNative() noexcept {
        return isTestAndSetNative();
    }

    base_nodiscard static inline constexpr bool isFetchAndStoreWaitFree() noexcept
    {
        return false; 
    }

    template <typename T>
    base_nodiscard static T fetchAndStoreRelaxed(
        std::atomic<T>& _atomic_value,
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_relaxed);
    }

    template <typename T>
    base_nodiscard static T fetchAndStoreAcquire(
        std::atomic<T>& _atomic_value,
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_acquire);
    }

    template <typename T>
    base_nodiscard static T fetchAndStoreRelease(
        std::atomic<T>& _atomic_value, 
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_release);
    }

    template <typename T>
    base_nodiscard static T fetchAndStoreOrdered(
        std::atomic<T>& _atomic_value,
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_acq_rel);
    }

    base_nodiscard static inline bool isFetchAndAddNative() noexcept 
    { 
        return isTestAndSetNative();
    }

    base_nodiscard static inline constexpr bool isFetchAndAddWaitFree() noexcept
    { 
        return false; 
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndAddRelaxed(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndAddAcquire(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(valueToAdd, std::memory_order_acquire);
    }

    template <typename T>
    base_nodiscard static inline T fetchAndAddRelease(
        std::atomic<T>& _atomic_value,
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(valueToAdd, std::memory_order_release);
    }

    template <typename T>
    base_nodiscard static inline T fetchAndAddOrdered(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndSubRelaxed(
        std::atomic<T>& _atomic_value,
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    base_nodiscard static inline base_nodiscard T fetchAndSubAcquire(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(valueToAdd, std::memory_order_acquire);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndSubRelease(
        std::atomic<T>& _atomic_value,
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(valueToAdd, std::memory_order_release);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndSubOrdered(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndAndRelaxed(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndAndAcquire(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(valueToAdd, std::memory_order_acquire);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndAndRelease(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(valueToAdd, std::memory_order_release);
    }

    template <typename T>
    base_nodiscard static inline T fetchAndAndOrdered(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T>
    base_nodiscard static inline T fetchAndOrRelaxed(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndOrAcquire(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(valueToAdd, std::memory_order_acquire);
    }

    template <typename T>
    base_nodiscard static inline T fetchAndOrRelease(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(valueToAdd, std::memory_order_release);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndOrOrdered(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndXorRelaxed(
        std::atomic<T>& _atomic_value,
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(
            valueToAdd, std::memory_order_relaxed);
    }

    template <typename T>
    base_nodiscard static inline T fetchAndXorAcquire(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(valueToAdd, std::memory_order_acquire);
    }

    template <typename T> 
    base_nodiscard static inline T fetchAndXorRelease(
        std::atomic<T>& _atomic_value, 
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(valueToAdd, std::memory_order_release);
    }

    template <typename T>
    base_nodiscard static inline T fetchAndXorOrdered(
        std::atomic<T>& _atomic_value,
        typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(valueToAdd, std::memory_order_acq_rel);
    }
};

__BASE_THREAD_NAMESPACE_END

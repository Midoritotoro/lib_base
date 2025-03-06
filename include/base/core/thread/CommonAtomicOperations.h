#pragma once

#include <base/core/thread/AtomicConfig.h>

__BASE_THREAD_NAMESPACE_BEGIN

template <typename AtomicType>
class AtomicOperations {
public:
    using Type = std::atomic<AtomicType>;

    template <typename T> 
    static inline NODISCARD 
        T load(const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    static inline NODISCARD
        T load(const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    static inline NODISCARD
        T loadRelaxed(const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    static inline NODISCARD
        T loadRelaxed(const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_relaxed);
    }

    template <typename T>
    static inline NODISCARD
        T loadAcquire(const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_acquire);
    }

    template <typename T>
    static inline NODISCARD
        T loadAcquire(
            const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_acquire);
    }

    template <typename T>
    static inline NODISCARD
        T loadSequentiallyConsistent(
            const std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_seq_cst);
    }

    template <typename T>
    static inline NODISCARD
        T loadSequentiallyConsistent(
            const volatile std::atomic<T>& _atomic_value) noexcept
    {
        return _atomic_value.load(std::memory_order_seq_cst);
    }

    template <typename T>
    static inline NODISCARD
        void store(
            std::atomic<T>& _atomic_value, 
            T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_relaxed);
    }

    template <typename T>
    static inline NODISCARD
        void storeRelaxed(
            std::atomic<T>& _atomic_value, 
            T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_relaxed);
    }

    template <typename T>
    static inline NODISCARD
        void storeRelease(
            std::atomic<T>& _atomic_value,
            T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_release);
    }

    template <typename T>
    static inline NODISCARD
        void storeSequentiallyConsistent(
            std::atomic<T>& _atomic_value,
            T newValue) noexcept
    {
        _atomic_value.store(newValue, std::memory_order_seq_cst);
    }

    static inline NODISCARD
        bool isReferenceCountingNative() noexcept
    { 
        return isTestAndSetNative(); 
    }

    static inline constexpr NODISCARD
        bool isReferenceCountingWaitFree() noexcept 
    {
        return false;
    }

    template <typename T>
    static inline NODISCARD
        bool ref(std::atomic<T>& _atomic_value)
    {
        return 
            (_atomic_value.fetch_add(
            1, std::memory_order_acq_rel) != T(-1));
    }

    template <typename T>
    static inline NODISCARD 
        bool deref(std::atomic<T>& _atomic_value) noexcept
    {
        // compare with ref
        return 
            (_atomic_value.fetch_sub(
            1, std::memory_order_acq_rel) != T(1));
    }

    static inline NODISCARD 
        bool isTestAndSetNative() noexcept
    {
        return AtomicTraits<sizeof(AtomicType)>::isLockFree();
    }

    static inline constexpr
        bool isTestAndSetWaitFree() noexcept
    { 
        return false;
    }

    template <typename T>
    static NODISCARD bool testAndSetRelaxed(
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
    static NODISCARD bool testAndSetAcquire(
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
    static NODISCARD bool testAndSetRelease(
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
    static NODISCARD bool testAndSetOrdered(
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

    static inline NODISCARD bool isFetchAndStoreNative() noexcept { 
        return isTestAndSetNative();
    }

    static inline constexpr NODISCARD 
        bool isFetchAndStoreWaitFree() noexcept
    {
        return false; 
    }

    template <typename T>
    static NODISCARD T fetchAndStoreRelaxed(
        std::atomic<T>& _atomic_value,
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_relaxed);
    }

    template <typename T>
    static NODISCARD T fetchAndStoreAcquire(
        std::atomic<T>& _atomic_value,
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_acquire);
    }

    template <typename T>
    static NODISCARD T fetchAndStoreRelease(
        std::atomic<T>& _atomic_value, 
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_release);
    }

    template <typename T>
    static NODISCARD T fetchAndStoreOrdered(
        std::atomic<T>& _atomic_value,
        T newValue) noexcept
    {
        return _atomic_value.exchange(
            newValue, std::memory_order_acq_rel);
    }

    static inline NODISCARD bool 
        isFetchAndAddNative() noexcept 
    { 
        return isTestAndSetNative();
    }

    static inline constexpr NODISCARD
        bool isFetchAndAddWaitFree() noexcept
    { 
        return false; 
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndAddRelaxed(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(
            valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndAddAcquire(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(
            valueToAdd, std::memory_order_acquire);
    }

    template <typename T>
    static inline NODISCARD
        T fetchAndAddRelease(
            std::atomic<T>& _atomic_value,
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(
            valueToAdd, std::memory_order_release);
    }

    template <typename T>
    static inline NODISCARD
        T fetchAndAddOrdered(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_add(
            valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndSubRelaxed(
            std::atomic<T>& _atomic_value,
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(
            valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndSubAcquire(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(
            valueToAdd, std::memory_order_acquire);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndSubRelease(
            std::atomic<T>& _atomic_value,
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(
            valueToAdd, std::memory_order_release);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndSubOrdered(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_sub(
            valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndAndRelaxed(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(
            valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndAndAcquire(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(
            valueToAdd, std::memory_order_acquire);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndAndRelease(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(
            valueToAdd, std::memory_order_release);
    }

    template <typename T>
    static inline NODISCARD
        T fetchAndAndOrdered(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_and(
            valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T>
    static inline NODISCARD
        T fetchAndOrRelaxed(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(
            valueToAdd, std::memory_order_relaxed);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndOrAcquire(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(
            valueToAdd, std::memory_order_acquire);
    }

    template <typename T>
    static inline NODISCARD
        T fetchAndOrRelease(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(
            valueToAdd, std::memory_order_release);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndOrOrdered(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_or(
            valueToAdd, std::memory_order_acq_rel);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndXorRelaxed(
            std::atomic<T>& _atomic_value,
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(
            valueToAdd, std::memory_order_relaxed);
    }

    template <typename T>
    static inline NODISCARD
        T fetchAndXorAcquire(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(
            valueToAdd, std::memory_order_acquire);
    }

    template <typename T> 
    static inline NODISCARD
        T fetchAndXorRelease(
            std::atomic<T>& _atomic_value, 
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(
            valueToAdd, std::memory_order_release);
    }

    template <typename T>
    static inline NODISCARD
        T fetchAndXorOrdered(
            std::atomic<T>& _atomic_value,
            typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
    {
        return _atomic_value.fetch_xor(
            valueToAdd, std::memory_order_acq_rel);
    }
};

__BASE_THREAD_NAMESPACE_END

#pragma once

#include <base/core/AtomicConfig.h>
#include <base/system/Platform.h>

namespace base {
	template <typename AtomicType>
	class AtomicOperations {
	public:
        using Type = std::atomic<AtomicType>;

        template <typename T> static inline
            T load(const std::atomic<T>& _q_value) noexcept
        {
            return _q_value.load(std::memory_order_relaxed);
        }

        template <typename T> static inline
            T load(const volatile std::atomic<T>& _q_value) noexcept
        {
            return _q_value.load(std::memory_order_relaxed);
        }

        template <typename T> static inline
            T loadRelaxed(const std::atomic<T>& _q_value) noexcept
        {
            return _q_value.load(std::memory_order_relaxed);
        }

        template <typename T> static inline
            T loadRelaxed(const volatile std::atomic<T>& _q_value) noexcept
        {
            return _q_value.load(std::memory_order_relaxed);
        }

        template <typename T> static inline
            T loadAcquire(const std::atomic<T>& _q_value) noexcept
        {
            return _q_value.load(std::memory_order_acquire);
        }

        template <typename T> static inline
            T loadAcquire(const volatile std::atomic<T>& _q_value) noexcept
        {
            return _q_value.load(std::memory_order_acquire);
        }

        template <typename T> static inline
            void store(std::atomic<T>& _q_value, T newValue) noexcept
        {
            _q_value.store(newValue, std::memory_order_relaxed);
        }

        template <typename T> static inline
            void storeRelaxed(std::atomic<T>& _q_value, T newValue) noexcept
        {
            _q_value.store(newValue, std::memory_order_relaxed);
        }

        template <typename T> static inline
            void storeRelease(std::atomic<T>& _q_value, T newValue) noexcept
        {
            _q_value.store(newValue, std::memory_order_release);
        }

        static inline bool isReferenceCountingNative() noexcept { return isTestAndSetNative(); }
        static inline constexpr bool isReferenceCountingWaitFree() noexcept { return false; }
        template <typename T>
        static inline bool ref(std::atomic<T>& _q_value)
        {
            return _q_value.fetch_add(1, std::memory_order_acq_rel) != T(-1);
        }

        template <typename T>
        static inline bool deref(std::atomic<T>& _q_value) noexcept
        {
            // compare with ref
            return _q_value.fetch_sub(1, std::memory_order_acq_rel) != T(1);
        }

        static inline bool isTestAndSetNative() noexcept
        {
            return QAtomicTraits<sizeof(X)>::isLockFree();
        }

        static inline constexpr bool isTestAndSetWaitFree() noexcept { return false; }

        template <typename T>
        static bool testAndSetRelaxed(std::atomic<T>& _q_value, T expectedValue, T newValue, T* currentValue = nullptr) noexcept
        {
            bool tmp = _q_value.compare_exchange_strong(expectedValue, newValue, std::memory_order_relaxed, std::memory_order_relaxed);
            if (currentValue)
                *currentValue = expectedValue;
            return tmp;
        }

        template <typename T>
        static bool testAndSetAcquire(std::atomic<T>& _q_value, T expectedValue, T newValue, T* currentValue = nullptr) noexcept
        {
            bool tmp = _q_value.compare_exchange_strong(expectedValue, newValue, std::memory_order_acquire, std::memory_order_acquire);
            if (currentValue)
                *currentValue = expectedValue;
            return tmp;
        }

        template <typename T>
        static bool testAndSetRelease(std::atomic<T>& _q_value, T expectedValue, T newValue, T* currentValue = nullptr) noexcept
        {
            bool tmp = _q_value.compare_exchange_strong(expectedValue, newValue, std::memory_order_release, std::memory_order_relaxed);
            if (currentValue)
                *currentValue = expectedValue;
            return tmp;
        }

        template <typename T>
        static bool testAndSetOrdered(std::atomic<T>& _q_value, T expectedValue, T newValue, T* currentValue = nullptr) noexcept
        {
            bool tmp = _q_value.compare_exchange_strong(expectedValue, newValue, std::memory_order_acq_rel, std::memory_order_acquire);
            if (currentValue)
                *currentValue = expectedValue;
            return tmp;
        }

        static inline bool isFetchAndStoreNative() noexcept { return isTestAndSetNative(); }
        static inline constexpr bool isFetchAndStoreWaitFree() noexcept { return false; }

        template <typename T>
        static T fetchAndStoreRelaxed(std::atomic<T>& _q_value, T newValue) noexcept
        {
            return _q_value.exchange(newValue, std::memory_order_relaxed);
        }

        template <typename T>
        static T fetchAndStoreAcquire(std::atomic<T>& _q_value, T newValue) noexcept
        {
            return _q_value.exchange(newValue, std::memory_order_acquire);
        }

        template <typename T>
        static T fetchAndStoreRelease(std::atomic<T>& _q_value, T newValue) noexcept
        {
            return _q_value.exchange(newValue, std::memory_order_release);
        }

        template <typename T>
        static T fetchAndStoreOrdered(std::atomic<T>& _q_value, T newValue) noexcept
        {
            return _q_value.exchange(newValue, std::memory_order_acq_rel);
        }

        static inline bool isFetchAndAddNative() noexcept { return isTestAndSetNative(); }
        static inline constexpr bool isFetchAndAddWaitFree() noexcept { return false; }

        template <typename T> static inline
            T fetchAndAddRelaxed(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_add(valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> static inline
            T fetchAndAddAcquire(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_add(valueToAdd, std::memory_order_acquire);
        }

        template <typename T> static inline
            T fetchAndAddRelease(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_add(valueToAdd, std::memory_order_release);
        }

        template <typename T> static inline
            T fetchAndAddOrdered(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_add(valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T> static inline
            T fetchAndSubRelaxed(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_sub(valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> static inline
            T fetchAndSubAcquire(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_sub(valueToAdd, std::memory_order_acquire);
        }

        template <typename T> static inline
            T fetchAndSubRelease(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_sub(valueToAdd, std::memory_order_release);
        }

        template <typename T> static inline
            T fetchAndSubOrdered(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_sub(valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T> static inline
            T fetchAndAndRelaxed(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_and(valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> static inline
            T fetchAndAndAcquire(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_and(valueToAdd, std::memory_order_acquire);
        }

        template <typename T> static inline
            T fetchAndAndRelease(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_and(valueToAdd, std::memory_order_release);
        }

        template <typename T> static inline
            T fetchAndAndOrdered(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_and(valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T> static inline
            T fetchAndOrRelaxed(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_or(valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> static inline
            T fetchAndOrAcquire(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_or(valueToAdd, std::memory_order_acquire);
        }

        template <typename T> static inline
            T fetchAndOrRelease(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_or(valueToAdd, std::memory_order_release);
        }

        template <typename T> static inline
            T fetchAndOrOrdered(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_or(valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T> static inline
            T fetchAndXorRelaxed(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_xor(valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> static inline
            T fetchAndXorAcquire(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_xor(valueToAdd, std::memory_order_acquire);
        }

        template <typename T> static inline
            T fetchAndXorRelease(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_xor(valueToAdd, std::memory_order_release);
        }

        template <typename T> static inline
            T fetchAndXorOrdered(std::atomic<T>& _q_value, typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _q_value.fetch_xor(valueToAdd, std::memory_order_acq_rel);
        }
	};
} // namespace base
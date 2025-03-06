#pragma once

#include <base/core/AtomicConfig.h>
#include <base/system/Platform.h>

namespace base {
	template <typename AtomicType>
	class AtomicOperations {
	public:
        using Type = std::atomic<AtomicType>;

        template <typename T> 
        static inline [[nodiscard]] 
            T load(const std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_relaxed);
        }

        template <typename T>
        static inline [[nodiscard]]
            T load(const volatile std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_relaxed);
        }

        template <typename T>
        static inline [[nodiscard]]
            T loadRelaxed(const std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_relaxed);
        }

        template <typename T>
        static inline [[nodiscard]]
            T loadRelaxed(const volatile std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_relaxed);
        }

        template <typename T>
        static inline [[nodiscard]]
            T loadAcquire(const std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_acquire);
        }

        template <typename T>
        static inline [[nodiscard]]
            T loadAcquire(const volatile std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_acquire);
        }

        template <typename T>
        static inline [[nodiscard]]
            T loadSequentiallyConsistent(const std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_seq_cst);
        }

        template <typename T>
        static inline [[nodiscard]]
            T loadSequentiallyConsistent(const volatile std::atomic<T>& _atomic_value) noexcept
        {
            return _atomic_value.load(std::memory_order_seq_cst);
        }

        template <typename T>
        static inline [[nodiscard]]
            void store(
                std::atomic<T>& _atomic_value, 
                T newValue) noexcept
        {
            _atomic_value.store(newValue, std::memory_order_relaxed);
        }

        template <typename T>
        static inline [[nodiscard]]
            void storeRelaxed(
                std::atomic<T>& _atomic_value, 
                T newValue) noexcept
        {
            _atomic_value.store(newValue, std::memory_order_relaxed);
        }

        template <typename T>
        static inline [[nodiscard]]
            void storeRelease(
                std::atomic<T>& _atomic_value,
                T newValue) noexcept
        {
            _atomic_value.store(newValue, std::memory_order_release);
        }

        template <typename T>
        static inline [[nodiscard]]
            void storeSequentiallyConsistent(
                std::atomic<T>& _atomic_value,
                T newValue) noexcept
        {
            _atomic_value.store(newValue, std::memory_order_seq_cst);
        }

        static inline [[nodiscard]] bool isReferenceCountingNative() noexcept { 
            return isTestAndSetNative(); 
        }

        static inline constexpr [[nodiscard]] bool isReferenceCountingWaitFree() noexcept {
            return false;
        }

        template <typename T>
        static inline [[nodiscard]]
            bool ref(std::atomic<T>& _atomic_value)
        {
            return 
                (_atomic_value.fetch_add(
                1, std::memory_order_acq_rel) != T(-1));
        }

        template <typename T>
        static inline [[nodiscard]] 
            bool deref(std::atomic<T>& _atomic_value) noexcept
        {
            // compare with ref
            return 
                (_atomic_value.fetch_sub(
                1, std::memory_order_acq_rel) != T(1));
        }

        static inline [[nodiscard]] 
            bool isTestAndSetNative() noexcept
        {
            return AtomicTraits<sizeof(AtomicType)>::isLockFree();
        }

        static inline constexpr bool isTestAndSetWaitFree() noexcept { return false; }

        template <typename T>
        static [[nodiscard]] bool testAndSetRelaxed(
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
        static [[nodiscard]] bool testAndSetAcquire(
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
        static [[nodiscard]] bool testAndSetRelease(
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
        static [[nodiscard]] bool testAndSetOrdered(
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

        static inline [[nodiscard]] bool isFetchAndStoreNative() noexcept { 
            return isTestAndSetNative();
        }

        static inline constexpr [[nodiscard]] 
            bool isFetchAndStoreWaitFree() noexcept
        {
            return false; 
        }

        template <typename T>
        static [[nodiscard]] T fetchAndStoreRelaxed(
            std::atomic<T>& _atomic_value,
            T newValue) noexcept
        {
            return _atomic_value.exchange(
                newValue, std::memory_order_relaxed);
        }

        template <typename T>
        static [[nodiscard]] T fetchAndStoreAcquire(
            std::atomic<T>& _atomic_value,
            T newValue) noexcept
        {
            return _atomic_value.exchange(
                newValue, std::memory_order_acquire);
        }

        template <typename T>
        static [[nodiscard]] T fetchAndStoreRelease(
            std::atomic<T>& _atomic_value, 
            T newValue) noexcept
        {
            return _atomic_value.exchange(
                newValue, std::memory_order_release);
        }

        template <typename T>
        static [[nodiscard]] T fetchAndStoreOrdered(
            std::atomic<T>& _atomic_value,
            T newValue) noexcept
        {
            return _atomic_value.exchange(
                newValue, std::memory_order_acq_rel);
        }

        static inline [[nodiscard]] bool isFetchAndAddNative() noexcept { 
            return isTestAndSetNative();
        }

        static inline constexpr [[nodiscard]] bool isFetchAndAddWaitFree() noexcept { 
            return false; 
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndAddRelaxed(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_add(
                valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndAddAcquire(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_add(
                valueToAdd, std::memory_order_acquire);
        }

        template <typename T>
        static inline [[nodiscard]]
            T fetchAndAddRelease(
                std::atomic<T>& _atomic_value,
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_add(
                valueToAdd, std::memory_order_release);
        }

        template <typename T>
        static inline [[nodiscard]]
            T fetchAndAddOrdered(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_add(
                valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndSubRelaxed(
                std::atomic<T>& _atomic_value,
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_sub(
                valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndSubAcquire(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_sub(
                valueToAdd, std::memory_order_acquire);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndSubRelease(
                std::atomic<T>& _atomic_value,
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_sub(
                valueToAdd, std::memory_order_release);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndSubOrdered(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_sub(
                valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndAndRelaxed(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_and(
                valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndAndAcquire(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_and(
                valueToAdd, std::memory_order_acquire);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndAndRelease(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_and(
                valueToAdd, std::memory_order_release);
        }

        template <typename T>
        static inline [[nodiscard]]
            T fetchAndAndOrdered(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_and(
                valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T>
        static inline [[nodiscard]]
            T fetchAndOrRelaxed(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_or(
                valueToAdd, std::memory_order_relaxed);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndOrAcquire(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_or(
                valueToAdd, std::memory_order_acquire);
        }

        template <typename T>
        static inline [[nodiscard]]
            T fetchAndOrRelease(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_or(
                valueToAdd, std::memory_order_release);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndOrOrdered(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_or(
                valueToAdd, std::memory_order_acq_rel);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndXorRelaxed(
                std::atomic<T>& _atomic_value,
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_xor(
                valueToAdd, std::memory_order_relaxed);
        }

        template <typename T>
        static inline [[nodiscard]]
            T fetchAndXorAcquire(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_xor(
                valueToAdd, std::memory_order_acquire);
        }

        template <typename T> 
        static inline [[nodiscard]]
            T fetchAndXorRelease(
                std::atomic<T>& _atomic_value, 
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_xor(
                valueToAdd, std::memory_order_release);
        }

        template <typename T>
        static inline [[nodiscard]]
            T fetchAndXorOrdered(
                std::atomic<T>& _atomic_value,
                typename AtomicAdditiveType<T>::AdditiveT valueToAdd) noexcept
        {
            return _atomic_value.fetch_xor(
                valueToAdd, std::memory_order_acq_rel);
        }
	};
} // namespace base
#pragma once 

#include <base/system/Platform.h>
#include <base/core/AtomicConfig.h>

#include <base/core/CommonAtomicOperations.h>


namespace base {
	template <
		typename Integer = int,
		typename = std::enable_if<std::is_integral_v<Integer>>>
	class AtomicInteger {
        static_assert(AtomicOpsSupport<sizeof(Integer)>::IsSupported, 
            "Параметр шаблона - числовой тип с неподдерживаемым для данной платформы размером");
	public:
        using SelfAtomicOperations = AtomicOperations<Integer>;

        [[nodiscard]] Integer loadRelaxed() const noexcept {
            return SelfAtomicOperations::loadRelaxed(_atomic);
        }

        void storeRelaxed(Integer newValue) noexcept { 
            SelfAtomicOperations::storeRelaxed(_atomic, newValue);
        }

        [[nodiscard]] Integer loadAcquire() const noexcept { 
            return SelfAtomicOperations::loadAcquire(_atomic);
        }

        void storeRelease(Integer newValue) noexcept { 
            SelfAtomicOperations::storeRelease(_atomic, newValue);
        }

        operator Integer() const noexcept {
            return loadAcquire();
        }

        Integer operator=(Integer newValue) noexcept {
            storeRelease(newValue);
            return newValue; 
        }

        static constexpr [[nodiscard]] 
            bool isReferenceCountingNative() noexcept 
        {
            return SelfAtomicOperations::isReferenceCountingNative(); 
        }

        static constexpr [[nodiscard]] 
            bool isReferenceCountingWaitFree() noexcept 
        { 
                return SelfAtomicOperations::isReferenceCountingWaitFree();
        }

        [[nodiscard]] bool ref() noexcept { 
            return SelfAtomicOperations::ref(_atomic);
        }

        [[nodiscard]] bool deref() noexcept {
            return SelfAtomicOperations::deref(_atomic); 
        }

        static constexpr [[nodiscard]]
            bool isTestAndSetNative() noexcept 
        {
            return SelfAtomicOperations::isTestAndSetNative();
        }

        static constexpr [[nodiscard]] 
            bool isTestAndSetWaitFree() noexcept
        { 
            return SelfAtomicOperations::isTestAndSetWaitFree(); 
        }

        [[nodiscard]] bool testAndSetRelaxed(
           Integer expectedValue, 
           Integer newValue) noexcept
        {
            return SelfAtomicOperations::testAndSetRelaxed(
                _atomic, expectedValue, newValue);
        }

        [[nodiscard]] bool testAndSetAcquire(
           Integer expectedValue, 
           Integer newValue) noexcept
        {
            return SelfAtomicOperations::testAndSetAcquire(
                _atomic, expectedValue, newValue);
        }

        [[nodiscard]] bool testAndSetRelease(
            Integer expectedValue, 
            Integer newValue) noexcept
        {
            return SelfAtomicOperations::testAndSetRelease(
                _atomic, expectedValue, newValue);
        }

        [[nodiscard]] bool testAndSetOrdered(
            Integer expectedValue, 
            Integer newValue) noexcept
        {
            return SelfAtomicOperations::testAndSetOrdered(
                _atomic, expectedValue, newValue);
        }

        [[nodiscard]] bool testAndSetRelaxed(
            Integer expectedValue,
            Integer newValue,
            Integer& currentValue) noexcept
        {
            return SelfAtomicOperations::testAndSetRelaxed(
                _atomic, expectedValue, newValue, &currentValue);
        }

        [[nodiscard]] bool testAndSetAcquire(
            Integer expectedValue,
            Integer newValue,
            Integer& currentValue) noexcept
        {
            return SelfAtomicOperations::testAndSetAcquire(
                _atomic, expectedValue, newValue, &currentValue);
        }

        [[nodiscard]] bool testAndSetRelease(
            Integer expectedValue, 
            Integer newValue, 
            Integer& currentValue) noexcept
        {
            return SelfAtomicOperations::testAndSetRelease(
                _atomic, expectedValue, newValue, &currentValue);
        }

        [[nodiscard]] bool testAndSetOrdered(
            Integer expectedValue, 
            Integer newValue, 
            Integer& currentValue) noexcept
        {
            return SelfAtomicOperations::testAndSetOrdered(
                _atomic, expectedValue, newValue, &currentValue);
        }

        static constexpr [[nodiscard]]
            bool isFetchAndStoreNative() noexcept
        {
            return SelfAtomicOperations::isFetchAndStoreNative();
        }

        static constexpr [[nodiscard]]
            bool isFetchAndStoreWaitFree() noexcept 
        {
            return SelfAtomicOperations::isFetchAndStoreWaitFree(); 
        }

        [[nodiscard]] Integer fetchAndStoreRelaxed(Integer newValue) noexcept {
            return SelfAtomicOperations::fetchAndStoreRelaxed(
                _atomic, newValue);
        }

        [[nodiscard]] Integer fetchAndStoreAcquire(Integer newValue) noexcept {
            return SelfAtomicOperations::fetchAndStoreAcquire(
                _atomic, newValue);
        }

        [[nodiscard]] Integer fetchAndStoreRelease(Integer newValue) noexcept {
            return SelfAtomicOperations::fetchAndStoreRelease(
                _atomic, newValue);
        }

        [[nodiscard]] Integer fetchAndStoreOrdered(Integer newValue) noexcept {
            return SelfAtomicOperations::fetchAndStoreOrdered(
                _atomic, newValue);
        }

        static constexpr [[nodiscard]] 
            bool isFetchAndAddNative() noexcept
        {
            return SelfAtomicOperations::isFetchAndAddNative(); 
        }

        static constexpr [[nodiscard]] 
            bool isFetchAndAddWaitFree() noexcept 
        {
            return SelfAtomicOperations::isFetchAndAddWaitFree(); 
        }

        [[nodiscard]] Integer fetchAndAddRelaxed(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAddRelaxed(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndAddAcquire(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAddAcquire(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndAddRelease(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAddRelease(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndAddOrdered(T valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAddOrdered(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndSubRelaxed(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndSubRelaxed(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndSubAcquire(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndSubAcquire(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndSubRelease(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndSubRelease(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndSubOrdered(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndSubOrdered(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndAndRelaxed(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAndRelaxed(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndAndAcquire(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAndAcquire(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndAndRelease(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAndRelease(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndAndOrdered(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndAndOrdered(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndOrRelaxed(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndOrRelaxed(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndOrAcquire(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndOrAcquire(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndOrRelease(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndOrRelease(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndOrOrdered(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndOrOrdered(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndXorRelaxed(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndXorRelaxed(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndXorAcquire(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndXorAcquire(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndXorRelease(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndXorRelease(
                _atomic, valueToAdd);
        }

        [[nodiscard]] Integer fetchAndXorOrdered(Integer valueToAdd) noexcept {
            return SelfAtomicOperations::fetchAndXorOrdered(
                _atomic, valueToAdd);
        }

        Integer operator++() noexcept {
            return fetchAndAddOrdered(1) + 1;
        }

        Integer operator++(int) noexcept {
            return fetchAndAddOrdered(1);
        }

        Integer operator--() noexcept {
            return fetchAndSubOrdered(1) - 1;
        }

        Integer operator--(int) noexcept {
            return fetchAndSubOrdered(1);
        }

        Integer operator+=(Integer v) noexcept {
            return fetchAndAddOrdered(v) + v;
        }

        Integer operator-=(Integer v) noexcept {
            return fetchAndSubOrdered(v) - v;
        }

        Integer operator&=(Integer v) noexcept {
            return fetchAndAndOrdered(v) & v;
        }

        Integer operator|=(Integer v) noexcept {
            return fetchAndOrOrdered(v) | v;
        }

        Integer operator^=(Integer v) noexcept {
            return fetchAndXorOrdered(v) ^ v;
        }

        AtomicInteger() = default;
        constexpr AtomicInteger(Integer value) noexcept : 
            _atomic(value) 
        {}

        AtomicInteger(const AtomicInteger&) = delete;

        AtomicInteger& operator=(const AtomicInteger&) = delete;
        AtomicInteger& operator=(const AtomicInteger&) volatile = delete;
	private:
		std::atomic<Integer> _atomic;
    };
} // namespace base
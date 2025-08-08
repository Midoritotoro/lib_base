#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/thread/AtomicConfig.h>

#include <base/core/thread/CommonAtomicOperations.h>
#include <base/core/utility/Assert.h>


__BASE_THREAD_NAMESPACE_BEGIN

template <
	typename Integer = int,
	typename = std::enable_if<std::is_integral_v<Integer>>>
class AtomicInteger {
public:
    static_assert(AtomicOpsSupport<sizeof(Integer)>::IsSupported, "Template parameter is an integral of a size not supported on this platform. ");

    using SelfAtomicOperations = AtomicOperations<Integer>;

    base_nodiscard Integer loadRelaxed() const noexcept {
        return SelfAtomicOperations::loadRelaxed(_atomic);
    }

    base_nodiscard Integer loadSequentiallyConsistent() const noexcept {
        return SelfAtomicOperations::loadSequentiallyConsistent(_atomic);
    }

    void storeRelaxed(Integer newValue) noexcept { 
        SelfAtomicOperations::storeRelaxed(_atomic, newValue);
    }

    base_nodiscard Integer loadAcquire() const noexcept { 
        return SelfAtomicOperations::loadAcquire(_atomic);
    }

    void storeRelease(Integer newValue) noexcept { 
        SelfAtomicOperations::storeRelease(
            _atomic, newValue);
    }

    void storeSequentiallyConsistent(Integer newValue) noexcept {
        SelfAtomicOperations::storeSequentiallyConsistent(
            _atomic, newValue);
    }

    Integer operator=(Integer newValue) noexcept {
        storeRelease(newValue);
        return newValue; 
    }

    static constexpr base_nodiscard 
        bool isReferenceCountingNative() noexcept 
    {
        return SelfAtomicOperations::isReferenceCountingNative(); 
    }

    static constexpr base_nodiscard 
        bool isReferenceCountingWaitFree() noexcept 
    { 
        return SelfAtomicOperations::isReferenceCountingWaitFree();
    }

    base_nodiscard bool ref() noexcept { 
        return SelfAtomicOperations::ref(_atomic);
    }

    base_nodiscard bool deref() noexcept {
        return SelfAtomicOperations::deref(_atomic); 
    }

    static constexpr base_nodiscard
        bool isTestAndSetNative() noexcept 
    {
        return SelfAtomicOperations::isTestAndSetNative();
    }

    static constexpr base_nodiscard 
        bool isTestAndSetWaitFree() noexcept
    { 
        return SelfAtomicOperations::isTestAndSetWaitFree(); 
    }

    base_nodiscard bool testAndSetRelaxed(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelaxed(
            _atomic, expectedValue, newValue);
    }

    base_nodiscard bool testAndSetAcquire(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetAcquire(
            _atomic, expectedValue, newValue);
    }

    base_nodiscard bool testAndSetRelease(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelease(
            _atomic, expectedValue, newValue);
    }

    base_nodiscard bool testAndSetOrdered(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetOrdered(
            _atomic, expectedValue, newValue);
    }

    base_nodiscard bool testAndSetRelaxed(
        Integer expectedValue,
        Integer newValue,
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelaxed(
            _atomic, expectedValue, newValue, &currentValue);
    }

    base_nodiscard bool testAndSetAcquire(
        Integer expectedValue,
        Integer newValue,
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetAcquire(
            _atomic, expectedValue, newValue, &currentValue);
    }

    base_nodiscard bool testAndSetRelease(
        Integer expectedValue, 
        Integer newValue, 
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelease(
            _atomic, expectedValue, newValue, &currentValue);
    }

    base_nodiscard bool testAndSetOrdered(
        Integer expectedValue, 
        Integer newValue, 
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetOrdered(
            _atomic, expectedValue, newValue, &currentValue);
    }

    static constexpr base_nodiscard
        bool isFetchAndStoreNative() noexcept
    {
        return SelfAtomicOperations::isFetchAndStoreNative();
    }

    static constexpr base_nodiscard
        bool isFetchAndStoreWaitFree() noexcept 
    {
        return SelfAtomicOperations::isFetchAndStoreWaitFree(); 
    }

    base_nodiscard Integer fetchAndStoreRelaxed(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreRelaxed(
            _atomic, newValue);
    }

    base_nodiscard Integer fetchAndStoreAcquire(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreAcquire(
            _atomic, newValue);
    }

    base_nodiscard Integer fetchAndStoreRelease(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreRelease(
            _atomic, newValue);
    }

    base_nodiscard Integer fetchAndStoreOrdered(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreOrdered(
            _atomic, newValue);
    }

    static constexpr base_nodiscard 
        bool isFetchAndAddNative() noexcept
    {
        return SelfAtomicOperations::isFetchAndAddNative(); 
    }

    static constexpr base_nodiscard 
        bool isFetchAndAddWaitFree() noexcept 
    {
        return SelfAtomicOperations::isFetchAndAddWaitFree(); 
    }

    base_nodiscard Integer fetchAndAddRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddRelaxed(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndAddAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddAcquire(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndAddRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddRelease(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndAddOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddOrdered(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndSubRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubRelaxed(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndSubAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubAcquire(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndSubRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubRelease(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndSubOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubOrdered(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndAndRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndRelaxed(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndAndAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndAcquire(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndAndRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndRelease(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndAndOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndOrdered(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndOrRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrRelaxed(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndOrAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrAcquire(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndOrRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrRelease(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndOrOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrOrdered(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndXorRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorRelaxed(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndXorAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorAcquire(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndXorRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorRelease(
            _atomic, valueToAdd);
    }

    base_nodiscard Integer fetchAndXorOrdered(Integer valueToAdd) noexcept {
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

    operator Integer() const volatile noexcept {
        return loadSequentiallyConsistent();
    }

    operator Integer() const noexcept {
        return loadSequentiallyConsistent();
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

__BASE_THREAD_NAMESPACE_END

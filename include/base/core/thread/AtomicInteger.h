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

    NODISCARD Integer loadRelaxed() const noexcept {
        return SelfAtomicOperations::loadRelaxed(_atomic);
    }

    NODISCARD Integer loadSequentiallyConsistent() const noexcept {
        return SelfAtomicOperations::loadSequentiallyConsistent(_atomic);
    }

    void storeRelaxed(Integer newValue) noexcept { 
        SelfAtomicOperations::storeRelaxed(_atomic, newValue);
    }

    NODISCARD Integer loadAcquire() const noexcept { 
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

    static constexpr NODISCARD 
        bool isReferenceCountingNative() noexcept 
    {
        return SelfAtomicOperations::isReferenceCountingNative(); 
    }

    static constexpr NODISCARD 
        bool isReferenceCountingWaitFree() noexcept 
    { 
        return SelfAtomicOperations::isReferenceCountingWaitFree();
    }

    NODISCARD bool ref() noexcept { 
        return SelfAtomicOperations::ref(_atomic);
    }

    NODISCARD bool deref() noexcept {
        return SelfAtomicOperations::deref(_atomic); 
    }

    static constexpr NODISCARD
        bool isTestAndSetNative() noexcept 
    {
        return SelfAtomicOperations::isTestAndSetNative();
    }

    static constexpr NODISCARD 
        bool isTestAndSetWaitFree() noexcept
    { 
        return SelfAtomicOperations::isTestAndSetWaitFree(); 
    }

    NODISCARD bool testAndSetRelaxed(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelaxed(
            _atomic, expectedValue, newValue);
    }

    NODISCARD bool testAndSetAcquire(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetAcquire(
            _atomic, expectedValue, newValue);
    }

    NODISCARD bool testAndSetRelease(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelease(
            _atomic, expectedValue, newValue);
    }

    NODISCARD bool testAndSetOrdered(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetOrdered(
            _atomic, expectedValue, newValue);
    }

    NODISCARD bool testAndSetRelaxed(
        Integer expectedValue,
        Integer newValue,
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelaxed(
            _atomic, expectedValue, newValue, &currentValue);
    }

    NODISCARD bool testAndSetAcquire(
        Integer expectedValue,
        Integer newValue,
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetAcquire(
            _atomic, expectedValue, newValue, &currentValue);
    }

    NODISCARD bool testAndSetRelease(
        Integer expectedValue, 
        Integer newValue, 
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelease(
            _atomic, expectedValue, newValue, &currentValue);
    }

    NODISCARD bool testAndSetOrdered(
        Integer expectedValue, 
        Integer newValue, 
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetOrdered(
            _atomic, expectedValue, newValue, &currentValue);
    }

    static constexpr NODISCARD
        bool isFetchAndStoreNative() noexcept
    {
        return SelfAtomicOperations::isFetchAndStoreNative();
    }

    static constexpr NODISCARD
        bool isFetchAndStoreWaitFree() noexcept 
    {
        return SelfAtomicOperations::isFetchAndStoreWaitFree(); 
    }

    NODISCARD Integer fetchAndStoreRelaxed(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreRelaxed(
            _atomic, newValue);
    }

    NODISCARD Integer fetchAndStoreAcquire(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreAcquire(
            _atomic, newValue);
    }

    NODISCARD Integer fetchAndStoreRelease(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreRelease(
            _atomic, newValue);
    }

    NODISCARD Integer fetchAndStoreOrdered(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreOrdered(
            _atomic, newValue);
    }

    static constexpr NODISCARD 
        bool isFetchAndAddNative() noexcept
    {
        return SelfAtomicOperations::isFetchAndAddNative(); 
    }

    static constexpr NODISCARD 
        bool isFetchAndAddWaitFree() noexcept 
    {
        return SelfAtomicOperations::isFetchAndAddWaitFree(); 
    }

    NODISCARD Integer fetchAndAddRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddRelaxed(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndAddAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddAcquire(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndAddRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddRelease(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndAddOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddOrdered(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndSubRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubRelaxed(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndSubAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubAcquire(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndSubRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubRelease(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndSubOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubOrdered(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndAndRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndRelaxed(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndAndAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndAcquire(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndAndRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndRelease(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndAndOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndOrdered(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndOrRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrRelaxed(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndOrAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrAcquire(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndOrRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrRelease(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndOrOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrOrdered(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndXorRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorRelaxed(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndXorAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorAcquire(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndXorRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorRelease(
            _atomic, valueToAdd);
    }

    NODISCARD Integer fetchAndXorOrdered(Integer valueToAdd) noexcept {
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

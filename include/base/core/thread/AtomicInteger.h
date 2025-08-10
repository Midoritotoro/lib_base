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

      Integer loadRelaxed() const noexcept {
        return SelfAtomicOperations::loadRelaxed(_atomic);
    }

      Integer loadSequentiallyConsistent() const noexcept {
        return SelfAtomicOperations::loadSequentiallyConsistent(_atomic);
    }

    void storeRelaxed(Integer newValue) noexcept { 
        SelfAtomicOperations::storeRelaxed(_atomic, newValue);
    }

      Integer loadAcquire() const noexcept { 
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

    static constexpr   
        bool isReferenceCountingNative() noexcept 
    {
        return SelfAtomicOperations::isReferenceCountingNative(); 
    }

    static constexpr   
        bool isReferenceCountingWaitFree() noexcept 
    { 
        return SelfAtomicOperations::isReferenceCountingWaitFree();
    }

      bool ref() noexcept { 
        return SelfAtomicOperations::ref(_atomic);
    }

      bool deref() noexcept {
        return SelfAtomicOperations::deref(_atomic); 
    }

    static constexpr  
        bool isTestAndSetNative() noexcept 
    {
        return SelfAtomicOperations::isTestAndSetNative();
    }

    static constexpr   
        bool isTestAndSetWaitFree() noexcept
    { 
        return SelfAtomicOperations::isTestAndSetWaitFree(); 
    }

      bool testAndSetRelaxed(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelaxed(
            _atomic, expectedValue, newValue);
    }

      bool testAndSetAcquire(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetAcquire(
            _atomic, expectedValue, newValue);
    }

      bool testAndSetRelease(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelease(
            _atomic, expectedValue, newValue);
    }

      bool testAndSetOrdered(
        Integer expectedValue, 
        Integer newValue) noexcept
    {
        return SelfAtomicOperations::testAndSetOrdered(
            _atomic, expectedValue, newValue);
    }

      bool testAndSetRelaxed(
        Integer expectedValue,
        Integer newValue,
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelaxed(
            _atomic, expectedValue, newValue, &currentValue);
    }

      bool testAndSetAcquire(
        Integer expectedValue,
        Integer newValue,
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetAcquire(
            _atomic, expectedValue, newValue, &currentValue);
    }

      bool testAndSetRelease(
        Integer expectedValue, 
        Integer newValue, 
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetRelease(
            _atomic, expectedValue, newValue, &currentValue);
    }

      bool testAndSetOrdered(
        Integer expectedValue, 
        Integer newValue, 
        Integer& currentValue) noexcept
    {
        return SelfAtomicOperations::testAndSetOrdered(
            _atomic, expectedValue, newValue, &currentValue);
    }

    static constexpr  
        bool isFetchAndStoreNative() noexcept
    {
        return SelfAtomicOperations::isFetchAndStoreNative();
    }

    static constexpr  
        bool isFetchAndStoreWaitFree() noexcept 
    {
        return SelfAtomicOperations::isFetchAndStoreWaitFree(); 
    }

      Integer fetchAndStoreRelaxed(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreRelaxed(
            _atomic, newValue);
    }

      Integer fetchAndStoreAcquire(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreAcquire(
            _atomic, newValue);
    }

      Integer fetchAndStoreRelease(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreRelease(
            _atomic, newValue);
    }

      Integer fetchAndStoreOrdered(Integer newValue) noexcept {
        return SelfAtomicOperations::fetchAndStoreOrdered(
            _atomic, newValue);
    }

    static constexpr   
        bool isFetchAndAddNative() noexcept
    {
        return SelfAtomicOperations::isFetchAndAddNative(); 
    }

    static constexpr   
        bool isFetchAndAddWaitFree() noexcept 
    {
        return SelfAtomicOperations::isFetchAndAddWaitFree(); 
    }

      Integer fetchAndAddRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddRelaxed(
            _atomic, valueToAdd);
    }

      Integer fetchAndAddAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddAcquire(
            _atomic, valueToAdd);
    }

      Integer fetchAndAddRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddRelease(
            _atomic, valueToAdd);
    }

      Integer fetchAndAddOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAddOrdered(
            _atomic, valueToAdd);
    }

      Integer fetchAndSubRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubRelaxed(
            _atomic, valueToAdd);
    }

      Integer fetchAndSubAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubAcquire(
            _atomic, valueToAdd);
    }

      Integer fetchAndSubRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubRelease(
            _atomic, valueToAdd);
    }

      Integer fetchAndSubOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndSubOrdered(
            _atomic, valueToAdd);
    }

      Integer fetchAndAndRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndRelaxed(
            _atomic, valueToAdd);
    }

      Integer fetchAndAndAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndAcquire(
            _atomic, valueToAdd);
    }

      Integer fetchAndAndRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndRelease(
            _atomic, valueToAdd);
    }

      Integer fetchAndAndOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndAndOrdered(
            _atomic, valueToAdd);
    }

      Integer fetchAndOrRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrRelaxed(
            _atomic, valueToAdd);
    }

      Integer fetchAndOrAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrAcquire(
            _atomic, valueToAdd);
    }

      Integer fetchAndOrRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrRelease(
            _atomic, valueToAdd);
    }

      Integer fetchAndOrOrdered(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndOrOrdered(
            _atomic, valueToAdd);
    }

      Integer fetchAndXorRelaxed(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorRelaxed(
            _atomic, valueToAdd);
    }

      Integer fetchAndXorAcquire(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorAcquire(
            _atomic, valueToAdd);
    }

      Integer fetchAndXorRelease(Integer valueToAdd) noexcept {
        return SelfAtomicOperations::fetchAndXorRelease(
            _atomic, valueToAdd);
    }

      Integer fetchAndXorOrdered(Integer valueToAdd) noexcept {
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

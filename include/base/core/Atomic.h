#pragma once 

#include <base/system/Platform.h>
#include <base/core/AtomicConfig.h>


namespace base {
	template <
		typename Integer = int,
		typename = std::enable_if<std::is_integral_v<Integer>>>
	class Atomic {
	public:
        Integer loadRelaxed() const;
        Integer loadAcquire() const;

        void storeRelaxed(Integer newValue);
        void storeRelease(Integer newValue);

        operator Integer() const;
        Atomic& operator=(Integer);

        static constexpr bool isReferenceCountingNative();
        static constexpr bool isReferenceCountingWaitFree();

        bool ref();
        bool deref();

        static constexpr bool isTestAndSetNative();
        static constexpr bool isTestAndSetWaitFree();

        bool testAndSetRelaxed(
            Integer expectedValue,
            Integer newValue);
        bool testAndSetAcquire(
            Integer expectedValue, 
            Integer newValue);

        bool testAndSetRelease(
            Integer expectedValue,
            Integer newValue);
        bool testAndSetOrdered(
            Integer expectedValue,
            Integer newValue);

        bool testAndSetRelaxed(
            Integer expectedValue, 
            Integer newValue,
            Integer& currentValue);
        bool testAndSetAcquire(
            Integer expectedValue,
            Integer newValue, 
            Integer& currentValue);

        bool testAndSetRelease(
            Integer expectedValue, 
            Integer newValue, 
            Integer& currentValue);
        bool testAndSetOrdered(
            Integer expectedValue, 
            Integer newValue, 
            Integer& currentValue);

        static constexpr bool isFetchAndStoreNative();
        static constexpr bool isFetchAndStoreWaitFree();

        Integer fetchAndStoreRelaxed(T newValue);
        Integer fetchAndStoreAcquire(T newValue);

        Integer fetchAndStoreRelease(T newValue);
        Integer fetchAndStoreOrdered(T newValue);

        static constexpr bool isFetchAndAddNative();
        static constexpr bool isFetchAndAddWaitFree();

        Integer fetchAndAddRelaxed(Integer valueToAdd);
        Integer fetchAndAddAcquire(Integer valueToAdd);

        Integer fetchAndAddRelease(Integer valueToAdd);
        Integer fetchAndAddOrdered(Integer valueToAdd);

        Integer fetchAndSubRelaxed(Integer valueToSub);
        Integer fetchAndSubAcquire(Integer valueToSub);

        Integer fetchAndSubRelease(Integer valueToSub);
        Integer fetchAndSubOrdered(Integer valueToSub);

        Integer fetchAndOrRelaxed(Integer valueToOr);
        Integer fetchAndOrAcquire(Integer valueToOr);

        Integer fetchAndOrRelease(Integer valueToOr);
        Integer fetchAndOrOrdered(Integer valueToOr);

        Integer fetchAndAndRelaxed(Integer valueToAnd);
        Integer fetchAndAndAcquire(Integer valueToAnd);

        Integer fetchAndAndRelease(Integer valueToAnd);
        Integer fetchAndAndOrdered(Integer valueToAnd);

        Integer fetchAndXorRelaxed(Integer valueToXor);
        Integer fetchAndXorAcquire(Integer valueToXor);

        Integer fetchAndXorRelease(Integer valueToXor);
        Integer fetchAndXorOrdered(Integer valueToXor);

        Integer operator++();
        Integer operator++(int);

        Integer operator--();
        Integer operator--(int);

        Integer operator+=(Integer value);
        Integer operator-=(Integer value);

        Integer operator|=(Integer value);
        Integer operator&=(Integer value);

        Integer operator^=(Integer value);
	private:
		std::atomic<Integer> _atomic;

	};
}
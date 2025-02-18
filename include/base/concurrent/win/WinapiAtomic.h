#pragma once 


#ifndef _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#endif // _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

#ifdef __STDC_NO_ATOMICS__ 
#undef __STDC_NO_ATOMICS__ 
#endif // __STDC_NO_ATOMICS__ 


#include <atomic>
#include <base/system/Platform.h>



namespace base::Threads {
	struct atomic_rc_t {
        ::std::atomic_uintptr_t refs;

        inline atomic_rc_t& operator=(const atomic_rc_t& other) {
            refs = other.refs.load();
            return *this;
        }
	};

    struct rcu_generation {
        ::std::atomic_uintptr_t readers;
        ::std::atomic_uint writer;
    };

    struct rcu_thread {
        rcu_generation* generation;
        uintptr_t recursion;
    };


	[[nodiscard]] bool AtomicRcDec(atomic_rc_t* rc);
    void AtomicRcInc(atomic_rc_t* rc);

	void AtomicRcInit(atomic_rc_t* rc);

    void atomic_notify_one(void* addr);
    void atomic_notify_all(void* addr);

    bool rcu_read_held();

    void rcu_read_lock();
    void rcu_read_unlock();
} // namespace base::Threads
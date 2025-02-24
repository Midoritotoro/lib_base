#include <base/concurrent/win/WinapiAtomic.h>
#include <base/utility/Assert.h>



namespace base::Threads {
    static thread_local rcu_thread current;
    static ::std::atomic<rcu_generation*> generation;
  

    bool AtomicRcDec(atomic_rc_t* rc) {
        uintptr_t prev = atomic_fetch_sub_explicit(&rc->refs, (uintptr_t)1,
            ::std::memory_order_acq_rel);
        Assert(prev);

        return prev == 1;
    }

    void AtomicRcInc(atomic_rc_t* rc)
    {
        uintptr_t prev = atomic_fetch_add_explicit(&rc->refs, (uintptr_t)1,
            ::std::memory_order_relaxed);

        Assert(prev);
        unused(prev);
    }


    void AtomicRcInit(atomic_rc_t* rc) {
        ::std::atomic_init(&rc->refs, (uintptr_t)1);
    }


    void atomic_notify_one(void* addr)
    {
        WakeByAddressSingle(addr);
    }

    void atomic_notify_all(void* addr)
    {
        WakeByAddressAll(addr);
    }

    bool rcu_read_held()
    {
        const rcu_thread* const self = &current;

        return self->recursion > 0;
    }

    void rcu_read_lock()
    {
        rcu_thread* const self = &current;
        rcu_generation* gen;

        if (self->recursion++ > 0)
            return; /* recursion: nothing to do */

        Assert(self->generation == NULL);
        gen = atomic_load_explicit(&generation, ::std::memory_order_acquire);
        self->generation = gen;
        atomic_fetch_add_explicit(&gen->readers, 1, ::std::memory_order_relaxed);
    }

    void rcu_read_unlock()
    {
        rcu_thread* const self = &current;
        rcu_generation* gen;

        Assert(rcu_read_held());

        if (--self->recursion > 0)
            return; /* recursion: nothing to do */

        gen = self->generation;
        self->generation = NULL;

        uintptr_t readers = atomic_fetch_sub_explicit(&gen->readers, 1,
            ::std::memory_order_relaxed);

        if (readers == 0)
            Assert("unreachable!", unreachable());

        if (readers > 1)
            return; /* Other reader threads remain: nothing to do */

        if (UNLIKELY(atomic_exchange_explicit(&gen->writer, 0,
            ::std::memory_order_release)))
            atomic_notify_one(&gen->writer); /* Last reader wakes writer up */
    }
} // namespace base::Threads
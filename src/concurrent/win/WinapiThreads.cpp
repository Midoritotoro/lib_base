#include <base/concurrent/win/WinapiThreads.h>

#include <atomic>
#include <cassert>

#include <Windows.h>
#include <thread>

#include <base/Platform.h>


namespace Threads {
    int threadvar_set(threadvar_t key, void* value)
    {
        *key->id= (ULONG)value;
        return 0;
    }

    void* threadvar_get(threadvar_t key)
    {
        return (void*)*key->id;
    }

    tick_t mdate_wall()
    {
        FILETIME ts;
        ULARGE_INTEGER s;

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
        GetSystemTimePreciseAsFileTime(&ts);
#else // _WIN32_WINNT < _WIN32_WINNT_WIN8
        GetSystemTimeAsFileTime(&ts);
#endif // _WIN32_WINNT < _WIN32_WINNT_WIN8
        s.LowPart = ts.dwLowDateTime;
        s.HighPart = ts.dwHighDateTime;
        /* hundreds of nanoseconds */
        static_assert ((10000000 % CLOCK_FREQ) == 0, "Broken frequencies ratio");
        return TICK_FROM_MSFTIME(s.QuadPart);
    }

#if WINAPI_FAMILY_PARTITION (WINAPI_PARTITION_DESKTOP)
    static tick_t mdate_multimedia(void)
    {
        DWORD ts = timeGetTime();

        /* milliseconds */
        static_assert ((CLOCK_FREQ % 1000) == 0, "Broken frequencies ratio");
        return TICK_FROM_MS(ts);
    }
#endif

    static tick_t(*mdate_selected) (void) = mdate_wall;

    tick_t tick_now(void)
    {
        return mdate_selected();
    }

     int savecancel()
    {
        if (!thread) /* not created by VLC, can't be cancelled */
            return true;

        int oldstate = thread->killable;
        thread->killable = false;
        return oldstate;
    }

    void global_mutex(unsigned n, bool acquire)
    {
        static mutex_t locks[] = {
            STATIC_MUTEX,
            STATIC_MUTEX,
            STATIC_MUTEX,
            STATIC_MUTEX,
    #ifdef _WIN32
            STATIC_MUTEX, // For MTA holder
    #endif
        };
        static_assert (MAX_MUTEX == ARRAY_SIZE(locks),
            "Wrong number of global mutexes");
        assert(n < ARRAY_SIZE(locks));

        mutex_t* lock = locks + n;
        if (acquire)
            mutex_lock(lock);
        else
            mutex_unlock(lock);
    }

    unsigned long thread_id()
    {
        static DWORD tid = 0;

        if (unlikely(tid == 0))
            tid = GetCurrentThreadId();

        return tid;
    }

    void restorecancel(int state)
    {
        if (!thread) /* not created by VLC, can't be cancelled */
            return;

        thread->killable = state;
    }

    void mutex_init_common(mutex_t* mtx, bool recursive)
    {
        atomic_init(&mtx->value, 0);
        atomic_init(&mtx->recursion, recursive ? 1 : 0);
        atomic_init(&mtx->owner, 0);
    }

    void mutex_init(mutex_t* mtx)
    {
        mutex_init_common(mtx, false);
    }

    void mutex_init_recursive(mutex_t* mtx)
    {
        mutex_init_common(mtx, true);
    }

    bool mutex_held(const mutex_t* mtx)
    {
        /* This comparison is thread-safe:
         * Even though other threads may modify the owner field at any time,
         * they will never make it compare equal to the calling thread.
         */
        return thread_id() == atomic_load_explicit(&mtx->owner,
            std::memory_order_relaxed);
    }

    void mutex_lock(mutex_t* mtx)
    {
        /* This is the Drepper (non-recursive) mutex algorithm
         * from his "Futexes are tricky" paper. The mutex value can be:
         * - 0: the mutex is free
         * - 1: the mutex is locked and uncontended
         * - 2: the mutex is contended (i.e., unlock needs to wake up a waiter)
         */
        if (mutex_trylock(mtx) == 0)
            return;

        int canc = savecancel(); /* locking is never a cancellation point */

        while (atomic_exchange_explicit(&mtx->value, 2, std::memory_order_acquire))
            atomic_wait(&mtx->value, 2);

        restorecancel(canc);
        atomic_store_explicit(&mtx->owner, thread_id(), std::memory_order_relaxed);
    }

    int mutex_trylock(mutex_t* mtx)
    {
        /* Check the recursion counter:
         * - 0: mutex is not recursive.
         * - 1: mutex is recursive but free or locked non-recursively.
         * - n > 1: mutex is recursive and locked n time(s).
         */
        unsigned recursion = atomic_load_explicit(&mtx->recursion,
            std::memory_order_relaxed);
        if (unlikely(recursion)) {
            if (mutex_held(mtx)) {
                /* This thread already owns the mutex, locks recursively.
                 * Other threads shall not have modified the recursion or owner fields.
                 */
                atomic_store_explicit(&mtx->recursion, recursion + 1,
                    std::memory_order_relaxed);
                return 0;
            }
        }
        else
            assert(!mutex_held(mtx));

        unsigned value = 0;

        if (atomic_compare_exchange_strong_explicit(&mtx->value, &value, 1,
            std::memory_order_acquire,
            std::memory_order_relaxed)) {
            atomic_store_explicit(&mtx->owner, thread_id(),
                std::memory_order_relaxed);
            return 0;
        }

        return EBUSY;
    }

    void mutex_unlock(mutex_t* mtx)
    {
        assert(mutex_held(mtx));

        unsigned recursion = atomic_load_explicit(&mtx->recursion,
            std::memory_order_relaxed);
        if (unlikely(recursion > 1)) {
            /* Non-last recursive unlocking. */
            atomic_store_explicit(&mtx->recursion, recursion - 1,
                std::memory_order_relaxed);
            return;
        }

        atomic_store_explicit(&mtx->owner, 0, std::memory_order_relaxed);

        switch (atomic_exchange_explicit(&mtx->value, 0, std::memory_order_release)) {
        case 2:
            atomic_notify_one(&mtx->value);
        case 1:
            break;
        default:
            (assert("unreachable"), unreachable());
        }
    }

    void cond_init(cond_t* cond)
    {
        cond->head = NULL;
        mutex_init(&cond->lock);
    }

    static void cond_signal_waiter(struct cond_waiter* waiter)
    {
        waiter->pprev = &waiter->next;
        waiter->next = NULL;
        atomic_fetch_add_explicit(&waiter->value, 1, std::memory_order_relaxed);
        atomic_notify_one(&waiter->value);
    }

    void cond_signal(cond_t* cond)
    {
        struct cond_waiter* waiter;

        /* Some call sites signal their condition variable without holding the
         * corresponding lock. Thus an extra lock is needed here to ensure the
         * consistency of the linked list and the lifetime of its elements.
         * If all call sites locked cleanly, the inner lock would be unnecessary.
         */
        mutex_lock(&cond->lock);
        waiter = cond->head;

        if (waiter != NULL) {
            struct cond_waiter* next = waiter->next;
            struct cond_waiter** pprev = waiter->pprev;

            *pprev = next;

            if (next != NULL)
                next->pprev = pprev;

            cond_signal_waiter(waiter);
        }

        mutex_unlock(&cond->lock);
    }

    void cond_broadcast(cond_t* cond)
    {
        struct cond_waiter* waiter;

        mutex_lock(&cond->lock);
        waiter = cond->head;
        cond->head = NULL;

        /* Keep the lock here so that waiters don't go out of scope */
        while (waiter != NULL) {
            struct cond_waiter* next = waiter->next;

            cond_signal_waiter(waiter);
            waiter = next;
        }

        mutex_unlock(&cond->lock);
    }

    static void cond_wait_prepare(struct cond_waiter* waiter,
        cond_t* cond, mutex_t* mutex)
    {
        struct cond_waiter* next;

        waiter->pprev = &cond->head;
        atomic_init(&waiter->value, 0);

        mutex_lock(&cond->lock);
        next = cond->head;
        cond->head = waiter;
        waiter->next = next;

        if (next != NULL)
            next->pprev = &waiter->next;

        mutex_unlock(&cond->lock);
        mutex_unlock(mutex);
    }

    static void cond_wait_finish(struct cond_waiter* waiter,
        cond_t* cond, mutex_t* mutex)
    {
        struct cond_waiter* next;

        /* If this waiter is still on the linked list, remove it before it goes
         * out of scope. Otherwise, this is a no-op.
         */
        mutex_lock(&cond->lock);
        next = waiter->next;
        *(waiter->pprev) = next;

        if (next != NULL)
            next->pprev = waiter->pprev;

        mutex_unlock(&cond->lock);

        /* Lock the caller's mutex as required by condition variable semantics. */
        mutex_lock(mutex);
    }

    void cond_wait(cond_t* cond, mutex_t* mutex)
    {
        struct cond_waiter waiter;

        // wait on a multiply locked mutex not supported
        assert(atomic_load_explicit(&mutex->recursion, memory_order_relaxed) <= 1);

        cond_wait_prepare(&waiter, cond, mutex);
        atomic_wait(&waiter.value, 0);
        cond_wait_finish(&waiter, cond, mutex);
    }

    int cond_timedwait(cond_t* cond, mutex_t* mutex,
        tick_t deadline)
    {
        struct cond_waiter waiter;
        int ret;

        // wait on a multiply locked mutex not supported
        assert(atomic_load_explicit(&mutex->recursion, memory_order_relaxed) <= 1);
        assert(atomic_load_explicit(&mutex->recursion, memory_order_relaxed) <= 1);
        assert(atomic_load_explicit(&mutex->recursion, memory_order_relaxed) <= 1);

        cond_wait_prepare(&waiter, cond, mutex);
        ret = atomic_timedwait(&waiter.value, 0, deadline);
        cond_wait_finish(&waiter, cond, mutex);

        return ret;
    }

    int cond_timedwait_daytime(cond_t* cond, mutex_t* mutex,
        time_t deadline)
    {
        struct cond_waiter waiter;
        int ret;

        cond_wait_prepare(&waiter, cond, mutex);
        ret = atomic_timedwait_daytime(&waiter.value, 0, deadline);
        cond_wait_finish(&waiter, cond, mutex);

        return ret;
    }

    /*** Generic semaphores ***/

    void sem_init(sem_t* sem, unsigned value)
    {
        atomic_init(&sem->value, value);
    }

    int sem_post(sem_t* sem)
    {
        unsigned exp = atomic_load_explicit(&sem->value, std::memory_order_relaxed);

        do
        {
            if (unlikely(exp == UINT_MAX))
                return EOVERFLOW;
        } while (!atomic_compare_exchange_weak_explicit(&sem->value, &exp, exp + 1,
            std::memory_order_release,
            std::memory_order_relaxed));

        atomic_notify_one(&sem->value);
        return 0;
    }

    void sem_wait(sem_t* sem)
    {
        unsigned exp = 1;

        while (!atomic_compare_exchange_weak_explicit(&sem->value, &exp, exp - 1,
            std::memory_order_acquire,
            std::memory_order_relaxed))
        {
            if (likely(exp == 0))
            {
                atomic_wait(&sem->value, 0);
                exp = 1;
            }
        }
    }

    int sem_timedwait(sem_t* sem, tick_t deadline)
    {
        unsigned exp = 1;

        while (!atomic_compare_exchange_weak_explicit(&sem->value, &exp, exp - 1,
            std::memory_order_acquire,
            std::memory_order_relaxed))
        {
            if (likely(exp == 0))
            {
                int ret = atomic_timedwait(&sem->value, 0, deadline);
                if (ret)
                    return ret;

                exp = 1;
            }
        }

        return 0;
    }

    int sem_trywait(sem_t* sem)
    {
        unsigned exp = atomic_load_explicit(&sem->value, std::memory_order_relaxed);

        do
            if (exp == 0)
                return EAGAIN;
        while (!atomic_compare_exchange_weak_explicit(&sem->value, &exp, exp - 1,
            std::memory_order_acquire,
            std::memory_order_relaxed));

        return 0;
    }


    int atomic_timedwait(void* addr, unsigned val, tick_t deadline)
    {
        tick_t delay;

        for (;;)
        {
            delay = deadline - tick_now();

            if (delay < 0)
                return ETIMEDOUT; // deadline passed

            DWORD ms;
            int64_t idelay = MS_FROM_TICK(delay);
            static_assert(sizeof(unsigned long) <= sizeof(DWORD), "unknown max DWORD");
            if (unlikely(idelay > ULONG_MAX))
                ms = ULONG_MAX;
            else
                ms = idelay;

            if (WaitOnAddress(addr, &val, sizeof(val), ms))
                return 0;
            if (GetLastError() == ERROR_TIMEOUT)
                return ETIMEDOUT;
        }
    }

    int atomic_timedwait_daytime(void* addr, unsigned val, time_t deadline)
    {
        time_t delay;

        for (;;)
        {
            delay = deadline - time(NULL);

            if (delay < 0)
                return ETIMEDOUT; // deadline passed

            DWORD ms;
            static_assert(sizeof(unsigned long) <= sizeof(DWORD), "unknown max DWORD");
            if (unlikely(delay > (ULONG_MAX / 1000)))
                ms = ULONG_MAX;
            else
                ms = delay * 1000;

            if (WaitOnAddress(addr, &val, sizeof(val), ms))
                return 0;
            if (GetLastError() == ERROR_TIMEOUT)
                return ETIMEDOUT;
        }
    }
} // namespace Threads
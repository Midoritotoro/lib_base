#pragma once 

#include <base/core/arch/Platform.h>

#ifdef CPP_MSVC_ONLY
// MSVC defines _YIELD_PROCESSOR()(_mm_pause) in <xatomic.h>
// Compiler sometimes recognizes _mm_pause as intrinsic
#  ifdef __cplusplus
#    include <atomic>
extern "C"
#  endif
void _mm_pause(void);
#endif

__BASE_ARCH_NAMESPACE_BEGIN

#ifdef CPP_GNU
    __attribute__((artificial))
#endif
        always_inline void YieldCpu(void);

    void YieldCpu(void)
#ifdef __cplusplus
        noexcept
#endif
    {
#if __has_builtin(__yield)
        __yield();

#elif defined(_YIELD_PROCESSOR) && defined(CPP_MSVC)
        _YIELD_PROCESSOR();

#elif __has_builtin(__builtin_ia32_pause)
        __builtin_ia32_pause();

#elif defined(PROCESSOR_X86) && defined(CPP_GNU)
        // GCC < 10 don`t have __has_builtin()
        __builtin_ia32_pause();

#elif defined(PROCESSOR_X86) && defined(CPP_MSVC)
        _mm_pause();

#elif defined(PROCESSOR_X86)
        asm("pause");

#elif __has_builtin(__builtin_arm_yield)
        __builtin_arm_yield();

#elif defined(PROCESSOR_ARM) && PROCESSOR_ARM >= 7 && defined(CPP_GNU)
        asm("yield");        

#elif defined(PROCESSOR_RISCV)
        asm(".word 0x0100000f");        // a.k.a. "pause"

#endif
    }

__BASE_ARCH_NAMESPACE_END
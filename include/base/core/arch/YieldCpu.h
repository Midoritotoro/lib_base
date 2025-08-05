#pragma once 

#include <base/core/arch/Platform.h>

#if defined(base_cpp_msvc_only)
// MSVC defines _YIELD_PROCESSOR()(_mm_pause) in <xatomic.h>
// Compiler sometimes recognizes _mm_pause as intrinsic
#  if defined(__cplusplus)
#    include <atomic>
extern "C"
#  endif // defined(__cplusplus)
  void _mm_pause(void);
#endif // defined(base_cpp_msvc_only)

__BASE_ARCH_NAMESPACE_BEGIN

#if defined(base_cpp_gnu)
    __attribute__((artificial))
#endif

always_inline void YieldCpu(void);

void YieldCpu(void)
#if defined(__cplusplus)
        noexcept
#endif // defined(__cplusplus)
    {
#if __has_builtin(__yield)
        __yield();

#elif defined(_YIELD_PROCESSOR) && defined(base_cpp_msvc)
        _YIELD_PROCESSOR();

#elif __has_builtin(__builtin_ia32_pause)
        __builtin_ia32_pause();

#elif defined(PROCESSOR_X86) && defined(base_cpp_gnu)
        // GCC < 10 don`t have __has_builtin()
        __builtin_ia32_pause();

#elif defined(PROCESSOR_X86) && defined(base_cpp_msvc)
        _mm_pause();

#elif defined(PROCESSOR_X86)
        asm("pause");

#elif __has_builtin(__builtin_arm_yield)
        __builtin_arm_yield();

#elif defined(PROCESSOR_ARM) && PROCESSOR_ARM >= 7 && defined(base_cpp_gnu)
        asm("yield");        

#elif defined(PROCESSOR_RISCV)
        asm(".word 0x0100000f");        // a.k.a. "pause"

#endif
    }

__BASE_ARCH_NAMESPACE_END
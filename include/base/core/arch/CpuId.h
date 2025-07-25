#pragma once 

#include <base/core/arch/Platform.h>


__BASE_ARCH_NAMESPACE_BEGIN

#if defined(CPP_GNU) && !defined(BaseGCCCpuIdCall)
// gcc won't allow us to clobber ebx since its used to store the got. so we need to
// lie to gcc and backup/restore ebx without declaring it as clobbered.
#  define BaseGCCCpuIdCall(eax, ebx, ecx, edx, op)      \
    asm volatile("pushl %%ebx   \n\t"                   \
        "cpuid         \n\t"                            \
        "movl %%ebx, %1\n\t"                            \
        "popl %%ebx    \n\t"                            \
        : "=a"(eax), "=r"(ebx), "=c"(ecx), "=d"(edx)    \
        : "a"(op)                                       \                           
    : "cc")
#endif

DECLARE_NOALIAS inline void CpuId(
    int32 op,
    int32& eax, int32& ebx,
    int32& ecx, int32& edx)
{
#if defined(CPP_GNU)
    BaseGCCCpuIdCall(eax, ebx, ecx, edx, op);
#elif defined(WIN32)
    int regs[4];
    __cpuid(regs, op);

    eax = regs[0];
    ebx = regs[1];

    ecx = regs[2];
    edx = regs[3];
#endif
}

DECLARE_NOALIAS inline void CpuId(
    int32* data,
    int32 op)
{
#if defined(CPP_GNU)
    BaseGCCCpuIdCall(data, data + 4, data + 8, data + 12, op);
#elif defined(WIN32)
    __cpuid(data, op);
#endif
}

DECLARE_NOALIAS inline void CpuIdExtended(
    int32* data,
    int32 op,
    int32 ex)
{
#if defined(CPP_GNU)
    BaseGCCCpuIdCall(data, data + 4, data + 8, data + 12, op);
#elif defined(WIN32)
    __cpuidex(data, op, ex);
#endif
}

DECLARE_NOALIAS inline void CpuIdExtended(int32 op,
    int32& eax, int32& ebx,
    int32& ecx, int32& edx,
    int32 ex)
{
#if defined(CPP_GNU)
    BaseGCCCpuIdCall(eax, ebx, ecx, edx, op);
#elif defined(WIN32)
    int regs[4];
    __cpuidex(regs, op, ex);

    eax = regs[0];
    ebx = regs[1];

    ecx = regs[2];
    edx = regs[3];
#endif
}

__BASE_ARCH_NAMESPACE_END
#pragma once 

#include <base/core/arch/Platform.h>


__BASE_ARCH_NAMESPACE_BEGIN


#if defined(base_cpp_gnu)
#  if !defined(BaseGCCCpuIdCall)
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
#  endif // BaseGCCCpuIdCall
#endif // base_cpp_gnu


base_always_inline void CpuId(
    int32 op,
    int32& eax,
    int32& ebx,
    int32& ecx,
    int32& edx) noexcept
{
#if defined(base_cpp_gnu)
    BaseGCCCpuIdCall(eax, ebx, ecx, edx, op);
#elif defined(base_os_win32)
    int regs[4];
    __cpuid(regs, op);

    eax = regs[0];
    ebx = regs[1];

    ecx = regs[2];
    edx = regs[3];
#endif // defined(base_cpp_gnu) || defined(base_os_win32)
}

base_always_inline void CpuId(
    int32* data,
    int32 op) noexcept
{
#if defined(base_cpp_gnu)
    BaseGCCCpuIdCall(data, data + 4, data + 8, data + 12, op);
#elif defined(base_os_win32)
    __cpuid(data, op);
#endif // defined(base_cpp_gnu) || defined(base_os_win32)
}

base_always_inline void CpuIdExtended(
    int32* data,
    int32 op,
    int32 ex) noexcept
{
#if defined(base_cpp_gnu)
    BaseGCCCpuIdCall(data, data + 4, data + 8, data + 12, op);
#elif defined(base_os_win32)
    __cpuidex(data, op, ex);
#endif // defined(base_cpp_gnu) || defined(base_os_win32)
}

inline void CpuIdExtended(
    int32 op,
    int32& eax,
    int32& ebx,
    int32& ecx,
    int32& edx,
    int32 ex)
{
#if defined(base_cpp_gnu)
    BaseGCCCpuIdCall(eax, ebx, ecx, edx, op);
#elif defined(base_os_win32)
    int regs[4];
    __cpuidex(regs, op, ex);

    eax = regs[0];
    ebx = regs[1];

    ecx = regs[2];
    edx = regs[3];
#endif // defined(base_cpp_gnu) || defined(base_os_win32)
}


__BASE_ARCH_NAMESPACE_END

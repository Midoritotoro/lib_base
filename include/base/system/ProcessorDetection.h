#pragma once


#if defined(__ORDER_BIG_ENDIAN__)
#  define BIG_ENDIAN __ORDER_BIG_ENDIAN__
#else
#  define BIG_ENDIAN 4321
#endif
#if defined(__ORDER_LITTLE_ENDIAN__)
#  define LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#else
#  define LITTLE_ENDIAN 1234
#endif

#if defined(__alpha__) || defined(_M_ALPHA)
#  define PROCESSOR_ALPHA

#elif defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__) || defined(__ARM64__)
#  if defined(__aarch64__) || defined(__ARM64__) || defined(_M_ARM64)
#    define PROCESSOR_ARM_64
#    define PROCESSOR_WORDSIZE 8
#  else
#    define PROCESSOR_ARM_32
#  endif
#  if defined(__ARM_ARCH) && __ARM_ARCH > 1
#    define PROCESSOR_ARM __ARM_ARCH
#  elif defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM > 1
#    define PROCESSOR_ARM __TARGET_ARCH_ARM
#  elif defined(_M_ARM) && _M_ARM > 1
#    define PROCESSOR_ARM _M_ARM
#  elif defined(__ARM64_ARCH_8__) \
      || defined(__aarch64__) \
      || defined(__ARMv8__) \
      || defined(__ARMv8_A__) \
      || defined(_M_ARM64)
#    define PROCESSOR_ARM 8
#  elif defined(__ARM_ARCH_7__) \
      || defined(__ARM_ARCH_7A__) \
      || defined(__ARM_ARCH_7R__) \
      || defined(__ARM_ARCH_7M__) \
      || defined(__ARM_ARCH_7S__) \
      || defined(_ARM_ARCH_7) \
      || defined(__CORE_CORTEXA__)
#    define PROCESSOR_ARM 7
#  elif defined(__ARM_ARCH_6__) \
      || defined(__ARM_ARCH_6J__) \
      || defined(__ARM_ARCH_6T2__) \
      || defined(__ARM_ARCH_6Z__) \
      || defined(__ARM_ARCH_6K__) \
      || defined(__ARM_ARCH_6ZK__) \
      || defined(__ARM_ARCH_6M__)
#    define PROCESSOR_ARM 6
#  elif defined(__ARM_ARCH_5TEJ__) \
        || defined(__ARM_ARCH_5TE__)
#    define PROCESSOR_ARM 5
#  else
#    define PROCESSOR_ARM 0
#  endif
#  if PROCESSOR_ARM >= 8
#    define PROCESSOR_ARM_V8
#  endif
#  if PROCESSOR_ARM >= 7
#    define PROCESSOR_ARM_V7
#  endif
#  if PROCESSOR_ARM >= 6
#    define PROCESSOR_ARM_V6
#  endif
#  if PROCESSOR_ARM >= 5
#    define PROCESSOR_ARM_V5
#  else
#    error "ARM architecture too old"
#  endif
#  if defined(__ARMEL__) || defined(_M_ARM64)
#    define BYTE_ORDER LITTLE_ENDIAN
#  elif defined(__ARMEB__)
#    define BYTE_ORDER BIG_ENDIAN
#  else

#endif

#elif defined(__hppa__)
#  define PROCESSOR_HPPA
#  define BYTE_ORDER BIG_ENDIAN

#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
#  define PROCESSOR_X86_32
#  define BYTE_ORDER LITTLE_ENDIAN
#  define PROCESSOR_WORDSIZE   4


#  if defined(_M_IX86)
#    define PROCESSOR_X86     (_M_IX86/100)
#  elif defined(__i686__) || defined(__athlon__) || defined(__SSE__) || defined(__pentiumpro__)
#    define PROCESSOR_X86     6
#  elif defined(__i586__) || defined(__k6__) || defined(__pentium__)
#    define PROCESSOR_X86     5
#  elif defined(__i486__) || defined(__80486__)
#    define PROCESSOR_X86     4
#  else
#    define PROCESSOR_X86     3
#  endif

#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
#  define PROCESSOR_X86       6
#  define PROCESSOR_X86_64
#  define BYTE_ORDER LITTLE_ENDIAN
#  define PROCESSOR_WORDSIZE   8

#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
#  define PROCESSOR_IA64
#  define PROCESSOR_WORDSIZE   8

#elif defined(__loongarch__)
#  define PROCESSOR_LOONGARCH
#  if __loongarch_grlen == 64
#    define PROCESSOR_LOONGARCH_64
#  else
#    define PROCESSOR_LOONGARCH_32
#  endif
#  define BYTE_ORDER LITTLE_ENDIAN


#elif defined(__m68k__)
#  define PROCESSOR_M68K
#  define BYTE_ORDER BIG_ENDIAN

#elif defined(__mips) || defined(__mips__) || defined(_M_MRX000)
#  define PROCESSOR_MIPS
#  if defined(_MIPS_ARCH_MIPS1) || (defined(__mips) && __mips - 0 >= 1)
#    define PROCESSOR_MIPS_I
#  endif
#  if defined(_MIPS_ARCH_MIPS2) || (defined(__mips) && __mips - 0 >= 2)
#    define PROCESSOR_MIPS_II
#  endif
#  if defined(_MIPS_ARCH_MIPS3) || (defined(__mips) && __mips - 0 >= 3)
#    define PROCESSOR_MIPS_III
#  endif
#  if defined(_MIPS_ARCH_MIPS4) || (defined(__mips) && __mips - 0 >= 4)
#    define PROCESSOR_MIPS_IV
#  endif
#  if defined(_MIPS_ARCH_MIPS5) || (defined(__mips) && __mips - 0 >= 5)
#    define PROCESSOR_MIPS_V
#  endif
#  if defined(_MIPS_ARCH_MIPS32) || defined(__mips32) || (defined(__mips) && __mips - 0 >= 32)
#    define PROCESSOR_MIPS_32
#  endif
#  if defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
#    define PROCESSOR_MIPS_64
#    define PROCESSOR_WORDSIZE 8
#  endif
#  if defined(__MIPSEL__)
#    define BYTE_ORDER LITTLE_ENDIAN
#  elif defined(__MIPSEB__)
#    define BYTE_ORDER BIG_ENDIAN
#  else

// BYTE_ORDER �� ���������, ������������� �������������� ����������� ������������������ ����
#  endif


#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \
      || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  \
      || defined(_M_MPPC) || defined(_M_PPC)
#  define PROCESSOR_POWER
#  if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
#    define PROCESSOR_POWER_64
#    define PROCESSOR_WORDSIZE 8
#  else
#    define PROCESSOR_POWER_32
#  endif

// BYTE_ORDER �� ���������, ������������� �������������� ����������� ������������������ ����

/*
 ��������� RISC-V, ��������� ��������: 32- � 64-���������

 � RISC-V ������� ���������� ���������.
*/

#elif defined(__riscv)
#  define PROCESSOR_RISCV
#  if __riscv_xlen == 64
#    define PROCESSOR_RISCV_64
#  else
#    define PROCESSOR_RISCV_32
#  endif
#  define BYTE_ORDER LITTLE_ENDIAN

/*
��������� S390, ��������� �������: S390X (64-���������)

 � S390 ������������ big-endian.
*/

#elif defined(__s390__)
#  define PROCESSOR_S390
#  if defined(__s390x__)
#    define PROCESSOR_S390_X
#  endif
#  define BYTE_ORDER BIG_ENDIAN

/*
 ��������� SuperH, �������������� ��������: SH-4A

 SuperH ����� �������� ������� ����������, ����������� �������������� ����������� ������������������, ������������� ����.
*/
// #elif defined(__sh__)
// #  define PROCESSOR_SH
// #  if defined(__sh4a__)
// #    define PROCESSOR_SH_4A
// #  endif
// ���� �� ��������� BYTE_ORDER, ����������� �������������� ����������� ������������������

/*
 ��������� SPARC, �������������� ��������: V9

 SPARC ����� �������� ������� ������ ������ �� ������ 9, � �� ����� ��� ������ 9 ����� �������� ������� ������ �
�������� �������� ������ �� ���������. �����������, ��� ��� ������� SPARC ����� �������� ������� ������.
*/

#elif defined(__sparc__)
#  define PROCESSOR_SPARC
#  if defined(__sparc_v9__) || defined(__sparcv9)
#    define PROCESSOR_SPARC_V9
#  endif
#  if defined(__sparc64__)
#    define PROCESSOR_SPARC_64
#  endif
#  define BYTE_ORDER BIG_ENDIAN

// -- Web Assembly --
#elif defined(__EMSCRIPTEN__)
#  define PROCESSOR_WASM
#  define BYTE_ORDER LITTLE_ENDIAN
#  define PROCESSOR_WORDSIZE 8
#ifdef COMPILER_SUPPORTS_SSE2
#  define PROCESSOR_X86 6   // �������� ��������� SIMD
# define PROCESSOR_X86_64 // wasm64
#  define PROCESSOR_WASM_64
#endif

#endif


/*
����������:
    GCC 4.6 ������� � SVN r165881 __BYTE_ORDER__, __ORDER_BIG_ENDIAN__, _ORDER_LITTLE_ENDIAN__
    � __ORDER_PDP_ENDIAN__. ���� �� ����������� GCC 4.6 ��� �����,
    ���� ��� ��������� ��������� ��� ������� ������� ������; ���� ��� �� ���, �
    ������� __LITTLE_ENDIAN__ ��� __BIG_ENDIAN__ �� ����������, �� ����
    ��� �� ������ ���������� ������� ������� ������.
*/
// ��������� ���������� ������������ ����� �� ��������� ��������, ���������� ����������, ����� �� ��� �� ����������.


#if !defined(BYTE_ORDER)
#  if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == BIG_ENDIAN || __BYTE_ORDER__ == LITTLE_ENDIAN)
// �������� ������������ __BYTE_ORDER__  ��� ����, ��������� ����������� *_ENDIAN ������������� ���������� ������������� �� ���������
#    define BYTE_ORDER __BYTE_ORDER__
#  elif defined(__BIG_ENDIAN__) || defined(_big_endian__) || defined(_BIG_ENDIAN)
#    define BYTE_ORDER BIG_ENDIAN
#  elif defined(__LITTLE_ENDIAN__) || defined(_little_endian__) || defined(_LITTLE_ENDIAN)
#    define BYTE_ORDER LITTLE_ENDIAN
#  else
#    error "Unable to determine byte order!"
#  endif
#endif

/*
 ������ ��������� � ������ ��������� ��������. �� ���������� 64-��������� ������� � �������:
 * GCC � ����������� ����������� (Clang, ICC � OS X � Windows) ������ ����������
 __SIZEOF_POINTER__. ��� ��������� ����������� ��� ��������� ������ ������ ILP32 �� 64-���������
�����������.
 * ����������� ������ Unix-������������ ���������� __LP64__ ��� _LP64 � 64-��������� ������
 (64-��������� Long � ���������)
 * ���� PROCESSOR_WORDSIZE ���� ���������� ����, ��������������, ����� ��������������� �������
 ������.
 � ��������� ������, �� ����� ������� 32-���������, � ����� ��������� � qglobal.cpp ��� ��� ���������.
*/

#if defined __SIZEOF_POINTER__
#  define POINTER_SIZE           __SIZEOF_POINTER__
#elif defined(__LP64__) || defined(_LP64)
#  define POINTER_SIZE           8
#elif defined(PROCESSOR_WORDSIZE)
#  define POINTER_SIZE           PROCESSOR_WORDSIZE
#else
#  define POINTER_SIZE           4
#endif

/*
 ���������� PROCESSOR_WORDSIZE ��� ������ ��������� ����� (������
��� ������ ��������). � ��������� ������������, ��� ��������� ����� ����
������ ��������, ������ ��������� ����.

 ������������ � �������� POINTER_SIZE, ���� ��� ���� �� ������ ��� ���������.
*/
#ifndef PROCESSOR_WORDSIZE
#  define PROCESSOR_WORDSIZE        POINTER_SIZE
#endif

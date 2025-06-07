#pragma once

#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#ifndef __BASE_DECLARE_MEMCPY
#  define __BASE_DECLARE_MEMCPY(name)								\
	BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL name(	\
		void*		destination,									\
		const void* source,											\
		size_t		length) noexcept						
#endif // __BASE_DECLARE_MEMCPY

// =======================================================================================
//									Scalar memcpy
// =======================================================================================

__BASE_DECLARE_MEMCPY(__base_memcpy64BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy64BitUnrolledTo128BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy64BitUnrolledTo256BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy64BitUnrolledTo512BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy64BitUnrolledTo1024BitUnaligned);

// =======================================================================================
//									Aligned xmm memcpy
// =======================================================================================

__BASE_DECLARE_MEMCPY(__base_memcpy128Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo256Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo512Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo1024Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo2048Bit);

// =======================================================================================
//									Unaligned xmm memcpy
// =======================================================================================


__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo256BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo512BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo1024BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy128BitUnrolledTo2048BitUnaligned);


// =======================================================================================
//									Aligned ymm memcpy
// =======================================================================================


__BASE_DECLARE_MEMCPY(__base_memcpy256Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo512Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo1024Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo2048Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo4096Bit);


// =======================================================================================
//									Unaligned ymm memcpy
// =======================================================================================


__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo512BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo1024BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo2048BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy256BitUnrolledTo4096BitUnaligned);


// =======================================================================================
//									Aligned zmm memcpy
// =======================================================================================


__BASE_DECLARE_MEMCPY(__base_memcpy512Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo1024Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo2048Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo4096Bit);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo8192Bit);

// =======================================================================================
//									Unaligned xmm memcpy
// =======================================================================================


__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo1024BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo2048BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo4096BitUnaligned);
__BASE_DECLARE_MEMCPY(__base_memcpy512BitUnrolledTo8192BitUnaligned);

__BASE_MEMORY_NAMESPACE_END
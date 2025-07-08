#pragma once 

#include <src/core/string/traits/StringConverterTraitsBase.h>
#include <src/core/string/StringConversionParameters.h>

#include <src/core/string/StringConversionResult.h>
#include <src/core/memory/MemoryUtility.h>

__BASE_STRING_NAMESPACE_BEGIN

#if !defined(__baseInitConversionParameters)
#  define __baseInitConversionParameters(_FromChar_, alignment) \
	    size_t alignedBytes					= (parameters.stringLength * sizeof(_FromChar_)) & ~size_t(alignment - 1);	\
		const void* stopAt					= parameters.originalInputStringDataStart;									\
		base::memory::AdvanceBytes(stopAt, alignedBytes);
#endif // __baseInitConversionParameters

#if !defined(__baseConvertOrFallback)
#  define __baseConvertOrFallback(_FromChar_, _ToChar_, isNarrowing) \
    if (inputStringVoidPointer == base::memory::UnCheckedToConstChar(parameters.inputStringDataStart + parameters.stringLength)) {\
      return base::string::StringConversionResult<_ToChar_>(parameters.outputStringDataStart, parameters.stringLength, isNarrowing); } \
    return FallbackTraits::convertString<_FromChar_, _ToChar_>( \
      static_cast<const _FromChar_*>(inputStringVoidPointer), \
      parameters.stringLength - (alignedBytes / sizeof(_FromChar_)), \
      reinterpret_cast<_ToChar_*>(outputStringVoidPointer)); 
#endif // __baseConvertOrFallback


template <CpuFeature _SimdType_> // ,class _NarrowingConversionBehaviour_>
class StringConverterTraits
{
	//static_assert(
		//false,
		//"base::core::string::StringConverterTraits<_SimdType_, _NarrowingConversionBehaviour_>: Invalid _SimdType_");
};

//template <class _NarrowingConversionBehaviour>
//class StringConverterTraits<CpuFeature::AVX512, _NarrowingConversionBehaviour>;
//
//template <class _NarrowingConversionBehaviour_>
//class StringConverterTraits<CpuFeature::AVX, _NarrowingConversionBehaviour_>;
//
//template <class _NarrowingConversionBehaviour_>
//class StringConverterTraits<CpuFeature::SSE, _NarrowingConversionBehaviour_>;
//
//template <class _NarrowingConversionBehaviour_>
//class StringConverterTraits<CpuFeature::None, _NarrowingConversionBehaviour_>;

__BASE_STRING_NAMESPACE_END


#pragma once 

#include <src/core/string/traits/StringConverterTraitsBase.h>
#include <src/core/string/StringConversionParameters.h>

#include <src/core/string/StringConversionResult.h>

__BASE_STRING_NAMESPACE_BEGIN

#if !defined(baseInitConversionPointers)
#  define baseInitConversionPointers(_CharType_, parameters, alignment) \
	    size_t avx512SizeInBytes = (parameters.stringLength * sizeof(_CharType_)) & ~size_t(alignment - 1); \
		void* stopAt = parameters.inputStringDataStart; \
		void* outputStringVoidPointer = parameters.outputStringDataStart; \
		const void* inputStringVoidPointer = parameters.inputStringDataStart; \
		memory::AdvanceBytes(stopAt, avx512SizeInBytes);
#endif


template <
	CpuFeature _SimdType_,
	class _NarrowingConversionBehaviour_>
class StringConverterTraits: 
	public StringConverterTraitsBase<
		_SimdType_, 
		_NarrowingConversionBehaviour_> 
{
	static_assert(
		false,
		"base::core::string::StringConverterTraits<_SimdType_, _NarrowingConversionBehaviour_>: Invalid _SimdType_");
};


__BASE_STRING_NAMESPACE_END

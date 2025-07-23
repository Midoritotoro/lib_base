#pragma once 

#include <src/core/string/StringConversionParameters.h>

#include <src/core/string/StringConversionResult.h>
#include <src/core/memory/MemoryUtility.h>

__BASE_STRING_NAMESPACE_BEGIN

template <arch::CpuFeature _SimdType_> // ,class _NarrowingConversionBehaviour_>
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


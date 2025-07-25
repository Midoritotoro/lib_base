#include <src/core/string/crt/cs/internal/BaseStrstrnInternal.h>

#include <src/core/string/crt/cs/internal/BaseStrstrnAnySizeInternal.h>
#include <src/core/string/crt/cs/internal/BaseStrstrnMemcmpInternal.h>

#include <src/core/string/crt/cs/internal/BaseStrstrnEqualInternal.h>

#include <src/core/memory/crt/FixedMemcmp.h>


__BASE_STRING_NAMESPACE_BEGIN

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::None>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
	if (mainLength == subLength)
        return (memcmp(mainString, subString, mainLength) == 0) ? mainString : nullptr;

    const char first        = subString[0];
    const sizetype maxpos   = sizetype(mainLength) - sizetype(subLength) + 1;
    
    for (sizetype i = 0; i < maxpos; i++) {
        if (mainString[i] != first) {
            i++;

            while (i < maxpos && mainString[i] != first)
                i++;

            if (i == maxpos)
                break;
        }

        sizetype j = 1;
        
        for (; j < subLength; ++j)
            if (mainString[i + j] != subString[j])
                break;
        
        if (j == subLength)
            return (mainString + i);
    }

    return nullptr;
}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::AVX512F>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
    const char* result = nullptr;

    if (mainLength < subLength)
        return result;

    switch (subLength) {
        case 0:
            return 0;

        case 1: {   
            const char* res = reinterpret_cast<const char*>(strchr(mainString, subString[0]));
            return res; 
        }

        case 2:     
            result = BaseFeatureAwareStrstrnMemcmp<2, decltype(memory::memcmp2), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp2);
            break;

        case 3:     
            result = BaseFeatureAwareStrstrnMemcmp<3, decltype(memory::memcmp3), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp3);
            break;

        case 4:     
            result = BaseFeatureAwareStrstrnMemcmp<4, decltype(memory::memcmp4), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 5:    
            result = BaseFeatureAwareStrstrnMemcmp<5, decltype(memory::memcmp5), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp5);
            break;

        case 6:    
            result = BaseFeatureAwareStrstrnMemcmp<6, decltype(memory::memcmp6), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp6);
            break;

        case 7:
            result = BaseFeatureAwareStrstrnMemcmp<7, decltype(memory::memcmp7), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp7);
            break;

        case 8:     
            result = BaseFeatureAwareStrstrnMemcmp<8, decltype(memory::memcmp8), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 9:     
            result = BaseFeatureAwareStrstrnMemcmp<9, decltype(memory::memcmp9), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp9);
            break;

        case 10:   
            result = BaseFeatureAwareStrstrnMemcmp<10, decltype(memory::memcmp10), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp10);
            break;

        case 11: 
            result = BaseFeatureAwareStrstrnMemcmp<11, decltype(memory::memcmp11), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp11);
            break;

        case 12:  
            result = BaseFeatureAwareStrstrnMemcmp<12, decltype(memory::memcmp12), arch::CpuFeature::AVX512F>(mainString, mainLength, subString, memory::memcmp12);
            break;

        default:   
            result = BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::AVX512F>(mainString, mainLength, subString, subLength);
            break;
    }

    if (result - mainString <= mainLength - subLength)
        return result;

    return nullptr;
}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::AVX2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
    const char* result = nullptr;

    if (mainLength < subLength)
        return result;

    /*switch (subLength) {
        case 0:
            return 0;

        case 1: {   
            const char* res = reinterpret_cast<const char*>(strchr(mainString, subString[0]));
            return res; 
        }

        case 2:     
            result = BaseFeatureAwareStrstrnEqual<arch::CpuFeature::AVX2, 2>(mainString, mainLength, subString);   
            break;

        case 3:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 3>(mainString, mainLength, subString, memory::memcmp1);
            break;

        case 4:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 4>(mainString, mainLength, subString, memory::memcmp2);
            break;

        case 5:    
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 5>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 6:    
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 6>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 7:
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 7>(mainString, mainLength, subString, memory::memcmp5);
            break;

        case 8:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 8>(mainString, mainLength, subString, memory::memcmp6);
            break;

        case 9:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 9>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 10:   
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 10>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 11: 
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 11>(mainString, mainLength, subString, memory::memcmp9);
            break;

        case 12:  
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::AVX2, 12>(mainString, mainLength, subString, memory::memcmp10);
            break;

        default:   
            result = BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::AVX2>(mainString, mainLength, subString, subLength);
            break;
    }*/

    if (result - mainString <= mainLength - subLength)
        return result;

    return nullptr;
}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrn<arch::CpuFeature::SSE2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
    const char* result = nullptr;

    if (mainLength < subLength)
        return result;

    /*switch (subLength) {
        case 0:
            return 0;

        case 1: {   
            const char* res = reinterpret_cast<const char*>(strchr(mainString, subString[0]));
            return res; 
        }

        case 2:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 2>(mainString, mainLength, subString, memory::alwaysTrue);   
            break;

        case 3:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 3>(mainString, mainLength, subString, memory::memcmp1);
            break;

        case 4:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 4>(mainString, mainLength, subString, memory::memcmp2);
            break;

        case 5:    
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 5>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 6:    
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 6>(mainString, mainLength, subString, memory::memcmp4);    
            break;

        case 7:
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 7>(mainString, mainLength, subString, memory::memcmp5);
            break;

        case 8:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 8>(mainString, mainLength, subString, memory::memcmp6);     
            break;

        case 9:     
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 9>(mainString, mainLength, subString, memory::memcmp8);      
            break;

        case 10:   
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 10>(mainString, mainLength, subString, memory::memcmp8);   
            break;

        case 11: 
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 11>(mainString, mainLength, subString, memory::memcmp9);
            break;

        case 12:  
            result = BaseFeatureAwareStrstrnMemcmp<arch::CpuFeature::SSE2, 12>(mainString, mainLength, subString, memory::memcmp10);  
            break;

        default:   
            result = BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::SSE2>(mainString, mainLength, subString, subLength);          
            break;
    }*/

    if (result - mainString <= mainLength - subLength)
        return result;

    return nullptr;
}

__BASE_STRING_NAMESPACE_END

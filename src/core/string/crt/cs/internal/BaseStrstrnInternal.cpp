#include <src/core/string/crt/cs/internal/BaseStrstrnInternal.h>

#include <src/core/string/crt/cs/internal/BaseStrstrnImplementationsInternal.h>
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
    using _Implementation_ = BaseStrstrnImplementationsInternal<arch::CpuFeature::AVX512F>;

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
            result = _Implementation_::Memcmp<2>(mainString, mainLength, subString, memory::memcmp2);
            break;

        case 3:     
            result = _Implementation_::Memcmp<3>(mainString, mainLength, subString, memory::memcmp3);
            break;

        case 4:     
            result = _Implementation_::Memcmp<4>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 5:    
            result = _Implementation_::Memcmp<5>(mainString, mainLength, subString, memory::memcmp5);
            break;

        case 6:    
            result = _Implementation_::Memcmp<6>(mainString, mainLength, subString, memory::memcmp6);
            break;

        case 7:
            result = _Implementation_::Memcmp<7>(mainString, mainLength, subString, memory::memcmp7);
            break;

        case 8:     
            result = _Implementation_::Memcmp<8>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 9:     
            result = _Implementation_::Memcmp<9>(mainString, mainLength, subString, memory::memcmp9);
            break;

        case 10:   
            result = _Implementation_::Memcmp<10>(mainString, mainLength, subString, memory::memcmp10);
            break;

        case 11: 
            result = _Implementation_::Memcmp<11>(mainString, mainLength, subString, memory::memcmp11);
            break;

        case 12:  
            result = _Implementation_::Memcmp<12>(mainString, mainLength, subString, memory::memcmp12);
            break;

        default:   
            result = _Implementation_::AnySize(mainString, mainLength, subString, subLength);
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
    using _Implementation_ = BaseStrstrnImplementationsInternal<arch::CpuFeature::AVX2>;

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
            result = _Implementation_::Equal<2>(mainString, mainLength, subString);
            break;

        case 3:     
            result = _Implementation_::Memcmp<3>(mainString, mainLength, subString, memory::memcmp1);
            break;

        case 4:     
            result = _Implementation_::Memcmp<4>(mainString, mainLength, subString, memory::memcmp2);
            break;

        case 5:    
            result = _Implementation_::Memcmp<5>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 6:    
            result = _Implementation_::Memcmp<6>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 7:
            result = _Implementation_::Memcmp<7>(mainString, mainLength, subString, memory::memcmp5);
            break;

        case 8:     
            result = _Implementation_::Memcmp<8>(mainString, mainLength, subString, memory::memcmp6);
            break;

        case 9:     
            result = _Implementation_::Memcmp<9>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 10:   
            result = _Implementation_::Memcmp<10>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 11: 
            result = _Implementation_::Memcmp<11>(mainString, mainLength, subString, memory::memcmp9);
            break;

        case 12:  
            result = _Implementation_::Memcmp<12>(mainString, mainLength, subString, memory::memcmp10);
            break;

        default:   
            result = _Implementation_::AnySize(mainString, mainLength, subString, subLength);
            break;
    }

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
    using _Implementation_ = BaseStrstrnImplementationsInternal<arch::CpuFeature::AVX2>;

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
            result = _Implementation_::Memcmp<2>(mainString, mainLength, subString, memory::alwaysTrue);
            break;

        case 3:     
            result = _Implementation_::Memcmp<3>(mainString, mainLength, subString, memory::memcmp1);
            break;

        case 4:     
            result = _Implementation_::Memcmp<4>(mainString, mainLength, subString, memory::memcmp2);
            break;

        case 5:    
            result = _Implementation_::Memcmp<5>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 6:    
            result = _Implementation_::Memcmp<6>(mainString, mainLength, subString, memory::memcmp4);
            break;

        case 7:
            result = _Implementation_::Memcmp<7>(mainString, mainLength, subString, memory::memcmp5);
            break;

        case 8:     
            result = _Implementation_::Memcmp<8>(mainString, mainLength, subString, memory::memcmp6);
            break;

        case 9:     
            result = _Implementation_::Memcmp<9>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 10:   
            result = _Implementation_::Memcmp<10>(mainString, mainLength, subString, memory::memcmp8);
            break;

        case 11: 
            result = _Implementation_::Memcmp<11>(mainString, mainLength, subString, memory::memcmp9);
            break;

        case 12:  
            result = _Implementation_::Memcmp<12>(mainString, mainLength, subString, memory::memcmp10);
            break;

        default:   
            result = _Implementation_::AnySize(mainString, mainLength, subString, subLength);
            break;
    }

    if (result - mainString <= mainLength - subLength)
        return result;

    return nullptr;
}

__BASE_STRING_NAMESPACE_END

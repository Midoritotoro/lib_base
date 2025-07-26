#pragma once 

#include <base/core/arch/Platform.h>

#include <src/core/utility/simd/SimdHelpers.h>
#include <base/core/utility/BitOps.h>

#include <src/core/string/crt/cs/internal/BaseStrstrClangMsvcHelper.h>

__BASE_STRING_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_CLASS(
	template <arch::CpuFeature feature>
	class BaseStrstrnImplementationsInternal,
	feature,
	"base::string",
	arch::CpuFeature::AVX512F, arch::CpuFeature::AVX2, arch::CpuFeature::SSE2
);

#define _mm512_set1_epu8(c) _mm512_set1_epi32(uint32_t(c) * 0x01010101u)

template <>
class BaseStrstrnImplementationsInternal<arch::CpuFeature::AVX512F> {
public:
	template <
		sizetype needleLength,
		typename _MemCmpLike_>
	static DECLARE_NOALIAS const char* Memcmp(
		const char*		mainString,
		const sizetype	mainLength,
		const char*		subString,
		_MemCmpLike_	memcmpLike) noexcept
	{
		if constexpr (needleLength <= 0)
			return nullptr;

		if (mainLength <= 0)
			return nullptr;

		const auto first = _mm512_set1_epi8(subString[0]);
		const auto last = _mm512_set1_epi8(subString[needleLength - 1]);

		char* haystack = const_cast<char*>(mainString);
		char* end = haystack + mainLength;

		for (; haystack < end; haystack += 64) {

			const auto block_first = _mm512_loadu_si512(haystack + 0);
			const auto block_last = _mm512_loadu_si512(haystack + needleLength - 1);

			const auto first_zeros = _mm512_xor_si512(block_first, first);
			const auto zeros = _mm512_ternarylogic_epi32(first_zeros, block_last, last, 0xf6);

			uint32_t mask = ZeroByteMask(zeros);

			while (mask) {

				const uint64_t p = CountTrailingZeroBits(mask);

				if (memcmpLike(haystack + 4 * p + 0, subString))
					return mainString + ((haystack - mainString) + 4 * p + 0);

				if (memcmpLike(haystack + 4 * p + 1, subString))
					return mainString + ((haystack - mainString) + 4 * p + 1);

				if (memcmpLike(haystack + 4 * p + 2, subString))
					return mainString + ((haystack - mainString) + 4 * p + 2);

				if (memcmpLike(haystack + 4 * p + 3, subString))
					return mainString + ((haystack - mainString) + 4 * p + 3);

				mask = ClearLeftMostSet(mask);
			}
		}

		return nullptr;
	}

	static DECLARE_NOALIAS const char* AnySize(
		const char*		mainString,
		const sizetype	mainLength,
		const char*		subString,
		const sizetype	subLength) noexcept
	{
		if (mainLength <= 0 || subLength <= 0)
			return nullptr;

		const auto first = _mm512_set1_epu8(subString[0]);
		const auto last = _mm512_set1_epu8(subString[subLength - 1]);

		char* haystack = const_cast<char*>(mainString);
		char* end = haystack + mainLength;

		for (; haystack < end; haystack += 64) {

			const auto blockFirst = _mm512_loadu_si512(haystack + 0);
			const auto blockLast = _mm512_loadu_si512(haystack + subLength - 1);

			const auto firstZeros = _mm512_xor_si512(blockFirst, first);

			/*
				first_zeros | block_last | last |  first_zeros | (block_last ^ last)
				------------+------------+------+------------------------------------
					 0      |      0     |   0  |      0
					 0      |      0     |   1  |      1
					 0      |      1     |   0  |      1
					 0      |      1     |   1  |      0
					 1      |      0     |   0  |      1
					 1      |      0     |   1  |      1
					 1      |      1     |   0  |      1
					 1      |      1     |   1  |      1
			*/

			const auto zeros = _mm512_ternarylogic_epi32(firstZeros, blockLast, last, 0xf6);
			uint32_t mask = ZeroByteMask(zeros);

			while (mask) {

				const uint64_t p = CountTrailingZeroBits(mask);

				if (memcmp(haystack + 4 * p + 0, subString, subLength) == 0)
					return mainString + ((haystack - mainString) + 4 * p + 0);

				if (memcmp(haystack + 4 * p + 1, subString, subLength) == 0)
					return mainString + ((haystack - mainString) + 4 * p + 1);

				if (memcmp(haystack + 4 * p + 2, subString, subLength) == 0)
					return mainString + ((haystack - mainString) + 4 * p + 2);

				if (memcmp(haystack + 4 * p + 3, subString, subLength) == 0)
					return mainString + ((haystack - mainString) + 4 * p + 3);

				mask = ClearLeftMostSet(mask);
			}
		}

		return nullptr;
	}
};

template <>
class BaseStrstrnImplementationsInternal<arch::CpuFeature::AVX2> {
public:
	template <
		sizetype needleLength,
		typename _MemCmpLike_>
	static DECLARE_NOALIAS const char* Memcmp(
		const char*		mainString,
		const sizetype	mainLength,
		const char*		subString,
		_MemCmpLike_	memcmpLike) noexcept
	{
		if constexpr (needleLength <= 0)
			return nullptr;

		if (mainLength <= 0)
			return nullptr;

		const auto first = _mm256_set1_epi8(subString[0]);
		const auto last = _mm256_set1_epi8(subString[needleLength - 1]);

		for (sizetype i = 0; i < mainLength; i += 32) {

			const auto blockFirst   = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i));
			const auto blockLast    = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i + needleLength - 1));

			const auto equalFirst = _mm256_cmpeq_epi8(first, blockFirst);
			const auto equalLast = _mm256_cmpeq_epi8(last, blockLast);

			uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(equalFirst, equalLast));

			while (mask != 0) {
				const auto bitpos = CountTrailingZeroBits(mask);

				if (memcmpLike(mainString + i + bitpos + 1, subString + 1))
					return mainString + i + bitpos;
           
				mask = ClearLeftMostSet(mask);
			}
		}

		return nullptr;
	}

	static DECLARE_NOALIAS const char* AnySize(
		const char*		mainString,
		const sizetype	mainLength,
		const char*		subString,
		const sizetype	subLength) noexcept
	{
		if (subLength <= 0 || mainLength <= 0)
			return nullptr;

		const auto first    = _mm256_set1_epi8(subString[0]);
		const auto last     = _mm256_set1_epi8(subString[subLength - 1]);

		for (sizetype i = 0; i < mainLength; i += 32) {
			const auto blockFirst   = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i));
			const auto blockLast    = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i + subLength - 1));

			const auto equalFirst   = _mm256_cmpeq_epi8(first, blockFirst);
			const auto equalLast    = _mm256_cmpeq_epi8(last, blockLast);

			uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(equalFirst, equalLast));

			while (mask != 0) {
				const auto bitpos = CountTrailingZeroBits(mask);

				if (memcmp(mainString + i + bitpos + 1, subString + 1, subLength - 2) == 0)
					return mainString + i + bitpos;
            
				mask = ClearLeftMostSet(mask);
			}
		}

		return nullptr;
	}

	template <sizetype needleLength>
	static DECLARE_NOALIAS NODISCARD const char* Equal(
		const char* string,
		sizetype    stringLength,
		const char* needle) noexcept
	{
		static_assert(needleLength > 0 && needleLength < 16, "needleLength must be in range [1..15]");

		if (stringLength <= 0)
			return nullptr;

		__m256i broadcasted[needleLength];

		for (unsigned i = 0; i < needleLength; i++)
			broadcasted[i] = _mm256_set1_epi8(needle[i]);

		auto curr = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string));

		for (sizetype i = 0; i < needleLength; i += 32) {

			const auto next = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i + 32));
			auto equal = _mm256_cmpeq_epi8(curr, broadcasted[0]);

			// AVX2 palignr works on 128-bit lanes, thus some extra work is needed
			//
			// curr = [a, b] (2 x 128 bit)
			// next = [c, d]
			// substring = [palignr(b, a, i), palignr(c, b, i)]
			__m256i next1;

			next1 = _mm256_inserti128_si256(next1, _mm256_extracti128_si256(curr, 1), 0); // b
			next1 = _mm256_inserti128_si256(next1, _mm256_extracti128_si256(next, 0), 1); // c

#if !defined(CPP_CLANG) && !defined(CPP_MSVC)
			for (unsigned i = 1; i < needleLength; i++) {
				const auto substring = _mm256_alignr_epi8(next1, curr, i);
				equal = _mm256_and_si256(equal, _mm256_cmpeq_epi8(substring, broadcasted[i]));
			}
#else
			StringFindLoop<needleLength>()(equal, next1, curr, broadcasted);
#endif

			curr = next;
			const uint32_t mask = _mm256_movemask_epi8(equal);

			if (mask != 0)
				return string + i + CountTrailingZeroBits(mask);
		}

		return nullptr;
	}
};


template <>
class BaseStrstrnImplementationsInternal<arch::CpuFeature::SSE2> {
public:
	template <
		sizetype needleLength,
		typename _MemCmpLike_>
	static DECLARE_NOALIAS const char* Memcmp(
		const char* mainString,
		const sizetype	mainLength,
		const char* subString,
		_MemCmpLike_	memcmpLike) noexcept
	{
		if constexpr (needleLength <= 0)
			return nullptr;
			
		if (mainLength <= 0)
			return nullptr;
			
		const auto first    = _mm_set1_epi8(subString[0]);
		const auto last     = _mm_set1_epi8(subString[needleLength - 1]);
			
		for (sizetype i = 0; i < mainLength; i += 16) {
			
			const auto blockFirst   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(mainString + i));
			const auto blockLast    = _mm_loadu_si128(reinterpret_cast<const __m128i*>(mainString + i + needleLength - 1));
			
			const auto equalFirst   = _mm_cmpeq_epi8(first, blockFirst);
			const auto equalLast    = _mm_cmpeq_epi8(last, blockLast);
			
			uint32_t mask = _mm_movemask_epi8(_mm_and_si128(equalFirst, equalLast));
			
			while (mask != 0) {
			
			    const auto bitpos = CountTrailingZeroBits(mask);
			
			    if (memcmpLike(mainString + i + bitpos + 1, subString + 1))
			        return mainString + i + bitpos;
			
			    mask = ClearLeftMostSet(mask);
			}
		}
			
		return nullptr;
	}

	static DECLARE_NOALIAS const char* AnySize(
		const char*		mainString,
		const sizetype	mainLength,
		const char*		subString,
		const sizetype	subLength) noexcept
	{
		if (mainLength <= 0 || subLength <= 0)
			return nullptr;

		const auto first = _mm_set1_epi8(subString[0]);
		const auto last = _mm_set1_epi8(subString[subLength - 1]);

		for (sizetype i = 0; i < mainLength; i += 16) {

			const auto blockFirst = _mm_loadu_si128(reinterpret_cast<const __m128i*>(mainString + i));
			const auto blockLast = _mm_loadu_si128(reinterpret_cast<const __m128i*>(mainString + i + subLength - 1));

			const auto equalFirst = _mm_cmpeq_epi8(first, blockFirst);
			const auto equalLast = _mm_cmpeq_epi8(last, blockLast);

			uint16_t mask = _mm_movemask_epi8(_mm_and_si128(equalFirst, equalLast));

			while (mask != 0) {

				const auto bitpos = CountTrailingZeroBits(mask);

				if (memcmp(mainString + i + bitpos + 1, subString + 1, subLength - 2) == 0)
					return mainString + i + bitpos;

				mask = ClearLeftMostSet(mask);
			}
		}

		return nullptr;
	}
};


__BASE_STRING_NAMESPACE_END

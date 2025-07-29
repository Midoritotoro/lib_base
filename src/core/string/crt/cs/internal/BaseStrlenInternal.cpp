#include <src/core/string/crt/cs/internal/BaseStrlenInternal.h>
#include <src/core/string/crt/BaseStrlenCheckForZeroBytes.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

__BASE_STRING_NAMESPACE_BEGIN

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::None>(const char* string) noexcept {
	const char* char_ptr;
	const unsigned long int* longword_ptr;
	unsigned long int longword, himagic, lomagic;

	for (char_ptr = string; ((unsigned long int) char_ptr
		& (sizeof(longword) - 1)) != 0;
		++char_ptr)
		if (*char_ptr == '\0')
			return char_ptr - string;

	longword_ptr = (unsigned long int*) char_ptr;

	himagic = 0x80808080L;
	lomagic = 0x01010101L;

	if (sizeof(longword) > 4)
	{
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
	}

	if (sizeof(longword) > 8)
		std::abort();

	for (;;)
	{
		longword = *longword_ptr++;

		if (((longword - lomagic) & ~longword & himagic) != 0)
		{
			const char* cp = (const char*)(longword_ptr - 1);

			if (cp[0] == 0)
				return cp - string;
			if (cp[1] == 0)
				return cp - string + 1;
			if (cp[2] == 0)
				return cp - string + 2;
			if (cp[3] == 0)
				return cp - string + 3;
			if (sizeof(longword) > 4)
			{
				if (cp[4] == 0)
					return cp - string + 4;
				if (cp[5] == 0)
					return cp - string + 5;
				if (cp[6] == 0)
					return cp - string + 6;
				if (cp[7] == 0)
					return cp - string + 7;
			}
		}
	}
}

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::SSE2>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto mask = __checkForZeroBytes<arch::CpuFeature::SSE2, 1>(_mm_loadu_si128(reinterpret_cast<const __m128i*>(current)));

		if (mask != 0)
			return reinterpret_cast<const char*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX2>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto mask = __checkForZeroBytes<arch::CpuFeature::AVX2, 1>(_mm256_lddqu_si256(reinterpret_cast<const __m256i*>(current)));

		if (mask != 0)
			return reinterpret_cast<const char*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX512BW>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto mask = __checkForZeroBytes<arch::CpuFeature::AVX512BW, 1>(_mm512_loadu_epi8(current));

		if (mask != 0)
			return reinterpret_cast<const char*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

__BASE_STRING_NAMESPACE_END

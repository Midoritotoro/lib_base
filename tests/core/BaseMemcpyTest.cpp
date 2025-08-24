#include <src/core/memory/crt/BaseMemcpy.h>
#include <src/core/memory/crt/BaseMemmove.h>

#include <vector>
#include <src/core/utility/simd/traits/SimdCountTraits.h>


int main(int argc, char** argv) {
	//std::vector<int> oldVector;
	//oldVector.resize(1000000000);

	//std::vector<int> newVector;
	//newVector.resize(1000000000);

	//std::cout << newVector.size() * sizeof(int);
	//base::memory::__base_memcpy(newVector.data(), oldVector.data(), newVector.size() * sizeof(int));
	////memcpy(newVector.data(), oldVector.data(), newVector.size() * sizeof(int));

	//Assert(oldVector == newVector);
	//auto b = _mm256_add_epi8(_mm512_castsi512_si256(_mm512_set1_epi8(10)), _mm512_extracti64x4_epi64(_mm512_set1_epi8(10), 1));
	//auto d = _mm_add_epi8(_mm256_castsi256_si128(b), _mm256_extracti64x2_epi64(b, 1));

	std::cout << base::CountTraits<base::arch::CpuFeature::AVX512F>::Reduce<1>(_mm512_set1_epi8(10));

	return 0;
}
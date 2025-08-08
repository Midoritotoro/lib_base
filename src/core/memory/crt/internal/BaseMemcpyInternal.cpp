#include <src/core/memory/crt/internal/BaseMemcpyInternal.h>
#include <src/core/memory/crt/internal/BaseMemcpyImplementationInternal.h>

#include <base/core/arch/ProcessorFeatures.h>

__BASE_MEMORY_NAMESPACE_BEGIN


template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::None>(
	void*				destination,
	const void*         source,
	sizetype            size) noexcept
{
    using _None_ = BaseMemcpyImplementationInternal<arch::CpuFeature::None>;

    void* returnval = destination; // memcpy is supposed to return the destination
    sizetype offset = 0; // Offset size needs to match the size of a pointer

    while (size)
        // The biggest sizes will go first for alignment. There's no benefit to using
        // aligned loads over unaligned loads here, so all are unaligned.
        // NOTE: Each memcpy has its own loop so that any one can be used individually.
    {
        if (size < 2) // 1 byte
        {
            _None_::Memcpy<1, false>(destination, source, size);
            offset = size & -1;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size = 0;

        }
        else if (size < 4) // 2 bytes
        {
            _None_::Memcpy<2, false>(destination, source, size >> 1);
            offset = size & -2;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 1;
        }
        else if (size < 8) // 4 bytes
        {
            _None_::Memcpy<4, false>(destination, source, size >> 2);
            offset = size & -4;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 3;
        }
        else // 8 bytes
        {
            _None_::Memcpy<8, false>(destination, source, size >> 3);
            offset = size & -8;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 7;
        }
    }

    return returnval;
}

template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::SSE>(
	void*				destination,
	const void*         source,
	sizetype            size) noexcept
{
    using _None_ = BaseMemcpyImplementationInternal<arch::CpuFeature::None>;

    using _Sse_ = BaseMemcpyImplementationInternal<arch::CpuFeature::SSE>;

    void* returnval = destination; // memcpy is supposed to return the destination
    sizetype offset = 0; // Offset size needs to match the size of a pointer

    while (size)
        // The biggest sizes will go first for alignment. There's no benefit to using
        // aligned loads over unaligned loads here, so all are unaligned.
        // NOTE: Each memcpy has its own loop so that any one can be used individually.
    {
        if (size < 2) // 1 byte
        {
            _None_::Memcpy<1, false>(destination, source, size);
            offset = size & -1;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size = 0;

        }
        else if (size < 4) // 2 bytes
        {
            _None_::Memcpy<2, false>(destination, source, size >> 1);
            offset = size & -2;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 1;
        }
        else if (size < 8) // 4 bytes
        {
            _None_::Memcpy<4, false>(destination, source, size >> 2);
            offset = size & -4;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 3;
        }
        else if (size < 16) // 8 bytes
        {
            _None_::Memcpy<8, false>(destination, source, size >> 3);
            offset = size & -8;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 7;
        }
        else if (size < 32) // 16 bytes
        {
            _Sse_::Memcpy<16, false>(destination, source, size >> 4);
            offset = size & -16;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 15;
        }
        else if (size < 64) // 32 bytes
        {
            _Sse_::Memcpy<32, false>(destination, source, size >> 5);
            offset = size & -32;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 31;
        }
        else if (size < 128) // 64 bytes
        {
            _Sse_::Memcpy<64, false>(destination, source, size >> 6);
            offset = size & -64;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 63;
        }
        else if (size < 256) // 128 bytes
        {
            _Sse_::Memcpy<128, false>(destination, source, size >> 7);
            offset = size & -128;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 127;
        }
        else // 256 bytes
        {
            _Sse_::Memcpy<256, false>(destination, source, size >> 8);
            offset = size & -256;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 255;
        }
    }

    return returnval;
}

template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::AVX>(
	void*				destination,
	const void*         source,
	sizetype            size) noexcept
{
    using _None_ = BaseMemcpyImplementationInternal<arch::CpuFeature::None>;

    using _Sse_ = BaseMemcpyImplementationInternal<arch::CpuFeature::SSE>;
    using _Avx_ = BaseMemcpyImplementationInternal<arch::CpuFeature::AVX>;

    void* returnval = destination; // memcpy is supposed to return the destination
    sizetype offset = 0; // Offset size needs to match the size of a pointer

    while (size)
        // The biggest sizes will go first for alignment. There's no benefit to using
        // aligned loads over unaligned loads here, so all are unaligned.
        // NOTE: Each memcpy has its own loop so that any one can be used individually.
    {
        if (size < 2) // 1 byte
        {
            _None_::Memcpy<1, false>(destination, source, size);
            offset = size & -1;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size = 0;

        }
        else if (size < 4) // 2 bytes
        {
            _None_::Memcpy<2, false>(destination, source, size >> 1);
            offset = size & -2;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 1;
        }
        else if (size < 8) // 4 bytes
        {
            _None_::Memcpy<4, false>(destination, source, size >> 2);
            offset = size & -4;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 3;
        }
        else if (size < 16) // 8 bytes
        {
            _None_::Memcpy<8, false>(destination, source, size >> 3);
            offset = size & -8;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 7;
        }
        else if (size < 32) // 16 bytes
        {
            _Sse_::Memcpy<16, false>(destination, source, size >> 4);
            offset = size & -16;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 15;
        }
        else if (size < 64) // 32 bytes
        {
            _Avx_::Memcpy<32, false>(destination, source, size >> 5);
            offset = size & -32;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 31;
        }
        else if (size < 128) // 64 bytes
        {
            _Avx_::Memcpy<64, false>(destination, source, size >> 6);
            offset = size & -64;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 63;
        }
        else if (size < 256) // 128 bytes
        {
            _Avx_::Memcpy<128, false>(destination, source, size >> 7);
            offset = size & -128;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 127;
        }
        else if (size < 512) // 256 bytes
        {
            _Avx_::Memcpy<256, false>(destination, source, size >> 8);
            offset = size & -256;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 255;
        }
        else // 512 bytes
        {
            _Avx_::Memcpy<512, false>(destination, source, size >> 9);
            offset = size & -512;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 511;
        }
    }

    return returnval;
}

template <>
base_declare_const_function base_nodiscard void* BaseFeatureAwareMemcpy<arch::CpuFeature::AVX512F>(
	void*				destination,
	const void*         source,
	sizetype            size) noexcept
{
	using _None_ = BaseMemcpyImplementationInternal<arch::CpuFeature::None>;

    using _Sse_ = BaseMemcpyImplementationInternal<arch::CpuFeature::SSE>;
    using _Avx_ = BaseMemcpyImplementationInternal<arch::CpuFeature::AVX>;
    using _Avx512F_ = BaseMemcpyImplementationInternal<arch::CpuFeature::AVX512F>;
	
    void* returnval = destination; // memcpy is supposed to return the destination
    sizetype offset = 0; // Offset size needs to match the size of a pointer

    while (size)
        // The biggest sizes will go first for alignment. There's no benefit to using
        // aligned loads over unaligned loads here, so all are unaligned.
        // NOTE: Each memcpy has its own loop so that any one can be used individually.
    {
        if (size < 2) // 1 byte
        {
            _None_::Memcpy<1, false>(destination, source, size);
            offset = size & -1;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size = 0;
            
        }
        else if (size < 4) // 2 bytes
        {
            _None_::Memcpy<2, false>(destination, source, size >> 1);
            offset = size & -2;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 1;
        }
        else if (size < 8) // 4 bytes
        {
            _None_::Memcpy<4, false>(destination, source, size >> 2);
            offset = size & -4;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 3;
        }
        else if (size < 16) // 8 bytes
        {
            _None_::Memcpy<8, false>(destination, source, size >> 3);
            offset = size & -8;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 7;
        }
        else if (size < 32) // 16 bytes
        {
            _Sse_::Memcpy<16, false>(destination, source, size >> 4);
            offset = size & -16;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 15;
        }
        else if (size < 64) // 32 bytes
        {
            _Avx_::Memcpy<32, false>(destination, source, size >> 5);
            offset = size & -32;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 31;
        }
        else if (size < 128) // 64 bytes
        {
            _Avx512F_::Memcpy<64, false>(destination, source, size >> 6);
            offset = size & -64;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 63;
        }
        else if (size < 256) // 128 bytes
        {
            _Avx512F_::Memcpy<128, false>(destination, source, size >> 7);
            offset = size & -128;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 127;
        }
        else if (size < 512) // 256 bytes
        {
            _Avx512F_::Memcpy<256, false>(destination, source, size >> 8);
            offset = size & -256;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 255;
        }
        else if (size < 1024) // 512 bytes
        {
            _Avx512F_::Memcpy<512, false>(destination, source, size >> 9);
            offset = size & -512;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 511;
        }
        else if (size < 2048) // 1024 bytes (1 kB)
        {
            _Avx512F_::Memcpy<1024, false>(destination, source, size >> 10);
            offset = size & -1024;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 1023;
        }
        else if (size < 4096) // 2048 bytes (2 kB)
        {
            _Avx512F_::Memcpy<2048, false>(destination, source, size >> 11);
            offset = size & -2048;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 2047;
        }
        else if (size < 8192) // 4096 bytes (4 kB)
        {
            _Avx512F_::Memcpy<4096, false>(destination, source, size >> 12);
            offset = size & -4096;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 4095;
        }
        else if (size < 16384) // 8192 bytes (8 kB)
        {
            _Avx512F_::Memcpy<8192, false>(destination, source, size >> 13);
            offset = size & -8192;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 8191;
        }
        else if (size < 32768) // 8192 bytes (8 kB)
        {
            _Avx512F_::Memcpy<16384, false>(destination, source, size >> 14);
            offset = size & -16384;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 16383;
        }
        else 
        {
            _Avx512F_::Memcpy<32768, false>(destination, source, size >> 15);
            offset = size & -32768;
            destination = (char*)destination + offset;
            source = (char*)source + offset;
            size &= 32767;
        }

    }

    return returnval;
}

__BASE_MEMORY_NAMESPACE_END

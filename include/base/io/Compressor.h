#pragma once 

#ifdef LIB_BASE_ENABLE_ZLIB
#include <zlib.h>


namespace base::io {
	class Compressor {
	public:
        static [[nodiscard]] int Compress(
            Bytef* dest,
            unsigned long* destLen,
            const Bytef* source,
            unsigned long sourceLen);

        static [[nodiscard]] int Decompress(
            Bytef* dest,
            unsigned long* destLen,
            const Bytef* source,
            unsigned long sourceLen);
	};
} // namespace base::io

#endif
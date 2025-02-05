#pragma once 

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
#pragma once 

#ifdef LIB_BASE_ENABLE_ZLIB

#include <zlib.h>
#include <base/core/BaseNamespace.h>

__BASE_IO_NAMESPACE_BEGIN

class Compressor {
public:
    static   int Compress(
        Bytef* dest,
        unsigned long* destLen,
        const Bytef* source,
        unsigned long sourceLen);

    static   int Decompress(
        Bytef* dest,
        unsigned long* destLen,
        const Bytef* source,
        unsigned long sourceLen);
};

__BASE_IO_NAMESPACE_END

#endif
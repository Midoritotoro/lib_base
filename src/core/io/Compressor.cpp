#include <base/core/io/Compressor.h>

#ifdef LIB_BASE_ENABLE_ZLIB

__BASE_IO_NAMESPACE_BEGIN

int Compressor::Compress(
    Bytef* dest,
    unsigned long* destLen,
    const Bytef* source,
    unsigned long sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = const_cast<Bytef*>(source);
    stream.avail_in = (uInt)sourceLen;

    if ((uLong)stream.avail_in != sourceLen)
        return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;

    if ((uLong)stream.avail_out != *destLen)
        return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)nullptr;
    stream.zfree = (free_func)nullptr;

    err = inflateInit2(&stream, -MAX_WBITS);
    if (err != Z_OK)
        return err;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            return Z_DATA_ERROR;

        return err;
    }

    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}

int Compressor::Decompress(
    Bytef* dest,
    unsigned long* destLen,
    const Bytef* source,
    unsigned long sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = const_cast<Bytef*>(source);
    stream.avail_in = (uInt)sourceLen;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;

    if ((uLong)stream.avail_out != *destLen)
        return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)nullptr;
    stream.zfree = (free_func)nullptr;
    stream.opaque = (voidpf)nullptr;

    err = deflateInit2(
        &stream, Z_DEFAULT_COMPRESSION,
        Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);

    if (err != Z_OK)
        return err;

    err = deflate(&stream, Z_FINISH);

    if (err != Z_STREAM_END) {
        deflateEnd(&stream);
        return err == Z_OK
            ? Z_BUF_ERROR
            : err;
    }

    *destLen = stream.total_out;

    err = deflateEnd(&stream);
    return err;
}

__BASE_IO_NAMESPACE_END

#endif

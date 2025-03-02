#pragma once 

#include <base/system/Platform.h>
#include <base/core/BaseNamespace.h>

__BASE_NAMESPACE_BEGIN

template <typename T> always_inline void ToUnaligned(const T src, void* dest)
{
    const size_t size = sizeof(T);
#if __has_builtin(__builtin_memcpy)
    __builtin_memcpy
#else
    memcpy
#endif
        (dest, &src, size);
}

template <typename T> always_inline T FromUnaligned(const void* src)
{
    T dest;
    const size_t size = sizeof(T);
#if __has_builtin(__builtin_memcpy)
    __builtin_memcpy
#else
    memcpy
#endif
        (&dest, src, size);
    return dest;
}

inline constexpr uint64 bswap_helper(uint64 source)
{
    return 0
        | ((source & UINT64_CAST(0x00000000000000ff)) << 56)
        | ((source & UINT64_CAST(0x000000000000ff00)) << 40)
        | ((source & UINT64_CAST(0x0000000000ff0000)) << 24)
        | ((source & UINT64_CAST(0x00000000ff000000)) << 8)
        | ((source & UINT64_CAST(0x000000ff00000000)) >> 8)
        | ((source & UINT64_CAST(0x0000ff0000000000)) >> 24)
        | ((source & UINT64_CAST(0x00ff000000000000)) >> 40)
        | ((source & UINT64_CAST(0xff00000000000000)) >> 56);
}

inline constexpr uint32 bswap_helper(uint32 source)
{
    return 0
        | ((source & 0x000000ff) << 24)
        | ((source & 0x0000ff00) << 8)
        | ((source & 0x00ff0000) >> 8)
        | ((source & 0xff000000) >> 24);
}

inline constexpr uint16 bswap_helper(uint16 source)
{
    return uint16(0
        | ((source & 0x00ff) << 8)
        | ((source & 0xff00) >> 8));
}

inline constexpr uint8 bswap_helper(uint8 source)
{
    return source;
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr T bswap(T source)
{
    return T(bswap_helper(typename IntegerForSizeof<T>::Unsigned(source)));
}

template<typename Float>
Float bswapFloatHelper(Float source)
{
    auto temp = FromUnaligned<typename IntegerForSizeof<Float>::Unsigned>(&source);
    temp = bswap(temp);
    return FromUnaligned<Float>(&temp);
}

inline float bswap(float source)
{
    return bswapFloatHelper(source);
}

inline float bswap(float source)
{
    return bswapFloatHelper(source);
}

inline double bswap(double source)
{
    return bswapFloatHelper(source);
}


#if BYTE_ORDER == BIG_ENDIAN
template <typename T> inline constexpr T ToBigEndian(T source)
{
    return source;
}

template <typename T> inline constexpr T FromBigEndian(T source)
{
    return source;
}

template <typename T> inline constexpr T ToLittleEndian(T source)
{
    return bswap(source);
}

template <typename T> inline constexpr T FromLittleEndian(T source)
{
    return bswap(source);
}

template <typename T> inline void ToBigEndian(T src, void* dest)
{
    ToUnaligned<T>(src, dest);
}

template <typename T> inline void ToLittleEndian(T src, void* dest)
{
    bswap<T>(src, dest);
}

template <typename T> inline void ToBigEndian(const void* source, sizetype count, void* dest)
{
    if (source != dest) 
        memcpy(dest, source, count * sizeof(T));
}

template <typename T> inline void ToLittleEndian(const void* source, sizetype count, void* dest)
{
    bswap<sizeof(T)>(source, count, dest);
}

template <typename T> inline void FromBigEndian(const void* source, sizetype count, void* dest)
{
    if (source != dest) memcpy(dest, source, count * sizeof(T));
}

template <typename T> inline void FromLittleEndian(const void* source, sizetype count, void* dest)
{
    bswap<sizeof(T)>(source, count, dest);
}

#else // LITTLE_ENDIAN

template <typename T> inline constexpr T ToBigEndian(T source)
{
    return bswap(source);
}

template <typename T> inline constexpr T FromBigEndian(T source)
{
    return bswap(source);
}

template <typename T> inline constexpr T ToLittleEndian(T source)
{
    return source;
}

template <typename T> inline constexpr T FromLittleEndian(T source)
{
    return source;
}

template <typename T> inline void ToBigEndian(T src, void* dest)
{
    bswap<T>(src, dest);
}

template <typename T> inline void ToLittleEndian(T src, void* dest)
{
    ToUnaligned<T>(src, dest);
}

template <typename T> inline void ToBigEndian(const void* source, sizetype count, void* dest)
{
    bswap<sizeof(T)>(source, count, dest);
}

template <typename T> inline void ToLittleEndian(const void* source, sizetype count, void* dest)
{
    if (source != dest) 
        memcpy(dest, source, count * sizeof(T));
}

template <typename T> inline void FromBigEndian(const void* source, sizetype count, void* dest)
{
    bswap<sizeof(T)>(source, count, dest);
}

template <typename T> inline void FromLittleEndian(const void* source, sizetype count, void* dest)
{
    if (source != dest) 
        memcpy(dest, source, count * sizeof(T));
}
#endif // BYTE_ORDER == BIG_ENDIAN

__BASE_NAMESPACE_END

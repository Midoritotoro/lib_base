#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h>

#include <type_traits>

__BASE_ARCH_NAMESPACE_BEGIN

template <typename T> base_always_inline void ToUnaligned(const T src, void* dest)
{
    const size_t size = sizeof(T);
#if __has_builtin(__builtin_memcpy)
    __builtin_memcpy
#else
    memcpy
#endif
        (dest, &src, size);
}

template <typename T> base_always_inline T FromUnaligned(const void* src)
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

inline double bswap(double source)
{
    return bswapFloatHelper(source);
}

/*
 * bswap(const T src, const void *dest);
 * Changes the byte order of \a src from big-endian to little-endian or vice versa
 * and stores the result in \a dest.
 * There is no alignment requirements for \a dest.
*/
template <typename T> inline void bswap(const T src, void* dest)
{
    ToUnaligned<T>(bswap(src), dest);
}

template <int Size> void* bswap(const void* source, sizetype count, void* dest) noexcept;
template<> inline void* bswap<1>(const void* source, sizetype count, void* dest) noexcept
{
    return source != dest ? memcpy(dest, source, size_t(count)) : dest;
}
template<> void* bswap<2>(const void* source, sizetype count, void* dest) noexcept;
template<> void* bswap<4>(const void* source, sizetype count, void* dest) noexcept;
template<> void* bswap<8>(const void* source, sizetype count, void* dest) noexcept;


#if base_byte_order == base_big_endian
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

#else // base_little_endian

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

template <typename T> inline T FromLittleEndian(const void* src)
{
    return FromLittleEndian(FromUnaligned<T>(src));
}

template <> inline uint8 FromLittleEndian<quint8>(const void* src)
{
    return static_cast<const quint8*>(src)[0];
}
template <> inline int8 FromLittleEndian<int8>(const void* src)
{
    return static_cast<const int8*>(src)[0];
}

template <class T> inline T FromBigEndian(const void* src)
{
    return FromBigEndian(FromUnaligned<T>(src));
}

template <> inline uint8 FromBigEndian<uint8>(const void* src)
{
    return static_cast<const uint8*>(src)[0];
}
template <> inline int8 FromBigEndian<int8>(const void* src)
{
    return static_cast<const int8*>(src)[0];
}
#endif // BYTE_ORDER == BIG_ENDIAN

__BASE_ARCH_NAMESPACE_END

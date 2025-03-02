#include <base/io/ByteArrayMatcher.h>

#include <limits.h>

__BASE_IO_NAMESPACE_BEGIN

static inline void bm_init_skiptable(const uchar* cc, sizetype len, uchar* skiptable)
{
    int l = int(qMin(len, sizetype(255)));
    memset(skiptable, l, 256 * sizeof(uchar));
    cc += len - l;
    while (l--)
        skiptable[*cc++] = l;
}

static inline sizetype bm_find(const uchar* cc, sizetype l, sizetype index, const uchar* puc,
    sizetype pl, const uchar* skiptable)
{
    if (pl == 0)
        return index > l ? -1 : index;
    const sizetype pl_minus_one = pl - 1;

    const uchar* current = cc + index + pl_minus_one;
    const uchar* end = cc + l;
    while (current < end) {
        sizetype skip = skiptable[*current];
        if (!skip) {
            // possible match
            while (skip < pl) {
                if (*(current - skip) != puc[pl_minus_one - skip])
                    break;
                skip++;
            }
            if (skip > pl_minus_one) // we have a match
                return (current - cc) - skip + 1;

            // in case we don't have a match we are a bit inefficient as we only skip by one
            // when we have the non matching char in the string.
            if (skiptable[*(current - skip)] == pl)
                skip = pl - skip;
            else
                skip = 1;
        }
        if (current > end - skip)
            break;
        current += skip;
    }
    return -1; // not found
}

ByteArrayMatcher::ByteArrayMatcher()
{
    p.p = nullptr;
    p.l = 0;
    memset(p.q_skiptable, 0, sizeof(p.q_skiptable));
}

ByteArrayMatcher::ByteArrayMatcher(const char* pattern, sizetype length) 
{
    p.p = reinterpret_cast<const uchar*>(pattern);
    if (length < 0)
        p.l = qstrlen(pattern);
    else
        p.l = length;
    bm_init_skiptable(p.p, p.l, p.q_skiptable);
}

ByteArrayMatcher::ByteArrayMatcher(const ByteArray& pattern)
    : q_pattern(pattern)
{
    p.p = reinterpret_cast<const uchar*>(pattern.constData());
    p.l = pattern.size();
    bm_init_skiptable(p.p, p.l, p.q_skiptable);
}

ByteArrayMatcher::ByteArrayMatcher(const ByteArrayMatcher& other)
{
    operator=(other);
}


ByteArrayMatcher::~ByteArrayMatcher()
{
}


ByteArrayMatcher& ByteArrayMatcher::operator=(const ByteArrayMatcher& other)
{
    q_pattern = other.q_pattern;
    memcpy(&p, &other.p, sizeof(p));
    return *this;
}

void ByteArrayMatcher::setPattern(const ByteArray& pattern)
{
    q_pattern = pattern;
    p.p = reinterpret_cast<const uchar*>(pattern.constData());
    p.l = pattern.size();
    bm_init_skiptable(p.p, p.l, p.q_skiptable);
}

sizetype ByteArrayMatcher::indexIn(const char* str, sizetype len, sizetype from) const
{
    if (from < 0)
        from = 0;
    return bm_find(reinterpret_cast<const uchar*>(str), len, from,
        p.p, p.l, p.q_skiptable);
}

sizetype ByteArrayMatcher::indexIn(ByteArrayView data, sizetype from) const
{
    if (from < 0)
        from = 0;
    return bm_find(reinterpret_cast<const uchar*>(data.data()), data.size(), from,
        p.p, p.l, p.q_skiptable);
}

static sizetype findChar(const char* str, sizetype len, char ch, sizetype from)
{
    const uchar* s = (const uchar*)str;
    uchar c = (uchar)ch;
    if (from < 0)
        from = qMax(from + len, sizetype(0));
    if (from < len) {
        const uchar* n = s + from - 1;
        const uchar* e = s + len;
        while (++n != e)
            if (*n == c)
                return  n - s;
    }
    return -1;
}

/*!
    \internal
 */
static sizetype FindByteArrayBoyerMoore(
    const char* haystack, sizetype haystackLen, sizetype haystackOffset,
    const char* needle, sizetype needleLen)
{
    uchar skiptable[256];
    bm_init_skiptable((const uchar*)needle, needleLen, skiptable);
    if (haystackOffset < 0)
        haystackOffset = 0;
    return bm_find((const uchar*)haystack, haystackLen, haystackOffset,
        (const uchar*)needle, needleLen, skiptable);
}

#define REHASH(a) \
    if (sl_minus_1 < sizeof(std::size_t) * CHAR_BIT) \
        hashHaystack -= std::size_t(a) << sl_minus_1; \
    hashHaystack <<= 1


sizetype FindByteArray(
    const char* haystack0, sizetype haystackLen, sizetype from,
    const char* needle, sizetype needleLen)
{
    const auto l = haystackLen;
    const auto sl = needleLen;
    if (from < 0)
        from += l;
    if (std::size_t(sl + from) > std::size_t(l))
        return -1;
    if (!sl)
        return from;
    if (!l)
        return -1;

    if (sl == 1)
        return findChar(haystack0, haystackLen, needle[0], from);

    /*
      We use the Boyer-Moore algorithm in cases where the overhead
      for the skip table should pay off, otherwise we use a simple
      hash function.
    */
    if (l > 500 && sl > 5)
        return FindByteArrayBoyerMoore(haystack0, haystackLen, from,
            needle, needleLen);

    /*
      We use some hashing for efficiency's sake. Instead of
      comparing strings, we compare the hash value of str with that
      of a part of this QString. Only if that matches, we call memcmp().
    */
    const char* haystack = haystack0 + from;
    const char* end = haystack0 + (l - sl);
    const auto sl_minus_1 = std::size_t(sl - 1);
    std::size_t hashNeedle = 0, hashHaystack = 0;
    qsizetype idx;
    for (idx = 0; idx < sl; ++idx) {
        hashNeedle = ((hashNeedle << 1) + needle[idx]);
        hashHaystack = ((hashHaystack << 1) + haystack[idx]);
    }
    hashHaystack -= *(haystack + sl_minus_1);

    while (haystack <= end) {
        hashHaystack += *(haystack + sl_minus_1);
        if (hashHaystack == hashNeedle && *needle == *haystack
            && memcmp(needle, haystack, sl) == 0)
            return haystack - haystack0;

        REHASH(*haystack);
        ++haystack;
    }
    return -1;
}

sizetype StaticByteArrayMatcherBase::indexOfIn(const char* needle, size_t nlen, const char* haystack, sizetype hlen, sizetype from) const noexcept
{
    if (from < 0)
        from = 0;
    return bm_find(reinterpret_cast<const uchar*>(haystack), hlen, from,
        reinterpret_cast<const uchar*>(needle), nlen, m_skiptable.data);
}


__BASE_IO_NAMESPACE_END

#undef REHASH

#include <base/string/StringMatcher.h>

static constexpr sizetype FoldBufferCapacity = 256;

static void bm_init_skiptable(QStringView needle, uchar *skiptable, Qt::CaseSensitivity cs)
{
    const char16_t *uc = needle.utf16();
    const sizetype len =
            cs == Qt::CaseSensitive ? needle.size() : qMin(needle.size(), FoldBufferCapacity);
    int l = qMin(int(len), 255);
    memset(skiptable, l, 256 * sizeof(uchar));
    uc += len - l;
    if (cs == Qt::CaseSensitive) {
        while (l--) {
            skiptable[*uc & 0xff] = l;
            ++uc;
        }
    } else {
        const char16_t *start = uc;
        while (l--) {
            skiptable[foldCase(uc, start) & 0xff] = l;
            ++uc;
        }
    }
}

static inline sizetype bm_find(QStringView haystack, sizetype index, QStringView needle,
                          const uchar *skiptable, Qt::CaseSensitivity cs)
{
    const char16_t *uc = haystack.utf16();
    const sizetype l = haystack.size();
    const char16_t *puc = needle.utf16();
    const sizetype pl = needle.size();

    if (pl == 0)
        return index > l ? -1 : index;

    if (cs == Qt::CaseSensitive) {
        const sizetype pl_minus_one = pl - 1;
        const char16_t *current = uc + index + pl_minus_one;
        const char16_t *end = uc + l;

        while (current < end) {
            sizetype skip = skiptable[*current & 0xff];
            if (!skip) {
                // possible match
                while (skip < pl) {
                    if (*(current - skip) != puc[pl_minus_one-skip])
                        break;
                    ++skip;
                }
                if (skip > pl_minus_one) // we have a match
                    return (current - uc) - pl_minus_one;

                // in case we don't have a match we are a bit inefficient as we only skip by one
                // when we have the non matching char in the string.
                if (skiptable[*(current - skip) & 0xff] == pl)
                    skip = pl - skip;
                else
                    skip = 1;
            }
            if (current > end - skip)
                break;
            current += skip;
        }
    } else {
        char16_t foldBuffer[FoldBufferCapacity];
        const sizetype foldBufferLength = qMin(FoldBufferCapacity, pl);
        const char16_t *start = puc;
        for (sizetype i = 0; i < foldBufferLength; ++i)
            foldBuffer[i] = foldCase(&puc[i], start);
        QStringView restNeedle = needle.sliced(foldBufferLength);
        const sizetype foldBufferEnd = foldBufferLength - 1;
        const char16_t *current = uc + index + foldBufferEnd;
        const char16_t *end = uc + l;

        while (current < end) {
            sizetype skip = skiptable[foldCase(current, uc) & 0xff];
            if (!skip) {
                // possible match
                while (skip < foldBufferLength) {
                    if (foldCase(current - skip, uc) != foldBuffer[foldBufferEnd - skip])
                        break;
                    ++skip;
                }
                if (skip > foldBufferEnd) { // Matching foldBuffer
                    sizetype candidatePos = (current - uc) - foldBufferEnd;
                    QStringView restHaystack =
                            haystack.sliced(qMin(haystack.size(), candidatePos + foldBufferLength));
                    if (restNeedle.size() == 0
                        || restHaystack.startsWith(
                                restNeedle, Qt::CaseInsensitive)) // Check the rest of the string
                        return candidatePos;
                }
                // in case we don't have a match we are a bit inefficient as we only skip by one
                // when we have the non matching char in the string.
                if (skiptable[foldCase(current - skip, uc) & 0xff] == foldBufferLength)
                    skip = foldBufferLength - skip;
                else
                    skip = 1;
            }
            if (current > end - skip)
                break;
            current += skip;
        }
    }
    return -1; // not found
}

void QStringMatcher::updateSkipTable()
{
    bm_init_skiptable(q_sv, q_skiptable, q_cs);
}

QStringMatcher::QStringMatcher(const QString &pattern, Qt::CaseSensitivity cs)
    : d_ptr(nullptr), q_cs(cs), q_pattern(pattern)
{
    q_sv = q_pattern;
    updateSkipTable();
}

QStringMatcher::QStringMatcher(QStringView str, Qt::CaseSensitivity cs)
    : d_ptr(nullptr), q_cs(cs), q_sv(str)
{
    updateSkipTable();
}

QStringMatcher::QStringMatcher(const QStringMatcher &other)
    : d_ptr(nullptr)
{
    operator=(other);
}

QStringMatcher::~QStringMatcher()
{
    Q_UNUSED(d_ptr);
}

QStringMatcher &QStringMatcher::operator=(const QStringMatcher &other)
{
    if (this != &other) {
        q_pattern = other.q_pattern;
        q_cs = other.q_cs;
        q_sv = other.q_sv;
        memcpy(q_skiptable, other.q_skiptable, sizeof(q_skiptable));
    }
    return *this;
}

void QStringMatcher::setPattern(const QString &pattern)
{
    q_pattern = pattern;
    q_sv = q_pattern;
    updateSkipTable();
}

QString QStringMatcher::pattern() const
{
    if (!q_pattern.isEmpty())
        return q_pattern;
    return q_sv.toString();
}


void QStringMatcher::setCaseSensitivity(Qt::CaseSensitivity cs)
{
    if (cs == q_cs)
        return;
    q_cs = cs;
    updateSkipTable();
}

sizetype QStringMatcher::indexIn(QStringView str, sizetype from) const
{
    if (from < 0)
        from = 0;
    return bm_find(str, from, q_sv, q_skiptable, q_cs);
}

sizetype qFindStringBoyerMoore(
    QStringView haystack, sizetype haystackOffset,
    QStringView needle, Qt::CaseSensitivity cs)
{
    uchar skiptable[256];
    bm_init_skiptable(needle, skiptable, cs);
    if (haystackOffset < 0)
        haystackOffset = 0;
    return bm_find(haystack, haystackOffset, needle, skiptable, cs);
}

QT_END_NAMESPACE

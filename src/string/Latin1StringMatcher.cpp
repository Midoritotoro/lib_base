#include <base/string/Latin1StringMatcher.h>

#include <limits.h>

namespace base::string {
    Latin1StringMatcher::Latin1StringMatcher() noexcept
        : m_pattern(),
        m_cs(Qt::CaseSensitive),
        m_caseSensitiveSearcher(m_pattern.data(), m_pattern.data())
    {
    }

    Latin1StringMatcher::Latin1StringMatcher(Latin1StringView pattern,
        CaseSensitivity cs) noexcept
        : m_pattern(pattern), m_cs(cs)
    {
        setSearcher();
    }

    Latin1StringMatcher::~Latin1StringMatcher() noexcept
    {
        freeSearcher();
    }

    void Latin1StringMatcher::setSearcher() noexcept
    {
        if (m_cs == CaseSensitive) {
            new (&m_caseSensitiveSearcher) CaseSensitiveSearcher(m_pattern.data(), m_pattern.end());
        }
        else {
            QtPrivate::QCaseInsensitiveLatin1Hash foldCase;
            sizetype bufferSize = std::min(m_pattern.size(), sizetype(sizeof m_foldBuffer));
            for (sizetype i = 0; i < bufferSize; ++i)
                m_foldBuffer[i] = static_cast<char>(foldCase(m_pattern[i].toLatin1()));

            new (&m_caseInsensitiveSearcher)
                CaseInsensitiveSearcher(m_foldBuffer, &m_foldBuffer[bufferSize]);
        }
    }

    void Latin1StringMatcher::freeSearcher() noexcept
    {
        if (m_cs == Qt::CaseSensitive)
            m_caseSensitiveSearcher.~CaseSensitiveSearcher();
        else
            m_caseInsensitiveSearcher.~CaseInsensitiveSearcher();
    }

    void Latin1StringMatcher::setPattern(Latin1StringView pattern) noexcept
    {
        if (m_pattern.latin1() == pattern.latin1() && m_pattern.size() == pattern.size())
            return; // Same address and size

        freeSearcher();
        m_pattern = pattern;
        setSearcher();
    }

    Latin1StringView Latin1StringMatcher::pattern() const noexcept
    {
        return m_pattern;
    }

    void Latin1StringMatcher::setCaseSensitivity(CaseSensitivity cs) noexcept
    {
        if (m_cs == cs)
            return;

        freeSearcher();
        m_cs = cs;
        setSearcher();
    }

    CaseSensitivity Latin1StringMatcher::caseSensitivity() const noexcept
    {
        return m_cs;
    }

    sizetype Latin1StringMatcher::indexIn(Latin1StringView haystack, sizetype from) const noexcept
    {
        if (m_pattern.isEmpty() && from == haystack.size())
            return from;
        if (from < 0)
            from += haystack.size();
        if (from >= haystack.size())
            return -1;

        auto begin = haystack.begin() + from;
        auto end = haystack.end();
        auto found = begin;
        if (m_cs == CaseSensitive) {
            found = m_caseSensitiveSearcher(begin, end, m_pattern.begin(), m_pattern.end()).begin;
            if (found == end)
                return -1;
        }
        else {
            const sizetype bufferSize = std::min(m_pattern.size(), sizetype(sizeof m_foldBuffer));
            const Latin1StringView restNeedle = m_pattern.sliced(bufferSize);
            const bool needleLongerThanBuffer = restNeedle.size() > 0;
            Latin1StringView restHaystack = haystack;
            do {
                found = m_caseInsensitiveSearcher(found, end, m_foldBuffer, &m_foldBuffer[bufferSize])
                    .begin;
                if (found == end) {
                    return -1;
                }
                else if (!needleLongerThanBuffer) {
                    break;
                }
                restHaystack = haystack.sliced(
                    qMin(haystack.size(),
                        bufferSize + sizetype(std::distance(haystack.begin(), found))));
                if (restHaystack.startsWith(restNeedle, Qt::CaseInsensitive))
                    break;
                ++found;
            } while (true);
        }
        return std::distance(haystack.begin(), found);
    }
} // namespace base::string
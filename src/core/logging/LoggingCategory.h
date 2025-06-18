#pragma once 

#include <src/core/logging/MessageContext.h>
#include <base/core/thread/AtomicInteger.h>

__BASE_LOGGING_NAMESPACE_BEGIN


class LoggingCategory
{
    DISABLE_COPY(LoggingCategory)
public:
    explicit LoggingCategory(const char* category, LogMessageType severityLevel = LogMessageType::DebugMessage);
    ~LoggingCategory();

    bool isEnabled(LogMessageType type) const;
    void setEnabled(LogMessageType type, bool enable);

    bool isDebugEnabled() const;
    bool isInfoEnabled() const;

    bool isWarningEnabled() const;
    bool isCriticalEnabled() const;

    const char* categoryName() const;

    LoggingCategory& operator()();
    const LoggingCategory& operator()() const;

    static LoggingCategory* defaultCategory();

    typedef void (*CategoryFilter)(LoggingCategory*);
    static CategoryFilter installFilter(CategoryFilter);

    static void setFilterRules(const string::String& rules);

private:
    void init(const char* category, LogMessageType severityLevel);

    const char* name;

    struct AtomicBools {
        thread::AtomicInteger<bool> enabledDebug;
        thread::AtomicInteger<bool> enabledWarning;
        thread::AtomicInteger<bool> enabledCritical;
        thread::AtomicInteger<bool> enabledInfo;
    };

    union {
        AtomicBools bools;
        thread::AtomicInteger<int> enabled;
    };
};

__BASE_LOGGING_NAMESPACE_END

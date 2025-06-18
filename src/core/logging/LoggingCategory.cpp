#include <src/core/logging/LoggingCategory.h>
#include <src/core/logging/LoggingRegistry.h>


__BASE_LOGGING_NAMESPACE_BEGIN

static inline constexpr const char defaultCategoryName[] = "default";

LoggingCategory::LoggingCategory(
    const char* category, 
    LogMessageType enableForLevel
)
    : name(nullptr)
{
    init(category, enableForLevel);
}

//void LoggingCategory::init(const char* category, LogMessageType severityLevel)
//{
//    enabled.storeRelaxed(0x01010101);   // enabledDebug = enabledWarning = enabledCritical = true;
//
//    if (category)
//        name = category;
//    else
//        name = defaultCategoryName;
//
//    if (LoggingRegistry* reg = LoggingRegistry::instance())
//        reg->registerCategory(this, severityLevel);
//}
//
//LoggingCategory::~LoggingCategory()
//{
//    if (LoggingRegistry* reg = LoggingRegistry::instance())
//        reg->unregisterCategory(this);
//}

bool LoggingCategory::isEnabled(LogMessageType msgtype) const
{
    switch (msgtype) {
        case LogMessageType::DebugMessage:          return isDebugEnabled();
        case LogMessageType::InfoMessage:           return isInfoEnabled();
        case LogMessageType::WarningMessage:        return isWarningEnabled();
        case LogMessageType::CriticalErrorMessage:  return isCriticalEnabled();
        case LogMessageType::FatalErrorMessage:     return true;
    }

    return false;
}

void LoggingCategory::setEnabled(LogMessageType type, bool enable)
{
    switch (type) {
        case LogMessageType::DebugMessage:          bools.enabledDebug.storeRelaxed(enable); break;
        case LogMessageType::InfoMessage:           bools.enabledInfo.storeRelaxed(enable); break;
        case LogMessageType::WarningMessage:        bools.enabledWarning.storeRelaxed(enable); break;
        case LogMessageType::CriticalErrorMessage:  bools.enabledCritical.storeRelaxed(enable); break;
        case LogMessageType::FatalErrorMessage:     break;
    }
}

LoggingCategory* LoggingCategory::defaultCategory()
{
    return new LoggingCategory(defaultCategoryName);
}

//LoggingCategory::CategoryFilter LoggingCategory::installFilter(LoggingCategory::CategoryFilter filter)
//{
//    return LoggingRegistry::instance()->installFilter(filter);
//}
//
//void LoggingCategory::setFilterRules(const QString& rules)
//{
//    LoggingRegistry::instance()->setApiRules(rules);
//}
//

bool LoggingCategory::isDebugEnabled() const {
    return bools.enabledDebug.loadRelaxed();
}

bool LoggingCategory::isInfoEnabled() const {
    return bools.enabledInfo.loadRelaxed();
}

bool LoggingCategory::isWarningEnabled() const {
    return bools.enabledWarning.loadRelaxed();
}

bool LoggingCategory::isCriticalEnabled() const {
    return bools.enabledCritical.loadRelaxed();
}

const char* LoggingCategory::categoryName() const {
    return name;
}

LoggingCategory& LoggingCategory::operator()() {
    return *this;
}

const LoggingCategory& LoggingCategory::operator()() const {
    return *this;
}

__BASE_LOGGING_NAMESPACE_END

#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/type_traits/TypeInfo.h>

#include <base/core/string/String.h>
#include <src/core/logging/LoggingCategory.h>

#include <base/core/thread/Mutex.h>
#include <map>

__BASE_STRING_NAMESPACE_BEGIN
class StringView;
__BASE_STRING_NAMESPACE_END


__BASE_LOGGING_NAMESPACE_BEGIN

#define LOGGING_CATEGORY_WITH_ENV_OVERRIDE(name, env, categoryName) \
    const LoggingCategory &name() \
    { \
        static constexpr char cname[] = categoryName;                               \
        static_assert(cname[0] == 'q' && cname[1] == 't' && cname[2] == '.'         \
                      && cname[4] != '\0', "Category name must start with 'qt.'");  \
        static const LoggingCategoryWithEnvironmentOverride category(cname, env);  \
        return category;                                                            \
    }

class LoggingRule
{
public:
    LoggingRule();
    LoggingRule(string::StringView pattern, bool enabled);
    int pass(string::StringView categoryName, LogMessageType type) const;

    enum PatternFlag {
        FullText = 0x1,
        LeftFilter = 0x2,
        RightFilter = 0x4,
        MidFilter = LeftFilter | RightFilter
    };

    DECLARE_FLAGS_ENUM(PatternFlags, PatternFlag)

    string::String category;
    int messageType;
    PatternFlags flags;
    bool enabled;

private:
    void parse(string::StringView pattern);
};

// DECLARE_TYPEINFO(LoggingRule, RELOCATABLE_TYPE);

class LoggingSettingsParser
{
public:
    void setImplicitRulesSection(bool inRulesSection) { 
        m_inRulesSection = inRulesSection;
    }

    void setContent(string::StringView content);

    std::vector<LoggingRule> rules() const { return _rules; }

private:
    void parseNextLine(string::StringView line);

private:
    bool m_inRulesSection = false;
    std::vector<LoggingRule> _rules;
};

class LoggingRegistry
{
    DISABLE_COPY(LoggingRegistry)
public:
    LoggingRegistry();

    void initializeRules();

    void registerCategory(LoggingCategory* category, LogMessageType enableForLevel);
    void unregisterCategory(LoggingCategory* category);

    void registerEnvironmentOverrideForCategory(const char* categoryName, const char* environment);
    void setApiRules(const string::Utf16String& content);

    LoggingCategory::CategoryFilter installFilter(LoggingCategory::CategoryFilter filter);
    static LoggingRegistry* instance();
private:
    void updateRules();

    static void defaultCategoryFilter(LoggingCategory* category);

    enum RuleSet {
        // sorted by order in which defaultCategoryFilter considers them:
        ConfigRules,
        ApiRules,
        EnvironmentRules,
        NumRuleSets
    };

    thread::Mutex registryMutex;

    // protected by mutex:
    std::vector<LoggingRule> ruleSets[NumRuleSets];
    std::map<LoggingCategory*, LogMessageType> categories;
    LoggingCategory::CategoryFilter categoryFilter;
    std::map<string::StringView, const char*> categoryEnvironmentOverrides; // Byte array view
};

class LoggingCategoryWithEnvironmentOverride : public LoggingCategory
{
public:
    LoggingCategoryWithEnvironmentOverride(
        const char* category,
        const char* environment
    ):
        LoggingCategory(registerOverride(category, environment), LogMessageType::InfoMessage)
    {}

private:
    static const char* registerOverride(const char* categoryName, const char* environment)
    {
        LoggingRegistry* c = LoggingRegistry::instance();
        if (c)
            c->registerEnvironmentOverrideForCategory(categoryName, environment);
        return categoryName;
    }
};

__BASE_LOGGING_NAMESPACE_END

#pragma once 

#include <base/core/arch/Platform.h> 
#include <base/core/utility/ClassHelperMacros.h>

#include <base/core/string/String.h>

__BASE_LOGGING_NAMESPACE_BEGIN

class Debug;
class MessageLogger;

enum class LogMessageType : uchar {
    DebugMessage            = 0x00,
    WarningMessage          = 0x01,
    CriticalErrorMessage    = 0x02,
    FatalErrorMessage       = 0x04,
    InfoMessage             = 0x08,
};

class MessageContext
{
    DISABLE_COPY(MessageContext)
public:
    constexpr MessageContext() noexcept = default;
    constexpr MessageContext(
        const char *fileName,
        int         lineNumber, 
        const char *functionName, 
        const char *categoryName
    ) noexcept : 
        _line(lineNumber), 
        _file(fileName), 
        _function(functionName),
        _category(categoryName) 
    {}

    int _version = 2;
    int _line = 0;

    const char* _file = nullptr;
    const char* _function = nullptr;
    const char* _category = nullptr;
private:
    MessageContext& copyContextFrom(const MessageContext& logContext) noexcept;

    friend class MessageLogger;
    friend class Debug;
};

__BASE_LOGGING_NAMESPACE_END

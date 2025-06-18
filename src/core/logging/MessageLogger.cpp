#include <src/core/logging/MessageContext.h>

__BASE_LOGGING_NAMESPACE_BEGIN

MessageContext& MessageContext::copyContextFrom(const MessageContext& logContext) noexcept
{
    _category = logContext._category;
    _file = logContext._file;

    _line = logContext._line;
    _function = logContext._function;

    return *this;
}

__BASE_LOGGING_NAMESPACE_END

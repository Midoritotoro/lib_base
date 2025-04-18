#pragma once 

#include <base/core/arch/Platform.h>

__BASE_THREAD_NAMESPACE_BEGIN

class AbstractThread {
public:
    enum Priority {
        IdlePriority,

        LowestPriority,
        LowPriority,

        NormalPriority,
        HighPriority,

        HighestPriority,
        TimeCriticalPriority,

        InheritPriority
    };

    virtual void setPriority(Priority priority) = 0;
    virtual Priority priority() const noexcept = 0;

    virtual bool isFinished() const noexcept = 0;
    virtual bool isRunning() const noexcept = 0;

    virtual void setTerminateOnClose(bool _terminateOnClose) = 0;
    virtual bool terminateOnClose() const noexcept = 0;

    virtual void waitMs(sizetype milliseconds) = 0;
    virtual sizetype threadId() const noexcept = 0;

    virtual void close() = 0;
    virtual void terminate() = 0;

    virtual void join() = 0;

};

__BASE_THREAD_NAMESPACE_END

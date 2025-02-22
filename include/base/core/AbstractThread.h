#pragma once 

#include <base/system/Platform.h>


namespace base {
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
	};
} // namespace base
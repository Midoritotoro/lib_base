#pragma once 


namespace base {
	class AbstractThread {
	public:
        enum Priority {
            LowestPriority,
            LowPriority,

            NormalPriority,
            HighPriority,

            HighestPriority
        };

        virtual void setPriority(Priority priority) = 0;
        virtual Priority priority() const noexcept = 0;

        virtual bool isFinished() const = 0;
        virtual bool isRunning() const = 0;

	};
} // namespace base
#pragma once 

#include <base/system/Platform.h>

namespace base {
	class AbstractMutex {
	public:
		virtual bool lock() = 0;
		virtual bool unlock() = 0;

		virtual bool isLocked() const noexcept = 0;
	};
}
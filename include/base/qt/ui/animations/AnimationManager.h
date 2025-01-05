#pragma once 

#include <base/qt/common/Timer.h>
#include <base/qt/ui/animations/AnimationBase.h>


namespace base::qt::ui::animations {
	class AnimationManager {
	public:
		AnimationManager() = default;
		AnimationManager& operator=(const AnimationManager& other) = default;

		void start(AnimationBase* animation);
		void stop();
	private:
		void update();

		AnimationBase* _currentAnimation;
		common::Timer _timer;

		Time::time_t _animationDurationProgress;
	};
} // namespace base::qt::ui::animations

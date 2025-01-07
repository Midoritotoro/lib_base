#pragma once 

#include <base/qt/common/Timer.h>
#include <base/qt/ui/animations/AnimationBase.h>


namespace base::qt::ui::animations {
	class HorizontalGrowthAnimation;
	class VerticalGrowthAnimation;
	class OpacityAnimation;

	class AnimationManager {
	public:
		AnimationManager() = default;
		AnimationManager& operator=(const AnimationManager& other) = default;

		void start(not_null<AnimationBase*> animation);
		void stop();

		[[nodiscard]] bool animating() const noexcept;
	private:
		void update();

		union {
			HorizontalGrowthAnimation* _horizontal;
			VerticalGrowthAnimation* _vertical;
			OpacityAnimation* _opacity;
		} _animation;

		union {
			float _opacity;
			int _rect;
		} _step;

		AnimationType _currentAnimationType;
		common::Timer _timer;

		int _iterations;
	};
} // namespace base::qt::ui::animations

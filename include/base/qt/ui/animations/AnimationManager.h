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

		[[nodiscard]] bool animating() const noexcept;
	private:
		void update();

		AnimationBase* _animation = nullptr;
		common::Timer _timer;

		float _opacityStep;
	};
} // namespace base::qt::ui::animations

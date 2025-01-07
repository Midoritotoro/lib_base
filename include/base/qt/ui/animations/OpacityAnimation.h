#pragma once 

#include <base/qt/ui/animations/AnimationManager.h>


namespace base::qt::ui::animations {
	class OpacityAnimation : public AnimationBase {
	public: 
		OpacityAnimation();
		OpacityAnimation& operator=(const OpacityAnimation& other) = default;

		void start(
			float from,
			float to,
			Time::time_t duration = kDefaultAnimationDuration,
			Time::time_t updateTimeout = MinimumAnimationUpdateTimeout());

		void stop();

		void restart();
		void restartAfterFinished();

		[[nodiscard]] bool animating() const noexcept;
	private: 
		float _opacityFrom = 1.f;
		float _opacityTo = 1.f;

		AnimationManager _animationManager;
		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations
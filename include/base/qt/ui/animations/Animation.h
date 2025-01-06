#pragma once 

#include <base/Time.h>

#include <base/qt/common/Types.h>
#include <base/qt/common/Timer.h>

#include <base/qt/ui/animations/AnimationManager.h>


namespace base::qt::ui::animations {
	class Animation: public AnimationBase {
	public: 
		Animation() = default;
		Animation& operator=(const Animation& other) = default;

		[[nodiscard]] AnimationType animationType() const noexcept;
		void setAnimationType(AnimationType type);

		void start(
			float from,
			float to,
			Time::time_t duration,
			Time::time_t updateTimeout);
		void start(const OpacityAnimation& animation);

		void stop();

		void restart();
		void restartAfterFinished();

		[[nodiscard]] bool animating() const noexcept;
	private: 
		AnimationType _animationType;
		AnimationManager _animationManager;

		float _currentOpacity;
	};
} // namespace base::qt::ui::animations
#pragma once 

#include <base/Time.h>

#include <base/qt/common/Types.h>
#include <base/qt/common/Timer.h>

#include <base/qt/ui/animations/AnimationManager.h>


namespace base::qt::ui::animations {
	//inline const OpacityAnimation defaultOpacityShowAnimation = OpacityAnimation {
	//	.from = 0,
	//	.to = 1,

	//	.duration = kDefaultAnimationDuration,
	//	.updateTimeout = MinimumAnimationUpdateTimeout()
	//};

	//inline const OpacityAnimation defaultOpacityHideAnimation = OpacityAnimation{
	//	.from = 1,
	//	.to = 0,

	//	.duration = kDefaultAnimationDuration,
	//	.updateTimeout = MinimumAnimationUpdateTimeout()
	//};

	class Animation: public AnimationBase {
	public: 
		Animation() = default;
		Animation& operator=(const Animation& other) = default;

		[[nodiscard]] AnimationType animationType() const noexcept;
		void setAnimationType(AnimationType type);

		void start(const OpacityAnimation& animation);
		void start(const HorizontalGrowthAnimation& animation);
		void start(const VerticalGrowthAnimation& animation);

		void stop();

		void restart();
		void restartAfterFinished();

		[[nodiscard]] bool animating() const noexcept;
	private: 
		AnimationType _animationType;
		AnimationManager _animationManager;
	};
} // namespace base::qt::ui::animations
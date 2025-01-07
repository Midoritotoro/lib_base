#pragma once

#include <base/qt/ui/animations/AnimationManager.h>


namespace base::qt::ui::animations {
	class HorizontalGrowthAnimation : public AnimationBase {
	public:
		enum class Direction : uchar {
			LeftToRight = 0x01,
			RightToLeft = 0x02
		};

		HorizontalGrowthAnimation();
		HorizontalGrowthAnimation& operator=(const HorizontalGrowthAnimation& other) = default;

		void start(
			QRect rect,
			Direction direction = Direction::LeftToRight,
			Time::time_t duration = kDefaultAnimationDuration,
			Time::time_t updateTimeout = MinimumAnimationUpdateTimeout());

		void stop();

		void restart();
		void restartAfterFinished();

		[[nodiscard]] bool animating() const noexcept;
		[[nodiscard]] QRect rect() const noexcept;
	private:
		int _targetWidth;
		mutable QRect _rect;

		Direction _direction;

		AnimationManager _animationManager;
		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations
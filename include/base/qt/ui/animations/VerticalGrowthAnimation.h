#pragma once


#include <base/qt/ui/animations/AnimationManager.h>


namespace base::qt::ui::animations {
	class VerticalGrowthAnimation : public AnimationBase {
	public:
		enum class Direction : uchar {
			TopToBottom = 0x01,
			BottomToTop = 0x02
		};

		VerticalGrowthAnimation();
		VerticalGrowthAnimation& operator=(const VerticalGrowthAnimation& other) = default;

		void start(
			QRect rect,
			Direction direction = Direction::TopToBottom,
			Time::time_t duration = kDefaultAnimationDuration,
			Time::time_t updateTimeout = MinimumAnimationUpdateTimeout());

		void stop();

		void restart();
		void restartAfterFinished();

		[[nodiscard]] bool animating() const noexcept;
		[[nodiscard]] QRect rect() const noexcept;
	private:
		int _targetHeight;
		mutable QRect _rect;

		Direction _direction;
		AnimationManager _animationManager;

		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations
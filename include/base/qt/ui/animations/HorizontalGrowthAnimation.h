#pragma once

#include <base/qt/ui/animations/AnimationManager.h>

__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

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
	[[nodiscard]] QRectF rect() const noexcept;
private:
	double _targetWidth = 0;
	mutable QRectF _rect;

	Direction _direction;

	AnimationManager* _animationManager = nullptr;
	friend class AnimationManager;
};

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END

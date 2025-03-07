#pragma once

#include <base/qt/ui/animations/AnimationManager.h>


__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

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
	[[nodiscard]] QRectF rect() const noexcept;
private:
	double _targetHeight = 0;
	mutable QRectF _rect;

	Direction _direction;
	AnimationManager* _animationManager = nullptr;

	friend class AnimationManager;
};

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END

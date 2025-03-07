#include <base/qt/ui/animations/HorizontalGrowthAnimation.h>
#include <base/core/async/Concurrent.h>


__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

HorizontalGrowthAnimation::HorizontalGrowthAnimation() {
	_animationType = AnimationType::HorizontalGrowth;
}

void HorizontalGrowthAnimation::start(
	QRect rect,
	Direction direction,
	Time::time_t duration,
	Time::time_t updateTimeout)
{
	Expects(duration != 0);
	Expects(rect.isEmpty() == false);

	_rect = rect;
	_targetWidth = _rect.width();

	_duration = duration;
	_direction = direction;

	_updateTimeout = std::clamp(updateTimeout,
		MinimumAnimationUpdateTimeout(),
		MaximumAnimationUpdateTimeout());

	_animationManager->start(this);
}

void HorizontalGrowthAnimation::stop() {
	_animationManager->stop();
}

void HorizontalGrowthAnimation::restart() {
}

void HorizontalGrowthAnimation::restartAfterFinished() {
	concurrent::invokeAsync([=] {
		while (animating());
		restart();
	});
}

bool HorizontalGrowthAnimation::animating() const noexcept {
	return _animationManager->animating();
}

QRectF HorizontalGrowthAnimation::rect() const noexcept {
	return _rect;
}

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END
#include <base/qt/ui/animations/CombinedGrowthAnimation.h>
#include <base/core/async/Concurrent.h>


__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

CombinedGrowthAnimation::CombinedGrowthAnimation() {
	_animationType = AnimationType::CombinedGrowth;
	_animationManager = new AnimationManager();
}

void CombinedGrowthAnimation::start(
	QRect rect,
	Direction direction,
	Corner start,
	Time::time_t duration,
	Time::time_t updateTimeout)
{
	Expects(duration != 0);
	Expects(rect.isEmpty() == false);

	_rect = rect;

	_direction = direction;

	_duration = duration;
	_startCorner = start;

	_updateTimeout = std::clamp(updateTimeout,
		MinimumAnimationUpdateTimeout(),
		MaximumAnimationUpdateTimeout());

	_animationManager->start(this);
}

void CombinedGrowthAnimation::stop() {
	_animationManager->stop();
}

void CombinedGrowthAnimation::restart() {
}

void CombinedGrowthAnimation::restartAfterFinished() {
	concurrent::invokeAsync([=] {
		while (animating());
		restart();
	});
}

bool CombinedGrowthAnimation::animating() const noexcept {
	return _animationManager->animating();
}

QRectF CombinedGrowthAnimation::rect() const noexcept {
	return _rect;
}

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END

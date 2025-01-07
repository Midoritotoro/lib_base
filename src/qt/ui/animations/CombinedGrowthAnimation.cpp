#include <base/qt/ui/animations/CombinedGrowthAnimation.h>
#include <base/Concurrent.h>


namespace base::qt::ui::animations {
	CombinedGrowthAnimation::CombinedGrowthAnimation() {
		_animationType = AnimationType::CombinedGrowth;
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
		_targetWidth = _rect.width();

		_direction = direction;

		_duration = duration;
		_startCorner = start;

		_updateTimeout = std::clamp(updateTimeout,
			MinimumAnimationUpdateTimeout(),
			MaximumAnimationUpdateTimeout());

		_animationManager.start(this);
	}

	void CombinedGrowthAnimation::stop() {
		_animationManager.stop();
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
		return _animationManager.animating();
	}

	QRectF CombinedGrowthAnimation::rect() const noexcept {
		return _rect;
	}
} // namespace base::qt::ui::animations
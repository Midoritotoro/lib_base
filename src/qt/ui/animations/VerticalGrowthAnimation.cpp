#include <base/qt/ui/animations/VerticalGrowthAnimation.h>
#include <base/Concurrent.h>


namespace base::qt::ui::animations {
	VerticalGrowthAnimation::VerticalGrowthAnimation() {
		_animationType = AnimationType::VerticalGrowth;
	}

	void VerticalGrowthAnimation::start(
		QRect rect,
		Direction direction,
		Time::time_t duration,
		Time::time_t updateTimeout)
	{
		Expects(duration != 0);
		Expects(rect.isEmpty() == false);

		_duration = duration;
		_direction = direction;

		_rect = rect;
		_targetHeight = _rect.height();

		_updateTimeout = std::clamp(updateTimeout,
			MinimumAnimationUpdateTimeout(),
			MaximumAnimationUpdateTimeout());

		_animationManager.start(this);
	}

	void VerticalGrowthAnimation::stop() {
		_animationManager.stop();
	}

	void VerticalGrowthAnimation::restart() {
	}

	void VerticalGrowthAnimation::restartAfterFinished() {
		concurrent::invokeAsync([=] {
			while (animating());
			restart();
		});
	}

	bool VerticalGrowthAnimation::animating() const noexcept {
		return _animationManager.animating();
	}

	QRectF VerticalGrowthAnimation::rect() const noexcept {
		return _rect;
	}
} // namespace base::qt::ui::animations
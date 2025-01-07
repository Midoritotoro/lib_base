#include <base/qt/ui/animations/HorizontalGrowthAnimation.h>
#include <base/Concurrent.h>


namespace base::qt::ui::animations {
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

		_duration = duration;
		_direction = direction;

		_updateTimeout = std::clamp(updateTimeout,
			MinimumAnimationUpdateTimeout(),
			MaximumAnimationUpdateTimeout());

		_animationManager.start(this);
	}

	void HorizontalGrowthAnimation::stop() {
		_animationManager.stop();
	}

	void HorizontalGrowthAnimation::restart() {
	}

	void HorizontalGrowthAnimation::restartAfterFinished() {
		concurrent::invokeAsync([=] {
			while (_animationManager.animating());
			restart();
		});
	}

	bool HorizontalGrowthAnimation::animating() const noexcept {
	}
} // namespace base::qt::ui::animations
#include <base/qt/ui/animations/OpacityAnimation.h>
#include <base/Assert.h>

#include <base/Concurrent.h>


namespace base::qt::ui::animations {
	OpacityAnimation::OpacityAnimation() {
		_animationType = AnimationType::Opacity;
	}

	void OpacityAnimation::start(
		float from,
		float to,
		Time::time_t duration,
		Time::time_t updateTimeout) 
	{
		Expects(duration != 0);
		Expects(from != to);
		_opacity = from;

		_opacityFrom = from;
		_opacityTo = to;

		_duration = duration;
		_updateTimeout = std::clamp(updateTimeout,
			MinimumAnimationUpdateTimeout(),
			MaximumAnimationUpdateTimeout());

		_animationManager.start(this);
	}

	void OpacityAnimation::stop() {
		_animationManager.stop();
	}

	void OpacityAnimation::restart() {
		/*if (animating() == false)
			return start(std::get<VerticalGrowthAnimation>(_animation));

		stop();
		start(_animation);*/
	}

	void OpacityAnimation::restartAfterFinished() {
		concurrent::invokeAsync([=] {
			while (_animationManager.animating());
			restart();
		});
	}

	bool OpacityAnimation::animating() const noexcept {
		return _animationManager.animating();
	}
} // namespace base::qt::ui::animations